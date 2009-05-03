#include "fooaudiostruct.hpp"
#include <cstring>

#define sound_format_ok(f) (((f) & SFMT_MASK_FORMAT) && (((f) & (SFMT_S8 | SFMT_U8 | SFMT_FLOAT)) || (f) & SFMT_MASK_ENDIANES))

char *sfmt_str (const long format, char *msg, size_t buf_size)
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

int sfmt_Bps (const long format)
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
