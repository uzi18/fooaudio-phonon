#ifndef _FOOFLACPLUGIN_HPP_
#define _FOOFLACPLUGIN_HPP_

#include <QObject>
#include <FLAC/all.h>

#include "../include/fooaudiostruct.hpp"
#include "../include/fooplugininterfaces.hpp"
#include "../include/footags.hpp"

class FooFlacPlugin : public QObject, public FooMusicFormatInterface
{
	Q_OBJECT
	Q_INTERFACES(FooMusicFormatInterface)

public:
	void *open (const char *);

private:
	void *openInternal (const char *, const int);
	void close (void *);
	int decode (void *, char *, int, SoundParams *);
	int seek (void *, int);
	void info (const char *, FileTags *, const int);
	int getBitrate (void *);
	int getDuration (void *);
	void getError (void *, DecoderError *);
	int ourFormatExt (const char *);
	void getName (const char *, char tab [4]);
	int getAvgBitrate (void *);
};

#endif // _FOOFLACPLUGIN_HPP_

