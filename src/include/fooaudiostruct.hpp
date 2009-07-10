#ifndef _FOOAUDIOSTRUCT_HPP_
#define _FOOAUDIOSTRUCT_HPP_

#include <cassert>
#include <alsa/asoundlib.h>
#include <cstring>

enum sfmtFmt
{
	SFMT_S8 = 0x00000001,
	SFMT_U8 = 0x00000002,
	SFMT_S16 = 0x00000004,
	SFMT_U16 = 0x00000008,
	SFMT_S32 = 0x00000010,
	SFMT_U32 = 0x00000020,
	SFMT_FLOAT = 0x00000040,
};

enum sfmtEndianes
{
	SFMT_LE = 0x00001000,
	SFMT_BE = 0x00002000,

#ifdef WORDS_BIGENDIAN
	SFMT_NE = SFMT_BE
#else
	SFMT_NE = SFMT_LE
#endif
};

struct Params
{
	unsigned channels;
	unsigned rate;
	snd_pcm_format_t format;
};

struct SoundParams
{
	int channels;
	int rate;
	long fmt;
};

struct OutputDriverCaps
{
	int minChannels;
	int maxChannels;
	long formats;
};

const long SFMT_MASK_FORMAT = 0x00000fff;
const long SFMT_MASK_ENDIANES = 0x00003000;

static SoundParams driverSoundParams = {0, 0, 0};

char *sfmtStr (const long, char *, size_t);
int sfmtBps (const long);
int audioGetBps ();
inline bool soundFormatOk (const long);
//int audioGetBps ();

#endif // _FOOAUDIOSTRUCT_HPP_

