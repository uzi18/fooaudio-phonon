#include "foofifobuf.hpp"

#include <cassert>
#include <cstring>

#include "foofifobuf.h"

/* Initialize the fifo_buf structure with empty buffer of the given size. */
void FifoBufInit (FifoBuf *b, const size_t size)
{
	assert (b != NULL);
	assert (size > 0);

	b->buf = (char *)malloc (size);
	b->size = size;
	b->pos = 0;
	b->fill = 0;
}

/* Destroy the buffer object. */
void FifoBufDestroy (FifoBuf *b)
{
	assert (b != NULL);

	free (b->buf);
	b->buf = NULL;
}

/* Put data into the buffer. Returns number of bytes actually put. */
size_t FifoBufPut (struct fifo_buf *b, const char *data, size_t size)
{
	size_t written = 0;

	assert (b != NULL);
	assert (b->buf != NULL);

	while (b->fill < b->size && written < size)
	{
		size_t write_from;
		size_t to_write;

		if (b->pos + b->fill < b->size)
		{
			write_from = b->pos + b->fill;
			to_write = b->size - (b->pos + b->fill);
		}
		else
		{
			write_from = b->fill - b->size + b->pos;
			to_write = b->size - b->fill;
		}

		if (to_write > size - written)
		{
			to_write = size - written;
		}

		memcpy (b->buf + write_from, data + written, to_write);
		b->fill += to_write;
		written += to_write;
	}

	return written;
}

/* Copy data from the beggining of the buffer to the user buffer. Returns the
 * number of bytes copied. */
size_t FifoBufPeek (FifoBuf *b, char *user_buf, size_t user_buf_size)
{
	size_t user_buf_pos = 0, written = 0;
	ssize_t left = b->fill;
	ssize_t pos = b->pos;

	assert (b != NULL);
	assert (b->buf != NULL);

	while (left && written < user_buf_size)
	{
		size_t to_copy = pos + left <= b->size ? left : b->size - pos;

		if (to_copy > user_buf_size - written)
		{
			to_copy = user_buf_size - written;
		}

		memcpy (user_buf + user_buf_pos, b->buf + pos, to_copy);
		user_buf_pos += to_copy;
		written += to_copy;

		left -= to_copy;
		pos += to_copy;
		if (pos == b->size)
		{
			pos = 0;
		}
	}

	 return written;
}

size_t FifoBufGet (FifoBuf *b, char *user_buf, size_t user_buf_size)
{
	size_t user_buf_pos = 0, written = 0;

	assert (b != NULL);
	assert (b->buf != NULL);

	while (b->fill && written < user_buf_size)
	{
		size_t to_copy = b->pos + b->fill <= b->size ? b->fill : b->size - b->pos;

		if (to_copy > user_buf_size - written)
		{
			to_copy = user_buf_size - written;
		}

		memcpy (user_buf + user_buf_pos, b->buf + b->pos, to_copy);
		user_buf_pos += to_copy;
		written += to_copy;

		b->fill -= to_copy;
		b->pos += to_copy;
		if (b->pos == b->size)
		{
			 b->pos = 0;
		}
	}

	return written;
}

/* Get the amount of free spaace in the buffer. */
size_t FifoBufGetSpace (const FifoBuf *b)
{
	assert (b != NULL);
	assert (b->buf != NULL);

	return b->size - b->fill;
}

size_t FifoBufGetFill (const FifoBuf *b)
{
	assert (b != NULL);

	return b->fill;
}

size_t FifoBufGetSize (const FifoBuf *b)
{
	assert (b != NULL);

	return b->size;
}

void FifoBufClear (FifoBuf *b)
{
	assert (b != NULL);
	b->fill = 0;
}

