#ifndef _FOOPLUGININTERFACES_HPP_
#define _FOOPLUGININTERFACES_HPP_

#include <QtPlugin>
#include "fooaudiostruct.hpp"

class FooAudioInterface
{
public:
	virtual ~FooAudioInterface () {}

	virtual int init (OutputDriverCaps *) = 0;
	virtual void shutdown () = 0;
	virtual int open (SoundParams *) = 0;
	virtual void close () = 0;
	virtual int play (const char *, const size_t) = 0;
	virtual int readMixer () = 0;
	virtual void setMixer (int) = 0;
	virtual int getBuffFill () = 0;
	virtual int reset () = 0;
	virtual int getRate () = 0;
	virtual void toggleMixerChannel () = 0;
	virtual char *getMixerChannelName () = 0;
};

class FooMusicFormatInterface
{
public:
	~FooMusicFormatInterface () {}

//	virtual void init () = 0;
//	virtual void destroy () = 0;
	virtual void *open (const char *) = 0;
//	virtual void *openStream (IoStream *) = 0;
//	virtual int canDecode (IoStream *) = 0;
	virtual void close (void *) = 0;
	virtual int decode (void *, char *, int , SoundParams *) = 0;
	virtual int seek (void *, int) = 0;
//	virtual void info (const char *, fileTags *, const int) = 0;
	virtual int getBitrate (void *) = 0;
	virtual int getDuration (void *) = 0;
//	virtual void getError (void *, decoderError *) = 0;
	virtual int ourFormatExt (const char *) = 0;
//	virtual int outFormatMime (const char *) = 0;
	virtual void getName (const char *, char tab [4]) = 0;
//	virtual int currentTags (void *data, FiuleTags *) = 0;
//	virtual IoStream *getStream (void *) = 0;
	virtual int getAvgBitrate (void *) = 0;

private:
	int apiVersion;
};

Q_DECLARE_INTERFACE (FooAudioInterface, "FooAudioInterface/1.0")
Q_DECLARE_INTERFACE (FooMusicFormatInterface, "FooMusicFormatInterface/1.0")

#endif
