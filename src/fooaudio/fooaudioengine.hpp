#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QDir>
#include "fooplaylistwidget.hpp"
#include "../include/fooplugininterfaces.hpp"
#include "../include/foooutbuf.hpp"
#include <pthread.h>

enum Request
{
	REQ_NOTHING,
	REQ_SEEK,
	REQ_STOP,
	REQ_PAUSE,
	REQ_UNPAUSE
};

enum State
{
	STATE_PLAY,
	STATE_STOP,
	STATE_PAUSE
};

class FooAudioEngine
{
//	Q_OBJECT

public:
	FooAudioEngine ();

	QDir getPluginsDir ();
	QStringList getPluginFileNames ();

	void audioPlay (const char *, FooPlaylistWidget *);
	void audioStop ();
	int currPlaying;
	void *playThread (void *);

	void player (const char *, const char *, FooOutBuf *);
	void playerStop ();

private:
	QDir pluginsDir;
	QStringList pluginFileNames;
	pthread_mutex_t currPlayingMut;
	pthread_mutex_t plistMut;
	pthread_mutex_t requestMut;
	FooPlaylistWidget *currPlist;
	pthread_t playingThread;
	int playThreadRunning;
	int stopPlaying;
	State state;
	char *currPlayingFname;
	int playNext;
	int playPrev;
	FooOutBuf outBuf;
	pthread_mutex_t decoderStreamMut;

	void loadPlugins ();

	QObject *alsaPlugin;
	FooMusicFormatInterface *musicFormatInterface;

	QObject *flacPlugin;
	FooAudioInterface *audioInterface;

	void getDecoder (const char *);

	Request request;

	void playerReset ();
	char *plistGetFile (int);
};

#endif // _FOOAUDIOENGINE_HPP_

