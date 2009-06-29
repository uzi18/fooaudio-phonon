#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QDir>
#include "fooplaylistwidget.hpp"
#include "../include/fooplugininterfaces.hpp"
#include "../include/foooutbuf.hpp"
#include <pthread.h>

#define PCM_BUF_SIZE		(32 * 1024)

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

struct BitrateListNode
{
	BitrateListNode *next;
	int time;
	int bitrate;
};

struct BitrateList
{
	BitrateListNode *head;
	BitrateListNode *tail;
	pthread_mutex_t mutex;
};

struct Precache
{
	char *file; /* the file to precache */
	char buf[2 * PCM_BUF_SIZE]; /* PCM buffer with precached data */
	int bufFill;
	int ok; /* 1 if precache succeed */
	SoundParams soundParams; /* of the sound in the buffer */
	struct decoder *f; /* decoder functions for precached file */
	void *decoderData;
	int running; /* if the precache thread is running */
	pthread_t tid; /* tid of the precache thread */
	BitrateList bitrateList;
	int decodedTime; /* how much sound we decoded in seconds */
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

	void player (const char *, const char *);
	void playerStop ();

	void playFile (const char *file, const char *next_file);

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

	Precache precache;
};

#endif // _FOOAUDIOENGINE_HPP_

