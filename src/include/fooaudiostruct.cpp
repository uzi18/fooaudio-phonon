#include "fooaudiostruct.hpp"
#include <cstring>

inline bool soundFormatOk(const long f)
{
	return (((f) & SFMT_MASK_FORMAT) && (((f) & (SFMT_S8 | SFMT_U8 | SFMT_FLOAT)) || (f) & SFMT_MASK_ENDIANES))
}

char *sfmtStr (const long format, char *msg, size_t buf_size)
{
	assert (sound_format_ok(format));

	assert (buf_size > 0);

	msg[0] = 0;

	if (format & SFMT_S8)
		strncat (msg, ", 8-bit signed", buf_size - strlen(msg) - 1);
	if (format & SFMT_U8)
		strncat (msg, ", 8-bit unsigned", buf_size - strlen(msg) - 1);
	if (format & SFMT_S16)
		strncat (msg, ", 16-bit signed", buf_size - strlen(msg) - 1);
	if (format & SFMT_U16)
		strncat (msg, ", 16-bit unsigned", buf_size - strlen(msg) - 1);
	if (format & SFMT_S32)
		strncat (msg, ", 24-bit signed (as 32-bit samples)", buf_size - strlen(msg) - 1);
	if (format & SFMT_U32)
		strncat (msg, ", 24-bit unsigned (as 32-bit samples)", buf_size - strlen(msg) - 1);
	if (format & SFMT_FLOAT)
		strncat (msg, ", float", buf_size - strlen(msg) - 1);

	if (format & SFMT_LE)
		strncat (msg, " little-endian", buf_size - strlen(msg) - 1);
	else if (format & SFMT_BE)
		strncat (msg, " big-endian", buf_size - strlen(msg) - 1);
	if (format & SFMT_NE)
		strncat (msg, " (native)", buf_size - strlen(msg) - 1);

	/* skip first ", " */
	if (msg[0])
		memmove (msg, msg + 2, strlen(msg) + 1);

	return msg;
}

/*inline bool sound_format_ok (const long f)
{
	return (((f) & SFMT_MASK_FORMAT) && (((f) & (SFMT_S8 | SFMT_U8 | SFMT_FLOAT)) || (f) & SFMT_MASK_ENDIANES));
}*/

int sfmtBps (const long format)
{
	int Bps = -1;

	switch (format & SFMT_MASK_FORMAT)
	{
		case SFMT_S8:
		case SFMT_U8:
			Bps = 1;
			break;

		case SFMT_S16:
		case SFMT_U16:
			Bps = 2;
			break;

		case SFMT_S32:
		case SFMT_U32:
			Bps = 4;

		case SFMT_FLOAT:
			Bps = sizeof (float);
			break;
	}

	assert (Bps > 0);

	return Bps;
}

int audioGetBps ()
{
	return driverSoundParams.rate * driverSoundParams.channels
		* (driverSoundParams.fmt ? sfmt_Bps(driverSoundParams.fmt)
				: 0);
}

long sfmtBestMatching (const long formats_with_endian, const long req_with_endian)
{
	long formats = formats_with_endian & SFMT_MASK_FORMAT;
	long req = req_with_endian & SFMT_MASK_FORMAT;
	long best = 0;

	if (formats & req)
		best = req;
	else if (req == SFMT_S8 || req == SFMT_U8) {
		if (formats & SFMT_S8)
			best = SFMT_S8;
		else if (formats & SFMT_U8)
			best = SFMT_U8;
		else if (formats & SFMT_S16)
			best = SFMT_S16;
		else if (formats & SFMT_U16)
			best = SFMT_U16;
		else if (formats & SFMT_S32)
			best = SFMT_S32;
		else if (formats & SFMT_U32)
			best = SFMT_U32;
		else if (formats & SFMT_FLOAT)
			best = SFMT_FLOAT;
	}
	else if (req == SFMT_S16 || req == SFMT_U16) {
		if (formats & SFMT_S16)
			best = SFMT_S16;
		else if (formats & SFMT_U16)
			best = SFMT_U16;
		else if (formats & SFMT_S32)
			best = SFMT_S32;
		else if (formats & SFMT_U32)
			best = SFMT_U32;
		else if (formats & SFMT_FLOAT)
			best = SFMT_FLOAT;
		else if (formats & SFMT_S8)
			best = SFMT_S8;
		else if (formats & SFMT_U8)
			best = SFMT_U8;
	}
	else if (req == SFMT_S32 || req == SFMT_U32 || req == SFMT_FLOAT) {
		if (formats & SFMT_S32)
			best = SFMT_S32;
		else if (formats & SFMT_U32)
			best = SFMT_U32;
		else if (formats & SFMT_S16)
			best = SFMT_S16;
		else if (formats & SFMT_U16)
			best = SFMT_U16;
		else if (formats & SFMT_FLOAT)
			best = SFMT_FLOAT;
		else if (formats & SFMT_S8)
			best = SFMT_S8;
		else if (formats & SFMT_U8)
			best = SFMT_U8;
	}

	assert (best != 0);

	if (!(best & (SFMT_S8 | SFMT_U8))) {
		if ((formats_with_endian & SFMT_LE)
				&& (formats_with_endian & SFMT_BE))
			best |= SFMT_NE;
		else
			best |= formats_with_endian & SFMT_MASK_ENDIANES;
	}

	cout << "Choosed " << sfmtStr(best, fmt_name1, sizeof(fmt_name1)) << " as the best matching " << sfmtStr(req_with_endian, fmt_name2, sizeof(fmt_name2)) << endl;

	return best;
}
