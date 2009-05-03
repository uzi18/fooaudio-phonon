#include <iostream>

using namespace std;

#include "fooio.hpp"

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
		FifoBufInit (&s->buf, optionsGetInt("InputBuffer") * 1024);
		s->prebuffer = optionsGetInt("Prebuffering") * 1024;

		pthread_cond_init (&s->bufFreeCond, NULL);
		pthread_cond_init (&s->bufFillCond, NULL);

		if (pthread_create(&s->readThread, NULL, ioReadThread, s))
		{
			cerr << "Can't create read thread: " << strerror (errno) << endl;
		}
	}

	return s;
}

void ioOpenFile (IoStream *, const char *file)
{
	stat file_stat;

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
/*
#ifdef HAVE_NMAP
		if (optionsGetInt("UseMmap") && s->size > 0)
		{
			if ((s->mem = mmap(0, s->size, PROT_READ, MAP_SHARED, s->fd, 0)) == MAP_FAILED)
			{
				s->mem = NULL;
				logit ("mmap() failed: %s", strerror(errno));
				s->source = IO_SOURCE_FD;
			}
			else
			{
				logit ("mmap()ed %lu bytes", (unsigned long)s->size);
				s->source = IO_SOURCE_MMAP;
				s->mem_pos = 0;
			}
		}
		else
		{
			logit ("Not using mmap()");
			s->source = IO_SOURCE_FD;
		}
#else
*/
		s->source = IO_SOURCE_FD;
/*
#endif
*/
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
			UNLOCK (s->bufMutex);
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

