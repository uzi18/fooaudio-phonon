#include <QApplication>
#include <QPluginLoader>
#include <iostream>

#include "fooaudioengine.hpp"

FooAudioEngine::FooAudioEngine ()
{
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

void FooAudioEngine::audioPlay (const char *fname)
{
	audioStop ();
	playerReset ();

	pthread_mutex_lock (currPlayingMut);
	pthread_mutex_lock (plistMut);

	currPlist = &playlist;

	if (*fname)
	{
		currPlaying = plistFindFname (currPlist, fname);
	}
	else if (plistCount(currPlist))
	{
		currPlaying = plistNext (currPlist, -1);
	}
	else
	{
		currPlaying = -1;
	}

	if (pthread_create(&playingThread, NULL, playThread, currPlaying != -1 ? NULL : (void *)fname))
	{
		cerr << "can't create thread" << endl;
	}
	playThreadRunning = 1;

	pthread_mutex_unlock (plistMut);
	pthread_mutex_unlock (currPlayingMut);
}

void FooAudioEngine::audioStop ()
{
	if (playThreadRunning)
	{
		clog << "audio_stop()" << endl;
		pthread_mutex_lock (request_mut);
		stopPlaying = 1;
		pthread_mutex_unlock (request_mut);
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
		stateChange ();
	}
}
