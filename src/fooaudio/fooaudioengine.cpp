#include <QApplication>
#include <QPluginLoader>
#include <iostream>
#include <errno.h>
using namespace std;
#include <pthread.h>

#include "../include/fooio.hpp"
#include "fooaudioengine.hpp"

void *sPlayThread (void *unused)
{
	FooAudioEngine *ae = (FooAudioEngine*) unused;
	ae->playThread(NULL);
}

FooAudioEngine::FooAudioEngine ()
{
	request = REQ_NOTHING;
//	currPlayingMut = PTHREAD_MUTEX_INITIALIZER;
//	plistMut = PTHREAD_MUTEX_INITIALIZER;
	playingThread = 0;
	playThreadRunning = 0;
	currPlaying = -1;
	stopPlaying = 0;

	loadPlugins ();
}

void FooAudioEngine::loadPlugins ()
{
	pluginsDir = QDir(qApp->applicationDirPath());
	pluginsDir.cd("plugins");

	QFileInfoList list = pluginsDir.entryInfoList();
	std::cout << "     Bytes Filename" << std::endl;
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName())) << std::endl;
	}

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		std::cout << "dupa " << qPrintable(QString(fileName)) << std::endl;

		if (plugin)
		{
			pluginFileNames += fileName;
			std::cout << qPrintable(QString(fileName)) << std::endl;

			FooAudioInterface *audio_ = qobject_cast<FooAudioInterface *>(plugin);

			if (audio_)
			{
				if (audio_->getType() == 2)
				{
					audioInterface = audio_;
					alsaPlugin = plugin;
				}
			}

			FooMusicFormatInterface *music_ = qobject_cast<FooMusicFormatInterface *>(plugin);

			if (music_)
			{
				if (music_->getType() == 2)
				{
					musicFormatInterface = music_;
					flacPlugin = plugin;
				}
			}
		}
	}
}

QDir FooAudioEngine::getPluginsDir ()
{
	return pluginsDir;
}

QStringList FooAudioEngine::getPluginFileNames ()
{
	return pluginFileNames;
}

void FooAudioEngine::audioPlay (const char *fname, FooPlaylistWidget *playlist)
{
	audioStop ();
	playerReset ();

	pthread_mutex_lock (&currPlayingMut);
	pthread_mutex_lock (&plistMut);

	currPlist = playlist;

	if (*fname)
	{
		currPlaying = currPlist->plistFindFname (fname);
	}
	else if (currPlist->plistCount())
	{
		currPlaying = currPlist->plistNext (-1);
	}
	else
	{
		currPlaying = -1;
	}

//	if (pthread_create(&playingThread, NULL, &(void *)playThread, currPlaying != -1 ? NULL : (void *)fname))
	if (pthread_create(&playingThread, NULL, sPlayThread, this))
	{
		cerr << "can't create thread" << endl;
	}
	playThreadRunning = 1;

	pthread_mutex_unlock (&plistMut);
	pthread_mutex_unlock (&currPlayingMut);
}

void FooAudioEngine::audioStop ()
{
	if (playThreadRunning)
	{
		clog << "audio_stop()" << endl;
		pthread_mutex_lock (&requestMut);
		stopPlaying = 1;
		pthread_mutex_unlock (&requestMut);
		playerStop ();
		clog << "pthread_join(playing_thread, NULL)" << endl;
		if (pthread_join(playingThread, NULL))
		{
			clog << "pthread_join() failed: " << strerror(errno) << endl;
		}
		playingThread = 0;
		playThreadRunning = 0;
		stopPlaying = 0;
		clog << "done stopping" << endl;
	}
	else if (state == STATE_PAUSE)
	{
		/* Paused internet stream - we are in fact stopped already. */
		if (currPlayingFname)
		{
			delete currPlayingFname;
			currPlayingFname = NULL;
		}

		state = STATE_STOP;
//		stateChange ();
	}
}

void FooAudioEngine::playerReset ()
{
	request = REQ_NOTHING;
}

void *FooAudioEngine::playThread (void *unused)
{
	clog << "entering playing thread" << endl;

	while (currPlaying != -1)
	{
		char *file;

		pthread_mutex_lock(&plistMut);
		file = currPlist->plistGetFile (currPlaying);
		pthread_mutex_unlock(&plistMut);

		playNext = 0;
		playPrev = 0;

		if (file)
		{
			int next;
			char *nextFile;

			pthread_mutex_lock(&currPlayingMut);
			pthread_mutex_lock(&plistMut);
			clog << "Playing item " << currPlaying << ' ' << file << endl;

			if (currPlayingFname)
			{
				delete currPlayingFname;
			}

			currPlayingFname = strdup (file);

			outBuf.timeSet (0.0);

			next = currPlist->plistNext (currPlaying);
			nextFile = next != -1 ? plistGetFile(next) : NULL;
			pthread_mutex_unlock(&plistMut);
			pthread_mutex_unlock(&currPlayingMut);

			player (file, nextFile);

			if (nextFile)
			{
				delete nextFile;
			}

			setInfoRate (0);
			setInfoBitrate (0);
			setInfoChannels (1);
			outBuf.timeSet (0.0);
			delete file;
		}

		pthread_mutex_lock(&currPlayingMut);

/*		if (lastStreamUrl)
		{
			delete lastStreamUrl;
			lastStreamUrl = NULL;
		}*/
		pthread_mutex_unlock(&currPlayingMut);

		if (stopPlaying)
		{
			pthread_mutex_lock (&currPlayingMut);
			currPlaying = -1;
			pthread_mutex_unlock (&currPlayingMut);
			clog << "stopped" << endl;
		}
		else
		{
			goToAnotherFile ();
		}

	}

	state = STATE_STOP;
	stateChange ();

	if (currPlayingFname)
	{
		delete currPlayingFname;
		currPlayingFname = NULL;
	}

	audioClose ();
	clog << "exiting" << endl;

	return NULL;
}

void FooAudioEngine::player(const char *file, const char *next_file)
{
	getDecoder (file);
	pthread_mutex_lock(&decoderStreamMut);
//	decoderStream = NULL;
	pthread_mutex_unlock(&decoderStreamMut);

	playFile (file, next_file);
}

void FooAudioEngine::playFile (const char *file, const char *next_file)
{
	void *decoderData;
	SoundParams soundParams = { 0, 0, 0 };
	float alreadyDecodedTime;

	outBuf.reset();

	precacheWait();

	if (precache.ok && strcmp(precache.file, file))
	{
		clog << "The precached file is not the file we want." << endl;
		precache.f->close (precache.decoderData);
		precacheReset (&precache);
	}

	if (precache.ok && !strcmp(precache.file, file))
	{
		struct decoder_error err;

		clog << "Using precached file" << endl;

		assert (f == precache.f);

		soundParams = precache.soundParams;
		decoderData = precache.decoderData;
		setInfoChannels (soundParams.channels);
		setInfoRate (soundParams.rate / 1000);

		if (!audioOpen(&soundParams))
		{
			return;
		}

		audioSendBuf (precache.buf, precache.bufFill);

		precache.f->getError (precache.decoderData, &err);

		if (err.type != ERROR_OK)
		{
			if (err.type != ERROR_STREAM || optionsGetInt("ShowStreamErrors"))
			{
				error ("%s", err.err);
			}

			decoderErrorClear (&err);
		}

		alreadyDecodedTime = precache.decodedTime;

		if (f->getAvgBitrate)
		{
			setInfoAvgBitrate (f->getAvgBitrate(decoderData));
		}
		else
		{
			setInfoAvgBitrate (0);
		}

		bitrateListInit (&bitrateList);
		bitrateList.head = precache.bitrateList.head;
		bitrateList.tail = precache.bitrateList.tail;

		/* don't free list elements when reseting precache */
		precache.bitrateList.head = NULL;
		precache.bitrateList.tail = NULL;
	}
	else
	{
		DecoderError err;

		statusMsg ("Opening...");
		decoderData = f->open(file);
		f->getError (decoderData, &err);
		if (err.type != ERROR_OK)
		{
			f->close (decoderData);
			error ("%s", err.err);
			decoderErrorClear (&err);
			clog << "Can't open file, exiting" << endl;
			return;
		}

		alreadyDecodedTime = 0.0;
		if (f->getAvgBitrate)
		{
			setInfoAvgBitrate (f->getAvgBitrate(decoderData));
		}
		bitrateListInit (&bitrateList);
	}

	audioPlistSetTime (file, f->getDuration(decoderData));
	audioStateStartedPlaying ();
	precacheReset(&precache);

	decodeLoop(f, decoderData, nextFile, soundParams, alreadyDecodedTime);
}

void FooAudioEngine::playerStop ()
{
	clog << "requesting stop" << endl;
	request = REQ_STOP;

	pthread_mutex_lock(&decoderStreamMut);

/*	if (decoderStream)
	{
		clog << "decoder_stream present, aborting..." << endl;
		ioAbort (decoderStream);
	}*/

	pthread_mutex_unlock (&decoderStreamMut);

	pthread_mutex_lock (&requestCondMutex);
	pthread_cond_signal (&requestCond);
	pthread_mutex_unlock (&requestCondMutex);
}

void FooAudioEngine::getDecoder(const char *file)
{
}
