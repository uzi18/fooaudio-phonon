#include <iostream>
#include <algorithm>

using namespace std;

#include "fooflacplugin.hpp"
#include "fooflacdata.hpp"
#include "../include/foodecoder.hpp"
#include "../include/fooio.hpp"

#define MAX_SUPPORTED_CHANNELS		2

#define SAMPLES_PER_WRITE		512
#define SAMPLE_BUFFER_SIZE ((FLAC__MAX_BLOCK_SIZE + SAMPLES_PER_WRITE) * MAX_SUPPORTED_CHANNELS * (32/8))

void *FooFlacPlugin::open (const char *file)
{
	return openInternal(file, 1);
}

void *FooFlacPlugin::openInternal (const char *file, const int buffered)
{
	FlacData *data = new FlacData ();

	decoderErrorInit (&data->error);

	data->decoder = NULL;
	data->bitrate = -1;
	data->avgBitrate = -1;
	data->abort = 0;
	data->sampleBufferFill = 0;
	data->lastDecodePosition = 0;

	data->stream = ioOpen (file, buffered);

	if (!ioOk(data->stream))
	{
		decoderError (&data->error, ERROR_FATAL, 0, "Can't load file: %s", ioStrerror(data->stream));
		ioClose (data->stream);

		return data;
	}

	data->ok = 1;

	if (!(data->decoder = FLAC__stream_decoder_new()))
	{
		decoderError (&data->error, ERROR_FATAL, 0, "FLAC__stream_decoder_new() failed");
		data->ok = 0;
		return data;
	}

	FLAC__stream_decoder_set_md5_checking (data->decoder, false);
	FLAC__stream_decoder_set_metadata_ignore_all (data->decoder);
	FLAC__stream_decoder_set_metadata_respond (data->decoder, FLAC__METADATA_TYPE_STREAMINFO);

	if (FLAC__stream_decoder_init_stream(data->decoder, readCallback, seekCallback, tellCallback, lengthCallback, eofCallback, writeCallback, metadataCallback, errorCallback, data) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
	{
		decoderError (&data->error, ERROR_FATAL, 0, "FLAC__stream_decoder_init() failed");
		data->ok = 0;
		return data;
	}

	if (!FLAC__stream_decoder_process_until_end_of_metadata(data->decoder))
	{
		decoderError (&data->error, ERROR_FATAL, 0, "FLAC__stream_decoder_process_until_end_of_metadata() failed.");
		data->ok = 0;
		return data;
	}

	data->avgBitrate = (data->bitsPerSample) * data->sampleRate;

	return data;
}

void FooFlacPlugin::close (void *void_data)
{
	FlacData *data = (FlacData *)void_data;

	if (data->ok)
	{
		if (data->decoder)
		{
			FLAC__stream_decoder_finish (data->decoder);
			FLAC__stream_decoder_delete (data->decoder);
		}

		ioClose (data->stream);
	}

	delete data;
}

int FooFlacPlugin::decode (void *void_data, char *buf, int buf_len, SoundParams *sound_params)
{
	FlacData *data = (FlacData *)void_data;
	unsigned int toCopy;
	int bytesPerSample;
	FLAC__uint64 decodePosition;

	bytesPerSample = data->bitsPerSample / 8;

	switch (bytesPerSample)
	{
		case 1:
			sound_params->fmt = SFMT_S8;
			break;
		case 2:
			sound_params->fmt = SFMT_S16 | SFMT_LE;
			break;
		case 3:
			sound_params->fmt = SFMT_S32 | SFMT_LE;
			break;
	}

	sound_params->rate = data->sampleRate;
	sound_params->channels = data->channels;

	decoderErrorClear (&data->error);

	if (!data->sampleBufferFill)
	{
		cout << "decoding..." << endl;

		if (FLAC__stream_decoder_get_state(data->decoder) == FLAC__STREAM_DECODER_END_OF_STREAM)
		{
			clog << "EOF" << endl;
			return 0;
		}

		if (!FLAC__stream_decoder_process_single(data->decoder))
		{
			decoderError (&data->error, ERROR_FATAL, 0, "Read error processing frame.");
			return 0;
		}

		if(!FLAC__stream_decoder_get_decode_position(data->decoder, &decodePosition))
		{
			decodePosition = 0;
		}

		if (decodePosition > data->lastDecodePosition)
		{
			int bytesPerSec = bytesPerSample * data->sampleRate * data->channels;

			data->bitrate = (decodePosition - data->lastDecodePosition) * 8.0 / (data->sampleBufferFill / (double)bytesPerSec) / 1000;
		}

		data->lastDecodePosition = decodePosition;
	}
	else
	{
		cout << "Some date remain in the buffer" << endl;
	}

	cout << "Decoded " << data->sampleBufferFill << ", data->sample_buffer_fill" << endl;

	toCopy = min((unsigned int)bufLen, data->sampleBufferFill);
	memcpy (buf, data->sampleBuffer, toCopy);
	memmove (data->sampleBuffer, data->sampleBuffer + toCopy,
			data->sampleBufferFill - toCopy);
	data->sampleBufferFill -= toCopy;

	return toCopy;
}

int FooFlacPlugin::seek (void *void_data, int sec)
{
	FlacData *data = (FlacData *)void_data;
	FLAC__uint64 targetSample;

	if (sec < 0 || (unsigned)sec > data->length)
	{
		return -1;
	}

	targetSample = (FLAC__uint64)((sec/(double)data->length) * (double)data->totalSamples);

	if (FLAC__stream_decoder_seek_absolute(data->decoder, targetSample))
	{
		return sec;
	}
	else
	{
		clog << "FLAC__stream_decoder_seek_absolute() failed." << endl;
		return -1;
	}
}

void FooFlacPlugin::info (const char *file_name, FileTags *info, const int tags_sel)
{
	if (tags_sel & TAGS_TIME)
	{
		 FlacData *data;

		if ((data = (FlacData *)openInternal(file_name, 0)))
		 {
			info->time = data->length;
			close (data);
		}
	}

	if (tags_sel & TAGS_COMMENTS)
	{
		get_vorbiscomments (file_name, info);
	}
}

int FooFlacPlugin::getBitrate (void *void_data)
{
	FlacData *data = (FlacData *)void_data;

	return data->bitrate;
}

int FooFlacPlugin::getDuration (void *void_data)
{
	FlacData *data = (FlacData *)void_data;

	return data->length;
}

void FooFlacPlugin::getError (void *prv_data, DecoderError *error)
{
	FlacData *data = (FlacData *)prv_data;

	decoderErrorCopy (error, &data->error);
}

int FooFlacPlugin::ourFormatExt(const char *ext)
{
	return !strcasecmp(ext, "flac") || !strcasecmp(ext, "fla");
}

void FooFlacPlugin::getName (const char *file, char buf[4])
{
	strcpy (buf, "FLA");
}

int FooFlacPlugin::getAvgBitrate (void *void_data)
{
	FlacData *data = (FlacData *)void_data;

	return data->avgBitrate / 1000;
}

Q_EXPORT_PLUGIN2(fooflacplugin, FooFlacPlugin)
