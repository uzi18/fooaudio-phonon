#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

#include "fooio.hpp"
#include <sys/stat.h>

char *xstrdup (const char *s)
{
	char *n;

	if (s && (n = strdup(s)) == NULL)
		cerr << "Can't allocate memory!";

	return s ? n : NULL;
}

IoStream *ioOpen (const char *file, const int buffered)
{
	IoStream *s;

	assert (file != NULL);

	s = new IoStream ();

	s->errnoVal = 0;
	s->readError = 0;
	s->strerror = NULL;
	s->opened = 0;
	s->bufFillCallback = NULL;

	memset (&s->metadata, 0, sizeof(s->metadata));

	ioOpenFile (s, file);

	pthread_mutex_init (&s->bufMutex, NULL);
	pthread_mutex_init (&s->ioMutex, NULL);
	pthread_mutex_init (&s->metadata.mutex, NULL);

	if (!s->opened)
	{
		return s;
	}

	s->stopReadThread = 0;
	s->eof = 0;
	s->afterSeek = 0;
	s->buffered = buffered;
	s->pos = 0;

	if (buffered)
	{
		FifoBufInit (&s->buf, 1 /*optionsGetInt("InputBuffer")*/ * 1024);
		s->prebuffer = /*optionsGetInt("Prebuffering")*/ 1 * 1024;

		pthread_cond_init (&s->bufFreeCond, NULL);
		pthread_cond_init (&s->bufFillCond, NULL);

		if (pthread_create(&s->readThread, NULL, ioReadThread, s))
		{
			cerr << "Can't create read thread: " << strerror (errno) << endl;
		}
	}

	return s;
}

void ioOpenFile (IoStream *s, const char *file)
{
	struct stat file_stat;

	if ((s->fd = open(file, O_RDONLY)) == -1)
	{
		s->errnoVal = errno;
	}
	else if (fstat(s->fd, &file_stat) == -1)
	{
		s->errnoVal = errno;
	}
	else
	{
		s->size = file_stat.st_size;
		s->source = IO_SOURCE_FD;
		s->opened = 1;
	}
}

int ioOkNolock (IoStream *s)
{
	return !s->readError && s->errnoVal == 0;
}

int ioOk (IoStream *s)
{
	int res;
	pthread_mutex_lock (&s->bufMutex);
	res = ioOkNolock (s);
	pthread_mutex_unlock (&s->bufMutex);

	return res;
}

void *ioReadThread (void *data)
{
	IoStream *s = (IoStream *)data;

	clog << "IO read thread created" << endl;

	while (!s->stopReadThread)
	{
		char read_buf[8096];
		int read_buf_fill = 0;
		int read_buf_pos = 0;

		pthread_mutex_lock (&s->ioMutex);
		cout << "Reading..." << endl;

		pthread_mutex_lock (&s->bufMutex);
		s->afterSeek = 0;
		pthread_mutex_unlock (&s->bufMutex);

		read_buf_fill = ioInternalRead (s, 0, read_buf, sizeof(read_buf));

		pthread_mutex_unlock (&s->ioMutex);

		cout << "Read " << (int)read_buf_fill << " bytes" << endl;

		pthread_mutex_lock (&s->bufMutex);

		if (s->stopReadThread)
		{
			pthread_mutex_unlock (&s->bufMutex);
			break;
		}

		if (read_buf_fill < 0)
		{
			s->errnoVal = errno;
			s->readError = 1;

			clog << "Exiting due tu read error." << endl;

			pthread_cond_broadcast (&s->bufFillCond);
			pthread_mutex_unlock (&s->bufMutex);
			break;
		}

		if (read_buf_fill == 0)
		{
			s->eof = 1;
			cout << "EOF, waiting" << endl;
			pthread_cond_broadcast (&s->bufFillCond);
			pthread_cond_wait (&s->bufFreeCond, &s->bufMutex);
			cout << "Got signal" << endl;
			pthread_mutex_unlock (&s->bufMutex);

			continue;
		}

		s->eof = 0;

		while (read_buf_pos < read_buf_fill && !s->afterSeek)
		{
			int put;

			cout << "Buffer fill: " << (unsigned long)FifoBufGetFill(&s->buf) << endl;
			put = FifoBufPut (&s->buf, read_buf + read_buf_pos, read_buf_fill - read_buf_pos);

			if (s->stopReadThread)
			{
				break;
			}

			if (put > 0)
			{
				cout << "Put " << put << " bytes into the buffer" << endl;

				if (s->bufFillCallback)
				{
					pthread_mutex_unlock (&s->bufMutex);

					s->bufFillCallback (s, FifoBufGetFill(&s->buf), FifoBufGetSize(&s->buf), s->bufFillCallbackData);

					pthread_mutex_lock (&s->bufMutex);
				}

				pthread_cond_broadcast (&s->bufFillCond);
				read_buf_pos += put;
			}
			else
			{
				cout << "The buffer is full, waiting." << endl;
				pthread_cond_wait (&s->bufFreeCond, &s->bufMutex);
				cout << "Some space in the buffer was freed" << endl;
			}
		}

		pthread_mutex_lock (&s->bufMutex);
	}

	if (s->stopReadThread)
	{
		clog << "Stop request" << endl;
	}

	clog << "Exiting IO read thread" << endl;

	return NULL;
}

void ioClose (IoStream *s)
{
	assert (s != NULL);

	clog << "Closing stream..." << endl;

	if (s->opened)
	{
		if (s->buffered)
		{
			ioAbort (s);

			clog << "Waiting for io_read_thread()..." << endl;
			pthread_join (s->readThread, NULL);
			clog << "IO read thread exited" << endl;
		}

/*
#ifdef HAVE_MMAP
		if (s->source == IO_SOURCE_MMAP)
		{
			if (s->mem && munmap(s->mem, s->size))
			{
				logit ("munmap() failed: %s", strerror(errno));
			}

			close (s->fd);
		}
*/

		if (s->source == IO_SOURCE_FD)
		{
			close (s->fd);
		}

		if (s->buffered)
		{
			FifoBufDestroy (&s->buf);

			if (pthread_cond_destroy(&s->bufFreeCond))
			{
				clog << "Destroying bufFreeCond faild: " << strerror(errno) << endl;
			}

			if (pthread_cond_destroy(&s->bufFillCond))
			{
				clog << "Destroying buf_fill_cond faild: " << strerror(errno) << endl;
			}
		}

		if (s->metadata.title)
		{
			free (s->metadata.title);
		}

		if (s->metadata.url)
		{
			free (s->metadata.url);
		}
	}

	if (pthread_mutex_destroy(&s->bufMutex))
	{
		clog << "Destroying buf_mutex failed: " << strerror(errno) << endl;
	}
	if (pthread_mutex_destroy(&s->ioMutex))
	{
		clog << "Destroying io_mutex failed: " << strerror(errno) << endl;
	}
	if (pthread_mutex_destroy(&s->metadata.mutex))
	{
		clog << "Destroying metadata mutex failed: " << strerror(errno) << endl;
	}

	if (s->strerror)
	{
		free (s->strerror);
	}

	delete s;

	clog << "Done" << endl;
}

char *ioStrerror (IoStream *s)
{
	char err[256];

	if (s->strerror)
	{
		free (s->strerror);
	}

	if (s->errnoVal) {
		strerror_r (s->errnoVal, err, sizeof(err));
		s->strerror = xstrdup (err);
	}
	else
		s->strerror = xstrdup ("OK");

	return s->strerror;
}

ssize_t ioRead (IoStream *s, void *buf, size_t count)
{
	ssize_t received;

	assert (s != NULL);
	assert (buf != NULL);
	assert (s->opened);

	cout << "Reading..." << endl;

	if (s->buffered)
	{
		received = ioReadBuffered (s, buf, count);
	}
	else if (s->eof)
	{
		received = 0;
	}
	else
	{
		received = ioReadUnbuffered (s, 0, buf, count);
	}

	return received;
}

long ioTell (IoStream *s)
{
	long res = -1;

	assert (s != NULL);

	if (s->buffered)
	{
		pthread_mutex_lock (&s->bufMutex);
		res = s->pos;
		pthread_mutex_unlock (&s->bufMutex);
	}
	else
		res = s->pos;

	cout << "We are at " << res << " byte" << endl;

	return res;
}

int ioEof (IoStream *s)
{
	int eof;

	assert (s != NULL);

	pthread_mutex_lock (&s->bufMutex);
	eof = (s->eof && (!s->buffered || !FifoBufGetFill(&s->buf))) || s->stopReadThread;
	pthread_mutex_unlock (&s->bufMutex);

	return eof;
}

off_t ioSeek (IoStream *s, off_t offset, int whence)
{
	off_t res;
	off_t newPos = 0;

	assert (s != NULL);
	assert (s->opened);

	if (s->source == IO_SOURCE_CURL || !ioOk(s))
	{
		return -1;
	}

	pthread_mutex_lock (&s->ioMutex);
	switch (whence)
	{
	case SEEK_SET:
		if (offset >= 0 && (size_t)offset < s->size)
		{
			newPos = offset;
		}
		break;

	case SEEK_CUR:
		if ((ssize_t)s->pos + offset >= 0 && s->pos + offset < s->size)
		{
			newPos = s->pos + offset;
		}
		break;

	case SEEK_END:
		newPos = s->size + offset;
		break;

	default:
		cerr << "Bad whence value: " << whence << endl;
	}

	if (s->buffered)
		res = ioSeekBuffered (s, newPos);
	else
		res = ioSeekUnbuffered (s, newPos);

	if (res != -1)
		s->pos = res;
	pthread_mutex_unlock (&s->ioMutex);

	if (res != -1)
	{
		cout << "Seek to: " << (unsigned long)res << endl;
	}
	else
	{
		clog << "Seek error" << endl;
	}

	return res;
}

/* Get the file size if available or -1. */
ssize_t ioFileSize (const IoStream *s)
{
	assert (s != NULL);

	return s->size;
}

/* Reat the data from the stream resource. If dont_move was set, the stream
 * position is unchanded. */
ssize_t ioInternalRead (IoStream *s, const int dont_move, char *buf, size_t count)
{
	ssize_t res = 0;

	assert (s != NULL);
	assert (buf != NULL);

	if (s->source == IO_SOURCE_FD)
	{
		res = ioReadFd (s, dont_move, buf, count);
	}
	else
	{
		cerr << "Unknown io_stream->source: " << s->source << endl;
	}

	return res;
}

void ioAbort (IoStream *s)
{
	assert (s != NULL);

	if (s->buffered && !s->stopReadThread)
	{
		clog << "Aborting..." << endl;
		pthread_mutex_lock (&s->bufMutex);
		s->stopReadThread = 1;
		ioWakeUp (s);
		pthread_cond_broadcast (&s->bufFillCond);
		pthread_cond_broadcast (&s->bufFreeCond);
		pthread_mutex_unlock (&s->bufMutex);
		clog << "done" << endl;
	}
}

ssize_t ioReadFd (IoStream *s, const int dont_move, void *buf, size_t count)
{
	ssize_t res;

	res = read (s->fd, buf, count);

	if (res < 0)
	{
		return -1;
	}

	if (dont_move && lseek(s->fd, -res, SEEK_CUR) < 0)
	{
		return -1;
	}

	return res;
}

off_t ioSeekFd (IoStream *s, const int where)
{
	return lseek (s->fd, where, SEEK_SET);
}

off_t ioSeekBuffered (IoStream *s, const long where)
{
	off_t res = -1;

	clog << "Seeking..." << endl;

	if (s->source == IO_SOURCE_FD)
		res = ioSeekFd (s, where);
	else
		cerr << "Unknown io_stream->source: " << s->source;

	pthread_mutex_lock (&s->bufMutex);
	FifoBufClear (&s->buf);
	pthread_cond_signal (&s->bufFreeCond);
	s->afterSeek = 1;
	s->eof = 0;
	pthread_mutex_unlock (&s->bufMutex);

	return res;
}

off_t ioSeekUnbuffered (IoStream *s, const long where)
{
	off_t res = -1;

	if (s->source == IO_SOURCE_FD)
		res = ioSeekFd (s, where);

	return res;
}

void ioWakeUp (IoStream *s)
{
	/*
#ifdef HAVE_CURL
	if (s->source == IO_SOURCE_CURL)
		io_curl_wake_up (s);
#endif
		*/
}

ssize_t ioReadBuffered (IoStream *s, void *buf, size_t count)
{
	ssize_t received = 0;

	pthread_mutex_lock (&s->bufMutex);

	while (received < (ssize_t)count && !s->stopReadThread && ((!s->eof && !s->readError) || FifoBufGetFill(&s->buf)))
	{
		if (FifoBufGetFill(&s->buf))
		{
			received += FifoBufGet (&s->buf, (char *)buf + received, count - received);
			cout << "Read " << (int)received << " bytes so far" << endl;
			pthread_cond_signal (&s->bufFreeCond);
		}
		else
		{
			cout  << "Buffer empty, waiting..." << endl;
			pthread_cond_wait (&s->bufFillCond, &s->bufMutex);
		}
	}

	cout << "done" << endl;
	s->pos += received;

	pthread_mutex_unlock (&s->bufMutex);

	return received ? received : (s->readError ? -1 : 0);
}

/* Read data from the stream without buffering. If dont_move was set, the
 * stream position is unchanged. */
ssize_t ioReadUnbuffered (IoStream *s, const int dont_move, void *buf, size_t count)
{
	ssize_t res;

	assert (!s->eof);

	res = ioInternalRead (s, dont_move, (char *)buf, count);

	if (!dont_move)
	{
		s->pos += res;
		if (res == 0)
			s->eof = 1;
	}

	return res;
}

