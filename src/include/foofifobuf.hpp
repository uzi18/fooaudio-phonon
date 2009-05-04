#ifndef _FOOFIFOBUF_HPP_
#define _FOOFIFOBUF_HPP_

#include <unistd.h>

struct FifoBuf
{
	char *buf;  /* The buffer content. */
	int size;   /* Size of the buffer. */
	int pos; /* Current position. */
	int fill;   /* Current fill. */
};

void FifoBufInit (FifoBuf *, const size_t);
void FifoBufDestroy (FifoBuf *);
size_t FifoBufPut (FifoBuf *, const char *, size_t);
size_t FifoBufGet (FifoBuf *, char *, size_t);
size_t FifoBufPeek (FifoBuf *, char *, size_t);
size_t FifoBufGetSpace (const FifoBuf *);
void FifoBufClear (FifoBuf *);
size_t FifoBufGetFill (const FifoBuf *);
size_t FifoBufGetSize (const FifoBuf *);

#endif

