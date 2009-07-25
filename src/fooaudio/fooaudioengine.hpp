#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QList>
#include <QDir>
#include "fooplaylistwidget.hpp"
#include "../include/fooplugininterfaces.hpp"
#include "../include/foooutbuf.hpp"
#include "../include/foodecoder.hpp"
#include "../include/fooaudiostruct.hpp"
#include "../include/footags.hpp"
#include <pthread.h>

#define PCM_BUF_SIZE		(32 * 1024)
#define PREBUFFER_THRESHOLD	(16 * 1024)

#define soundParamsEq(p1, p2) ((p1).fmt == (p2).fmt && (p1).channels == (p2).channels && (p1).rate == (p2).rate)

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
	int time;
	int bitrate;
};

class Precache
{
public:
	void wait();
	int isOk();
	FooMusicFormatInterface * getDecoder();
	SoundParams getSoundParams();
	char * getBuf();
	int getBufFill();
	QList<BitrateListNode> getBitrateList();
	int getDecodedTime();
	void *getDecoderData();
	void reset();
	char *getFile();
	void startPrecache(const char *file);

private:
	char *file; /* the file to precache */
	char buf[2 * PCM_BUF_SIZE]; /* PCM buffer with precached data */
	int bufFill;
	int ok; /* 1 if precache succeed */
	SoundParams soundParams; /* of the sound in the buffer */
	FooMusicFormatInterface *decoder; /* decoder functions for precached file */
	void *decoderData;
	int running; /* if the precache thread is running */
	pthread_t tid; /* tid of the precache thread */
	QList <BitrateListNode> bitrateList;
	int decodedTime; /* how much sound we decoded in seconds */
};

struct SoundInfo
{
	int avgBitrate;
	int bitrate;
	int rate;
	int channels;
};

class FooAudioEngine
{
//	Q_OBJECT

public:
	FooAudioEngine ();

	QDir getPluginsDir ();
	QStringList getPluginFileNames ();

	void play (const char *, FooPlaylistWidget *);
	void stop ();
	int currPlaying;
	void *playThread (void *);

	void player (const char *, const char *);
	void playerStop ();

	void playFile (const char *, const char *);
	int open (SoundParams *);

	void resetSoundParams(SoundParams *);
	int sendBuf (const char *, const size_t);

	void plistSetTime (const char *, const int);

// sygnały?
	void setInfoAvgBitrate (const int);

	void decodeLoop (void *, const char *, SoundParams, const float);

	void stateStartedPlaying();

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
	pthread_mutex_t requestCondMutex;
	SoundParams reqSoundParams;

	void loadPlugins();

	QObject *alsaPlugin;
	FooMusicFormatInterface *musicFormatInterface;

	QObject *flacPlugin;
	FooAudioInterface *audioInterface;

	FooMusicFormatInterface * getDecoder(const char *);

	Request request;

	void playerReset();
	char *plistGetFile(int);

	void setInfoRate(const int);
	void setInfoBitrate(const int);
	void setInfoChannels(const int);
	void stateChange();
	void goToAnotherFile();
	void close();

	Precache precache;
	SoundInfo soundInfo;

	int opened;
	OutputDriverCaps hwCaps;

	QList <BitrateListNode> bitrateList;
	pthread_mutex_t bitrateListMutex;

	pthread_cond_t requestCond;
};

#endif // _FOOAUDIOENGINE_HPP_

