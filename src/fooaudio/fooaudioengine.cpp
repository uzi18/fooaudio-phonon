#include <QApplication>
#include <QPluginLoader>
#include <iostream>
#include <errno.h>
using namespace std;
#include <pthread.h>

#include "../include/fooio.hpp"
#include "../include/foodecoder.hpp"
#include "fooaudioengine.hpp"

const int SFMT_STR_MAX = 265;

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

	soundInfo.avgBitrate = -1;
	soundInfo.bitrate = -1;
	soundInfo.channels = -1;
	soundInfo.rate = -1;

	audioOpened = 0;

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
			setInfoChannels (1);	void close();
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

	precache.wait();

	if (precache.isOk() && strcmp(precache.getFile(), file))
	{
		clog << "The precached file is not the file we want." << endl;
#warning "poprawić dekoder!"
//		precache.f->close (precache.decoderData);
		precache.reset();
	}

	if (precache.isOk() && !strcmp(precache.getFile(), file))
	{
		DecoderError err;

		clog << "Using precached file" << endl;

		assert (musicFormatInterface == precache.getDecoder());

		soundParams = precache.getSoundParams();
		decoderData = precache.getDecoderData();
		setInfoChannels (soundParams.channels);
		setInfoRate (soundParams.rate / 1000);

		if (!audioOpen(&soundParams))
		{
			return;
		}

		audioSendBuf (precache.getBuf(), precache.getBufFill());
#warning "poprawić dekoder, getError(), decoderData is private";
//		precache.getDecoder()->getError (precache.decoderData, &err);

		if (err.getType() != ERROR_OK)
		{
			if (err.getType() != ERROR_STREAM || 1 /*optionsGetInt("ShowStreamErrors")*/)
			{
				cerr << err.err << endl;
			}

			err.clear();
		}

		alreadyDecodedTime = precache.getDecodedTime();

		if (musicFormatInterface->getAvgBitrate(0))
		{
			setInfoAvgBitrate (musicFormatInterface->getAvgBitrate(decoderData));
		}
		else
		{
			setInfoAvgBitrate (0);
		}

		bitrateListInit (&bitrateList);
		bitrateList.head = precache.getBitrateList().head;
		bitrateList.tail = precache.getBitrateList().tail;

		/* don't free list elements when reseting precache */
		precache.getBitrateList().head = NULL;
		precache.getBitrateList().tail = NULL;
	}
	else
	{
		DecoderError err;

		statusMsg("Opening...");
		decoderData = musicFormatInterface->open(file);
		musicFormatInterface->getError (decoderData, &err);
		if (err.getType() != ERROR_OK)
		{
			musicFormatInterface->close(decoderData);
			error ("%s", err.err);
			decoderErrorClear (&err);
			clog << "Can't open file, exiting" << endl;
			return;
		}

		alreadyDecodedTime = 0.0;
		if (musicFormatInterface->getAvgBitrate(0))
		{
			setInfoAvgBitrate (musicFormatInterface->getAvgBitrate(decoderData));
		}
		bitrateListInit (&bitrateList);
	}

	audioPlistSetTime (file, musicFormatInterface->getDuration(decoderData));
	audioStateStartedPlaying ();
	precache.reset();

	decodeLoop(musicFormatInterface, decoderData, next_file, soundParams, alreadyDecodedTime);
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

FooMusicFormatInterface * FooAudioEngine::getDecoder(const char *file)
{
}

void FooAudioEngine::setInfoRate (const int rate)
{
	soundInfo.rate = rate;
//	addEventAll (EV_RATE, NULL); <-- poprawić na odwołania do GUI
}

void FooAudioEngine::setInfoBitrate (const int bitrate)
{
	soundInfo.bitrate = bitrate;
//	addEventAll (EV_BITRATE, NULL);
}

void FooAudioEngine::setInfoChannels(const int channels)
{
	soundInfo.channels = channels;
//	addEventAll (EV_CHANNELS, NULL);
}

void FooAudioEngine::stateChange()
{
//	addEventAll (EV_STATE, NULL);
}

// przejście do następnego pliku
void FooAudioEngine::goToAnotherFile()
{
}

void FooAudioEngine::audioClose()
{
	if (audioOpened)
	{
		resetSoundParams (&reqSoundParams);
		resetSoundParams (&driverSoundParams);
		musicFormatInterface->close(0);
#warning "need_audio_conversion - sprawdzić i poprawić"
/*		if (need_audio_conversion) {
			audio_conv_destroy (&sound_conv);
			need_audio_conversion = 0;
		}*/
		audioOpened = 0;
	}
}

void Precache::wait()
{
	if (running)
	{
		clog << "Waiting for precache thread..." << endl;
		if (pthread_join(tid, NULL))
		{
			cerr << "pthread_join() for precache thread failed" << endl;
		}
		running = 0;
		cout << "done" << endl; // debug
	}
	else
	{
		cout << "Precache thread is not running" << endl;
	}
}

int Precache::isOk()
{
	return ok;
}

char * Precache::getFile()
{
	return file;
}

FooMusicFormatInterface * Precache::getDecoder()
{
	return decoder;
}

SoundParams Precache::getSoundParams()
{
	return soundParams;
}

char * Precache::getBuf()
{
	return buf;
}

int Precache::getBufFill()
{
	return bufFill;
}

BitrateList Precache::getBitrateList()
{
	return bitrateList;
}

void Precache::reset()
{
	assert (!running);
	ok = 0;
	if (file) {
		delete[] file;
		file = NULL;
		bitrateListDestroy (&bitrateList);
	}
}

int Precache::getDecodedTime()
{
	return decodedTime;
}

void *Precache::getDecoderData()
{
}

/* Return 0 on error. If sound params == NULL, open the device with the last
 * parameters. */
int FooAudioEngine::audioOpen (SoundParams *sound_params)
{
	int res;
	static SoundParams last_params = { 0, 0, 0 };

	if (!sound_params)
	{
		sound_params = &last_params;
	}
	else
	{
		last_params = *sound_params;
	}

	assert (soundFormatOk(sound_params->fmt));

	if (audioOpened && soundParamsEq(reqSoundParams, *sound_params))
	{
		if (audioGetBps() < 88200)
		{
			clog << "Reopening device due to low bps." << endl;

			/* Not closing the device would cause that much
			 * sound from the previuous file stays in the buffer
			 * and the user will see old data, so close it. */
			audioClose ();
		}
		else
		{
			clog << "Audio device already opened with such parameters." << endl;

			return 1;
		}
	}
	else if (audioOpened)
	{
		audioClose ();
	}

	reqSoundParams = *sound_params;

	/* Set driver_sound_params to parameters supported by the driver that
	 * are nearly the requested parameters */

	if (false /*optionsGetInt("ForceSampleRate")*/)
	{
		driverSoundParams.rate = true; // optionsGetInt("ForceSampleRate");
		clog << "Setting forced driver sample rate to " << driverSoundParams.rate << "Hz" << endl;
	}
	else
	{
		driverSoundParams.rate = reqSoundParams.rate;
	}

	driverSoundParams.fmt = sfmtBestMatching (hwCaps.formats, reqSoundParams.fmt);

	/* number of channels */
	if (reqSoundParams.channels > hwCaps.maxChannels)
	{
		driverSoundParams.channels = hwCaps.maxChannels;
	}
	else if (reqSoundParams.channels < hwCaps.minChannels)
	{
		driverSoundParams.channels = hwCaps.minChannels;
	}
	else
	{
		driverSoundParams.channels = reqSoundParams.channels;
	}

	res = audioInterface->open (&driverSoundParams);
	driverSoundParams.rate = audioInterface->getRate();

	if (res)
	{
		char fmtName[SFMT_STR_MAX];

		if (driverSoundParams.fmt != reqSoundParams.fmt || driverSoundParams.channels != reqSoundParams.channels || (!sampleRateCompat(reqSoundParams.rate, driverSoundParams.rate)))
		{
			clog << "Conversion of the sound is needed." << endl;
			if (!audioConvNew (&soundConv, &reqSoundParams, &driverSoundParams))
			{
				audioInterface->close();
				resetSoundParams (&reqSoundParams);
				return 0;
			}
#warning "needAudioConversion = 1; ???"
//			needAudioConversion = 1;
		}
		audioOpened = 1;

		clog << "Requested sound parameters: " << sfmtStr(reqSoundParams.fmt, fmtName, sizeof(fmtName)) << ", " << reqSoundParams.channels << " channels, " << reqSoundParams.rate << "Hz" << endl;
		clog << "Driver sound parameters: " << sfmtStr(driverSoundParams.fmt, fmtName, sizeof(fmtName)) << ", " << driverSoundParams.channels <<  "channels, " << driverSoundParams.rate << "Hz" << endl;
	}

	return res;
}

void FooAudioEngine::resetSoundParams(SoundParams *params)
{
	params->rate = 0;
	params->channels = 0;
	params->fmt = 0;
}

int FooAudioEngine::audioSendBuf (const char *buf, const size_t size)
{
	size_t out_data_len = size;
	int res;
	char *converted = NULL;

#warning "Konwersja!"
	if (false /*need_audio_conversion*/)
	{
//		converted = audio_conv (&sound_conv, buf, size, &out_data_len);
	}

	if (false /*need_audio_conversion && converted*/)
	{
		//res = out_buf_put (&out_buf, converted,	out_data_len);
	}
	else if (true /*!need_audio_conversion*/)
	{
		res = outBuf.put(buf, size);
	}
	else
	{
		res = 0;
	}

//	if (converted)
//		free (converted);

	return res;
}

void FooAudioEngine::setInfoAvgBitrate (const int avg_bitrate)
{
	// sygnał do ustawiania bitrate w odtwarzaczu
}

void FooAudioEngine::audioPlistSetTime (const char *file, const int time)
{
	int i;

	pthread_mutex_lock (&plistMut);
	if (/*(i = currPlist->findFname(file)) != -1*/ false)
	{
#warning "currPlist czy playlist? i  ogólnie poprawić"
//		currPlist->setItemTime (i, time);
//		currPlist.items[i].mtime = getMtime (file);
		cout << "Setting time for " << file << endl;
	}
	else
	{
		clog << "Request for updating time for a file not present on the playlist!" << endl;
	}
	pthread_mutex_unlock(&plistMut);
}

void FooAudioEngine::decodeLoop(void *decoder_data, const char *next_file, OutBuf *out_buf, SoundParams sound_params, const float already_decoded_sec)
{
	int eof = 0;
	char buf[PCM_BUF_SIZE];
	int decoded = 0;
	SoundParams new_sound_params;
	int sound_params_change = 0;
	float decode_time = already_decoded_sec; /* the position of the decoder
							 (in seconds) */

	outBufSetFreeCallback (out_buf, bufFreeCallback);

	pthread_mutex_lock (&currTagsMut);
	currTags = tagsNew ();
	pthread_mutex_unlock (&currTagsMut);
/*
	if (musicFormatInterface->getStream())
	{
		pthread_mutex_lock (&decoderStreamMut);
		decoderStream = musicFormatInterface->getStream (decoderData);
		pthread_mutex_lock (&decoderStreamMut);
	}
	else
	*/
	{
		clog << "No get_stream() function" << endl;
	}
#warning "status_msg()"
//	status_msg ("Playing...");

	while (1)
	{
		cout << "loop..." << endl;

		pthread_mutex_lock  (&requestCondMutex);
		if (!eof && !decoded)
		{
			DecoderError err;

			pthread_mutex_unlock(&requestCondMutex);

			if (decoderStream && outBufGetFill(out_buf) < PREBUFFER_THRESHOLD)
			{
				prebuffering = 1;
				ioPrebuffer (decoderStream, optionsGetInt("Prebuffering") * 1024);
				prebuffering = 0;
#warning "status_msg()"
//				status_msg ("Playing...");
			}

			decoded = musicFormatInterface->decode(decoder_data, buf, sizeof(buf), &new_sound_params);

			if (decoded)
			{
				decode_time += decoded / (float)(sfmtBps(new_sound_params.fmt) * new_sound_params.rate * new_sound_params.channels);
			}

			musicFormatInterface->getError (decoderData, &err);
			if (err.type != ERROR_OK)
			{
				if (err.type != ERROR_STREAM || optionsGetnt("ShowStreamErrors"))
				{
					error ("%s", err.err);
				}
				decoderErrorClear (&err);
			}

			if (!decoded)
			{
				eof = 1;
				clog << "EOF from decoder" << endl;
			}
			else
			{
				clog << "decoded " << decoded << " bytes" << endl;
				if (!soundParamsEq(newSoundParams, soundParams))
				{
					soundParamsChange = 1;
				}

				bitrateListAdd (&bitrateList, decodeTime, musicFormatInterface->getBitrate(decoderData));
				updateTags (musicFormatInterface, decoder_data, decoder_stream);
			}
		}

		/* Wait, if there is no space in the buffer to put the decoded
		 * data or EOF occured and there is something in the buffer. */
		else if (decoded > out_buf->getFree() || (eof && out_buf->getFill()))
		{
			cout << "waiting..." << endl;
			if (eof && !precache.file && next_file && fileType(next_file) == F_SOUND && optionsGetInt("Precache"))
			{
				precache->startPrecache (next_file);
			}
			pthread_cond_wait (&requestCond, &requestCondMutex);
			pthread_mutex_unlock (&request_cond_mutex);
		}
		else
		{
			pthread_mutex_unlock (&request_cond_mutex);
		}

		/* When clearing request, we must make sure, that another
		 * request will not arrive at the moment, so we check if
		 * the request has changed. */
		if (request == REQ_STOP) {
			logit ("stop");
			out_buf_stop (out_buf);

			LOCK (request_cond_mutex);
			if (request == REQ_STOP)
				request = REQ_NOTHING;
			UNLOCK (request_cond_mutex);

			break;
		}
		else if (request == REQ_SEEK) {
			int decoder_seek;

			logit ("seeking");
			if ((decoder_seek = f->seek(decoder_data, req_seek))
					== -1)
				logit ("error when seeking");
			else {
				out_buf_stop (out_buf);
				out_buf_reset (out_buf);
				out_buf_time_set (out_buf, decoder_seek);
				bitrate_list_empty (&bitrate_list);
				decode_time = decoder_seek;
				eof = 0;
				decoded = 0;
			}

			LOCK (request_cond_mutex);
			if (request == REQ_SEEK)
				request = REQ_NOTHING;
			UNLOCK (request_cond_mutex);

		}
		else if (!eof && decoded <= out_buf_get_free(out_buf)
				&& !sound_params_change) {
			debug ("putting into the buffer %d bytes", decoded);
			audio_send_buf (buf, decoded);
			decoded = 0;
		}
		else if (!eof && sound_params_change
				&& out_buf_get_fill(out_buf) == 0) {
			logit ("sound parameters has changed.");
			sound_params = new_sound_params;
			sound_params_change = 0;
			set_info_channels (sound_params.channels);
			set_info_rate (sound_params.rate / 1000);
			out_buf_wait (out_buf);
			if (!audio_open(&sound_params))
				break;
		}
		else if (eof && out_buf_get_fill(out_buf) == 0) {
			logit ("played everything");
			break;
		}
	}

	status_msg ("");

	LOCK (decoder_stream_mut);
	decoder_stream = NULL;
	f->close (decoder_data);
	UNLOCK (decoder_stream_mut);

	bitrate_list_destroy (&bitrate_list);

	LOCK (curr_tags_mut);
	if (curr_tags) {
		tags_free (curr_tags);
		curr_tags = NULL;
	}
	UNLOCK (curr_tags_mut);

	out_buf_wait (out_buf);
}
