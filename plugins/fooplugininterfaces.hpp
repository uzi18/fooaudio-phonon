#ifndef _FOOPLUGININTERFACES_HPP_
#define _FOOPLUGININTERFACES_HPP_

#include <QtPlugin>

class FooAudioInterface
{
public:
	~FooAudioInterface () {};

	virtual int init (struct output_driver_caps *caps) = 0;
	virtual void shutdown () = 0;
	virtual int open (struct sound_params *sound_params) = 0;
	virtual void close () = 0;
	virtual int play (const char *buff, const size_t size) = 0;
	virtual int readMixer () = 0;
	virtual void setMixer (int vol) = 0;
	virtual int getBuffFill () = 0;
	virtual int reset () = 0;
	virtual int getRate () = 0;
	virtual void toggleMixerChannel () = 0;
	virtual char *getMixerChannelName () = 0;
};

/*class FooMusicFormatInterface
{
public:
	~FooMusicFormatInterface () {};

	virtual void init () = 0;
	virtual void destroy () = 0;
	virtual void *open (const char *uri) = 0;
	virtual void *openStream (struct io_stream *stream) = 0;
	virtual int canDecode (struct io_stream *stream) = 0;
	virtual void close (void *data) = 0;
	virtual int decode (void *data, char *buf, int buf_len, struct sound_params *sound_params) = 0;
	virtual int seek (void *data, int sec) = 0;
	virtual void info (const char *file, struct file_tags *tags, const int tags_sel) = 0;
	virtual int getBitrate (void *data) = 0;
	virtual int getDuration (void *data) = 0;
	virtual void getError (void *data, struct decoder_error *error) = 0;
	virtual int ourFormatExt (const char *ext) = 0;
	virtual int outFormatMime (const char *mime_type) = 0;
	virtual void getName (const char *file. char buf[4]) = 0;
	virtual int currentTags (void *data, struct fiule_tags *tags) = 0;
	virtual struct io_stream *getStream (void *data) = 0;

private:
	int apiVersion;
};*/

Q_DECLARE_INTERFACE (FooAudioInterface, "FooAudioInterface/1.0")
//Q_DECLARE_INTERFACE (FooMusicFormatInterface, "FooMusicFormatInterface/1.0")

#endif

