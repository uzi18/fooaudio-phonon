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
	int getType();

private:
	void getVorbiscomments (const char *, FileTags *);
	void fillTag (FLAC__StreamMetadata_VorbisComment_Entry *, FileTags *);
};

//private:
	size_t packPcmSigned (FLAC__byte *, const FLAC__int32 * const input[], unsigned int, unsigned int, unsigned int);
	FLAC__StreamDecoderWriteStatus writeCallback (const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const buffer[], void *);
	FLAC__StreamDecoderReadStatus readCallback (const FLAC__StreamDecoder *, FLAC__byte buffer[], size_t *, void *);
	FLAC__StreamDecoderSeekStatus seekCallback (const FLAC__StreamDecoder *, FLAC__uint64, void *);
	FLAC__StreamDecoderTellStatus tellCallback (const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
	FLAC__StreamDecoderLengthStatus lengthCallback (const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
	FLAC__bool eofCallback (const FLAC__StreamDecoder *, void *);
	void metadataCallback (const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
	void errorCallback (const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);
//};

#endif // _FOOFLACPLUGIN_HPP_

