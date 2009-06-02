#include "foooutbuf.hpp"

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "fooaudiostruct.hpp"
#include "foofifobuf.hpp"
#include "foooutbuf.hpp"

/* Don't play more than that value (in seconds) in one audio_play().
 * This prevent locking. */
#define AUDIO_MAX_PLAY		0.1
#define AUDIO_MAX_PLAY_BYTES	32768

/* Initialize the buf structure, size is the buffer size. */
void FooOutBuf::init (int size)
{
	//assert (this != NULL);
	assert (size > 0);

	FifoBufInit (&buf, size);
	exit = 0;
	pause_ = 0;
	stop_ = 0;
	time = 0.0;
	resetDev = 0;
	hardwareBufFill = 0;
	readThreadWaiting = 0;
	freeCallback = NULL;

	pthread_mutex_init (&mutex, NULL);
	pthread_cond_init (&playCond, NULL);
	pthread_cond_init (&readyCond, NULL);

	/*fd = open ("out_test", O_CREAT | O_TRUNC | O_WRONLY, 0600);*/

	if (pthread_create(&tid, NULL, read_thread, buf)) {
		clog << "Can't create buffer thread: " << strerror(errno) << endl;
		cerr << "Can't create buffer thread" << endl;
	}
}

void FooOutBuf::destroy ()
{
	//assert (this != NULL);

	pthread_mutex_lock(&mutex);
	exit = 1;
	pthread_cond_signal (&playCond);
	pthread_mutex_unlock(&mutex);

	pthread_join (tid, NULL);

	/* Let know other threads using this buffer that the state of the
	 * buffer is different. */
	pthread_mutex_lock(&mutex);
	FifoBufClear (&buf);
	pthread_cond_broadcast (&readyCond);
	pthread_mutex_unlock(&mutex);

	fifo_buf_destroy (&buf->buf);

	if (pthread_mutex_destroy(&mutex))
		clog << "Destroying buffer mutex failed: " << strerror(errno) << endl;
	if (pthread_cond_destroy(&playCond))
		clog << "Destroying buffer play condition failed: " << strerror(errno) << endl;
	if (pthread_cond_destroy(&readyCond))
		clog << "Destroying buffer ready condition failed: " << strerror(errno) << endl;

	clog << "buffer destroyed" << endl;

	/*close (fd);*/
}

/* Put data at the end of the buffer, return 0 if nothing was put. */
int FooOutBuf::put (const char *data, int size)
{
	int pos = 0;

	/*logit ("got %d bytes to play", size);*/

	while (size)
	{
		int written;

		pthread_mutex_lock (&mutex);

		if (FifoBufGetSpace(&buf) == 0 && !stop_)
		{
			/*logit ("buffer full, waiting for the signal");*/
			pthread_cond_wait (&readyCond, &mutex);
			/*logit ("buffer ready");*/
		}

		if (stop_)
		{
			clog << "the buffer is stop_ped, refusing to write to the buffer" << endl;
			pthread_mutex_unlock(&mutex);
			return 0;
		}

		written = FifoBufPut (&buf, data + pos, size);

		if (written)
		{
			pthread_cond_signal (&playCond);
			size -= written;
			pos += written;
		}

		pthread_mutex_unlock (&mutex);

	}

	return 1;
}

void FooOutBuf::pause ()
{
	pthread_mutex_lock(&mutex);
	pause_ = 1;
	resetDev = 1;
	pthread_mutex_unlock(&mutex);
}

void FooOutBuf::unpause ()
{
	pthread_mutex_lock(&mutex);
	pause_ = 0;
	pthread_cond_signal (&playCond);
	pthread_mutex_unlock(&mutex);
}

/* stop playing, after that buffer will refuse to play anything and ignore data
 * sent by buf_put(). */
void FooOutBuf::stop ()
{
	clog << "stopping the buffer" << endl;
	pthread_mutex_lock(&mutex);
	stop_ = 1;
	pause_ = 0;
	resetDev = 1;
	clog << "sending signal" << endl;
	pthread_cond_signal (&playCond);
	clog << "waiting for signal" << endl;
	pthread_cond_wait (&readyCond, &mutex);
	clog << "done" << endl;
	pthread_mutex_unlock(&mutex);
}

/* Reset the buffer state: this can by called ONLY when the buffer is stopped
 * and buf_put is not used! */
void FooOutBuf::reset ()
{
	clog << "resetting the buffer" << endl;

	pthread_mutex_lock(&mutex);
	FifoBufClear (&buf);
	stop_ = 0;
	pause_ = 0;
	reset_dev = 0;
	hardwareBufFill = 0;

	pthread_mutex_unlock(&mutex);
}

void FooOutBuf::timeSet (const float time)
{
	pthread_mutex_lock(&mutex);
	time = time;
	pthread_mutex_unlock(&mutex);
}

int FooOutBuf::timeGet ()
{
	int time;
	int bps = audioGetBps ();

	pthread_mutex_lock(&mutex);
	time = buf->time - (bps ? buf->hardware_buf_fill / (float)bps : 0);
	pthread_mutex_unlock(&mutex);

	assert (time >= 0);

	return time;
}

void FooOutBuf::setFreeCallback(out_buf_free_callback callback)
{
	//assert (this != NULL);

	pthread_mutex_lock(&mutex);
	freeCallback = callback;
	pthread_mutex_unlock(&mutex);
}

int FooOutBuf::getFree ()
{
	int space;

	//assert (this != NULL);

	pthread_mutex_lock(&mutex);
	space = FifoBufGetSpace (&buf);
	pthread_mutex_unlock(&mutex);

	return space;
}

int FooOutBuf::getFill ()
{
	int fill;

	//assert (this != NULL);

	pthread_mutex_lock(&mutex);
	fill = FifoBufGetFill (&buf);
	pthread_mutex_unlock(&mutex);

	return fill;
}

/* Wait until the read thread will stop and wait for data to come.
 * This makes sure that the audio device isn't used (of course only if you
 * don't put anything in the buffer). */
void FooOutBuf::wait ()
{
	//assert (this != NULL);

	clog << "Waiting for read thread to suspend..." << endl;

	pthread_mutex_lock(&mutex);

	while (!readThreadWaiting)
	{
		cout << "waiting...." << endl;
		pthread_cond_wait (&readyCond, &mutex);
	}
	pthread_mutex_unlock(&mutex);

	clog << "done" << endl;
}
