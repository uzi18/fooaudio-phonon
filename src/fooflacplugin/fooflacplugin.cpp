#include <iostream>
#include <algorithm>

using namespace std;

#include "fooflacplugin.hpp"
#include "fooflacdata.hpp"
#include "../include/footags.hpp"
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

	toCopy = min((unsigned int)buf_len, data->sampleBufferFill);
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
		getVorbiscomments (file_name, info);
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

FLAC__StreamDecoderWriteStatus writeCallback (const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	FlacData *data = (FlacData *)client_data;
	const unsigned int wideSamples = frame->header.blocksize;

	if (data->abort)
	{
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}

	data->sampleBufferFill = packPcmSigned(data->sampleBuffer, buffer, wideSamples,data->channels, data->bitsPerSample);

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}


void FooFlacPlugin::getVorbiscomments (const char *filename, FileTags *tags)
{
	FLAC__Metadata_SimpleIterator *iterator = FLAC__metadata_simple_iterator_new();
	FLAC__bool gotVorbisComments = false;

	cout << "Reading comments for " << filename << endl;

	if (!iterator)
	{
		clog << "FLAC__metadata_simple_iterator_new() failed." << endl;
		return;
	}

	if (!FLAC__metadata_simple_iterator_init(iterator, filename, true, true))
	{
		clog << "FLAC__metadata_simple_iterator_init failed." << endl;
		FLAC__metadata_simple_iterator_delete(iterator);
		return;
	}

	do
	{
		if (FLAC__metadata_simple_iterator_get_block_type(iterator) == FLAC__METADATA_TYPE_VORBIS_COMMENT)
		{
			FLAC__StreamMetadata *block;

			block = FLAC__metadata_simple_iterator_get_block (iterator);

			if (block)
			{
				unsigned int i;
				const FLAC__StreamMetadata_VorbisComment *vc = &block->data.vorbis_comment;

				for (i = 0; i < vc->num_comments; i++)
					fillTag (&vc->comments[i], tags);

				FLAC__metadata_object_delete (block);
				gotVorbisComments = true;
			}
		}
	}
	while (!gotVorbisComments && FLAC__metadata_simple_iterator_next(iterator));

	FLAC__metadata_simple_iterator_delete(iterator);
}

void FooFlacPlugin::fillTag (FLAC__StreamMetadata_VorbisComment_Entry *comm, FileTags *tags)
{
	char *name, *value;
	FLAC__byte *eq;
	int valueLength;

	eq = (FLAC__byte*)memchr (comm->entry, '=', comm->length);
	if (!eq)
	{
		return;
	}

	name = new char [eq - comm->entry + 1];
	strncpy (name, (char *)comm->entry, eq - comm->entry);
	name[eq - comm->entry] = 0;
	valueLength = comm->length - (eq - comm->entry + 1);

	if (valueLength == 0)
	{
		delete[] name;
		return;
	}

	value = new char [valueLength + 1];
	strncpy (value, (char *)(eq + 1), valueLength);
	value[valueLength] = 0;

	if (!strcasecmp(name, "title"))
	{
		tags->title = value;
	}
	else if (!strcasecmp(name, "artist"))
	{
		tags->artist = value;
	}
	else if (!strcasecmp(name, "album"))
	{
		tags->album = value;
	}
	else if (!strcasecmp(name, "tracknumber") || !strcasecmp(name, "track"))
	{
		tags->track = atoi (value);
		delete[] value;
	}
	else
	{
		delete[] value;
	}

	delete name;
}

/* Convert FLAC big-endian data into PCM little-endian. */
size_t packPcmSigned (FLAC__byte *data, const FLAC__int32 * const input[], unsigned int wide_samples, unsigned int channels, unsigned int bps)
{
	FLAC__byte * const start = data;
	FLAC__int32 sample;
	const FLAC__int32 *input_;
	unsigned int samples, channel;
	unsigned int bytesPerSample;
	unsigned int incr;

	if (bps == 24)
	{
		bps = 32; /* we encode to 32-bit words */
	}
	bytesPerSample = bps / 8;
	incr = bytesPerSample * channels;

	for (channel = 0; channel < channels; channel++)
	{
		samples = wide_samples;
		data = start + bytesPerSample * channel;
		input_ = input[channel];

		while(samples--)
		{
			sample = *input_++;

			switch(bps)
			{
			case 8:
				data[0] = sample;
				break;
			case 16:
				data[1] = (FLAC__byte)(sample >> 8);
				data[0] = (FLAC__byte)sample;
				break;
			case 32:
				data[3] = (FLAC__byte)(sample >> 16);
				data[2] = (FLAC__byte)(sample >> 8);
				data[1] = (FLAC__byte)sample;
				data[0] = 0;
				break;
			}

			data += incr;
		}
	}

	cout << "Converted %d bytes" << wide_samples * channels * bytesPerSample << endl;

	return wide_samples * channels * bytesPerSample;
}

FLAC__StreamDecoderReadStatus readCallback (const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	FlacData *data = (FlacData *)client_data;
	ssize_t res;

	res = ioRead (data->stream, buffer, *bytes);

	if (res > 0)
	{
		*bytes = res;
		return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
	}

	if (res == 0)
	{
		*bytes = 0;
		/* not sure why this works, but if it ain't broke... */
		return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
	}

	cerr << "read error: " << ioStrerror(data->stream);

	return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
}

FLAC__StreamDecoderSeekStatus seekCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	return ioSeek(data->stream, absolute_byte_offset, SEEK_SET) >= 0 ? FLAC__STREAM_DECODER_SEEK_STATUS_OK : FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
}

FLAC__StreamDecoderTellStatus tellCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	*absolute_byte_offset = ioTell (data->stream);

	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__StreamDecoderLengthStatus lengthCallback (const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	*stream_length = ioFileSize (data->stream);

	return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

FLAC__bool eofCallback (const FLAC__StreamDecoder *decoder, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	return ioEof (data->stream);
}

void metadataCallback (const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		cout << "Got metadata info" << endl;

		data->totalSamples = (unsigned int)(metadata->data.stream_info.total_samples & 0xffffffff);
		data->bitsPerSample = metadata->data.stream_info.bits_per_sample;
		data->channels = metadata->data.stream_info.channels;
		data->sampleRate = metadata->data.stream_info.sample_rate;
		data->length = data->totalSamples / data->sampleRate;
	}
}
void errorCallback (const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
	FlacData *data = (FlacData *)client_data;

	if (status != FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC) {
		cout << "Aborting due to error" << endl;
		data->abort = 1;
	}
	else
		decoderError (&data->error, ERROR_FATAL, 0, "FLAC: lost sync");
}

int FooFlacPlugin::getType()
{
	return 2;
}

Q_EXPORT_PLUGIN2(fooflacplugin, FooFlacPlugin)
