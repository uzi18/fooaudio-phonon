#ifndef _FOOFLACDATA_HPP_
#define _FOOFLACDATA_HPP_

#include "../include/fooio.hpp"

#define MAX_SUPPORTED_CHANNELS		2

#define SAMPLES_PER_WRITE		512
#define SAMPLE_BUFFER_SIZE ((FLAC__MAX_BLOCK_SIZE + SAMPLES_PER_WRITE) * MAX_SUPPORTED_CHANNELS * (32/8))

struct FlacData
{
	FLAC__StreamDecoder *decoder;
	IoStream *stream;
	int bitrate;
	int avgBitrate;
	int abort; /* abort playing (due to an error) */

	unsigned int length;
	unsigned int totalSamples;

	FLAC__byte sampleBuffer[SAMPLE_BUFFER_SIZE];

	 unsigned int sampleBufferFill;

	 /* sound parameters */
	 unsigned int bitsPerSample;
	 unsigned int sampleRate;
	 unsigned int channels;

	 FLAC__uint64 lastDecodePosition;
	 int ok; /* was this stream successfully opened? */
	 DecoderError error;
};

#endif // _FOOFLACDATA_HPP_
