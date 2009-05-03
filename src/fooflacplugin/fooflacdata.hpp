#ifndef _FOOFLACDATA_HPP_
#define _FOOFLACDATA_HPP_

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
	 unsigned int sample_rate;
	 unsigned int channels;

	 FLAC__uint64 lastDecodePosition;
	 int ok; /* was this stream successfully opened? */
	 decoderError error;
}

#endif // _FOOFLACDATA_HPP_
