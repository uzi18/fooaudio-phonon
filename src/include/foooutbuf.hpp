#ifndef _FOOOUTBUF_HPP_
#define _FOOOUTBUF_HPP_

#include <pthread.h>
#include "foofifobuf.hpp"

typedef void (out_buf_free_callback)();

class FooOutBuf
{
private:
	FifoBuf buf;
	pthread_mutex_t mutex;
	pthread_t tid;	/* Thread id of the reading thread. */

	/* Signals. */
	pthread_cond_t playCond;	/* Something was written to the buffer. */
	pthread_cond_t readyCond;	/* There is some space in the buffer. */

	/* Optional callback called when there is some free space in
	 * the buffer. */
	out_buf_free_callback *free_callback;

	/* State flags of the buffer. */
	int pause_;
	int exit;	/* Exit when the buffer is empty. */
	int stop_;	/* Don't play anything. */

	int resetDev;	/* request to the reading thread to reset the audio
				device */

	float time;	/* Time of played sound .*/
	int hardwareBufFill;	/* How the sound card buffer is filled */

	int readThreadWaiting; /* is the read thread waiting for data? */

public:
	void init (int size);
	void destroy ();
	int put (const char *data, int size);
	void pause ();
	void unpause ();
	void stop ();
	void reset ();
	void timeSet (const float time);
	int timeGet ();
	void setFreeCallback (out_buf_free_callback callback);
	int getFree ();
	int getFill ();
	void wait ();
};

#endif // _FOOOUTBUF_HPP_
