#include "fooflacplugin.hpp"
#include "fooflacdata.hpp"
#include "../include/foodecoder.hpp"
#include "../include/fooio.hpp"

void *FooFlacPlugin::open (const char *file)
{
	return openInternal (file, 1);
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
		decoderError (&data->error, ERROR_FATAL, 0, "Can't load file: %s", ioStrError(data->stream));
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

