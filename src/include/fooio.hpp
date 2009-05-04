#ifndef _FOOIO_HPP_
#define _FOOIO_HPP_

#include "foofifobuf.hpp"

enum IoSource
{
	IO_SOURCE_FD,
//	IO_SOURCE_MMAP,
};

struct IoStream;

typedef void (*BufFillCallbackT) (struct IoStream *, size_t, size_t, void *);

struct IoStream
{
	enum IoSource source;
	int fd;
	size_t size;   /* source of the file if needed */
	int errnoVal; /* errno value of the last operation  - 0 if ok */
	int readError; /* set to != 0 if the last read operation dailed */
	char *strerror;   /* error string */
	int opened; /* was the stream opened (open(), mmap(), etc.)? */
	int eof; /* was the end of file reached? */
	int afterSeek;   /* are we after seek and need to do fresh read()? */
	int buffered;  /* are we using the buffer? */
	size_t pos; /* current position in the file from the user point of view */
	size_t prebuffer; /* number of bytes left to prebuffer */
	pthread_mutex_t ioMutex;  /* mutex for IO operations */

/*
#ifdef HAVE_MMAP
	void *mem;
	size_t mem_pos;
#endif
*/

	FifoBuf buf;

	pthread_mutex_t bufMutex;
	pthread_cond_t bufFreeCond; /* some space became available in the buffer */
	pthread_cond_t bufFillCond; /* the buffer was filled with some data */
	pthread_t readThread;

	int stopReadThread;      /* request for stopping the read thread */

	struct StreamMetadata
	{
		pthread_mutex_t mutex;
		char *title;   /* title of the stream */
		char *url;
	} metadata;

	/* callbacks */
	BufFillCallbackT bufFillCallback;
	void *bufFillCallbackData;
};

IoStream *ioOpen (const char *, const int);
void ioOpenFile (IoStream *, const char *);
int ioOkNolock (IoStream *);
int ioOk (IoStream *);
void *ioReadThread (void *);
char *ioStrerror (IoStream *);
void ioClose (IoStream *s);

#endif // _FOOIO_HPP_
