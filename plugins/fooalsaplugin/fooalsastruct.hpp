#ifndef _FOOALSASTRUCT_HPP_
#define _FOOALSASTRUCT_HPP_

enum sfmt_fmt
{
	SFMT_S8 = 0x00000001,
	SFMT_U8 = 0x00000002,
	SFMT_S16 = 0x00000004,
	SFMT_U16 = 0x00000008,
	SFMT_S32 = 0x00000010,
	SFMT_U32 = 0x00000020,
	SFMT_FLOAT = 0x00000040,
};

enum sfmt_endianes
{
	SFMT_LE = 0x00001000,
	SFMT_BE = 0x00002000,

#ifdef WORDS_BIGENDIAN
	SFMT_NE = SFMT_BE
#else
	SFMT_NE = SFMT_LE
#endif
}

struct sound_params
{
	int channels;
	int rate;
	long fmt;
};

struct outputDriverCaps
{
	int min_channels;
	int max_channels;
	long formats;
}

const long SFMT_MASK_FORMAT = 0x00000fff;
const long SFMT_MASK_ENDIANES = 0x00003000

char *sfmt_str (const long format, char *msg, const size_t buf_size);
inline bool sound_format_ok (const long f);

