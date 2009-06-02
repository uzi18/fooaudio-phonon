#include <alsa/asoundlib.h>
#include <errno.h>
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

#include "../include/fooaudiostruct.hpp"
#include "fooalsaplugin.hpp"
#include "fooalsaoption.hpp"

FooAlsaPlugin::FooAlsaPlugin () : BUFFER_MAX_USEC(300000)
{
	handle = NULL;
	chunk_size = -1;
	alsa_buf_fill = 0;
	mixer_handle = NULL;
	mixer_elem1 = NULL;
	mixer_elem2 = NULL;
	mixer_elem_curr = NULL;
	mixer1_min = -1;
	mixer1_max = -1;
	mixer2_min = -1;
	mixer2_max = -1;
	volume1 = -1;
	volume2 = -1;
	real_volume1 = -1;
	real_volume2 = -1;

	params.channels = 0;
	params.rate = 0;
	params.format = SND_PCM_FORMAT_UNKNOWN;
}

int FooAlsaPlugin::init(OutputDriverCaps *caps)
{
	int err;

	if ((err = snd_mixer_open(&mixer_handle, 0)) < 0)
	{
		cerr << "Can't open ALSA mixer: " << snd_strerror(err) << endl;
		mixer_handle = NULL;
	}
	else if ((err = snd_mixer_attach(mixer_handle, options_get_str("AlsaDevice"))) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't attach mixer: " << snd_strerror(err) << endl;
	}
	else if ((err = snd_mixer_selem_register(mixer_handle, NULL, NULL)) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't register mixer: " << snd_strerror(err) << endl;
	}
	else if ((err = snd_mixer_load(mixer_handle)) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't load mixer: " << snd_strerror(err) << endl;
	}

	if (mixer_handle)
	{
		mixer_elem1 = initMixerChannel(options_get_str("AlsaMixer"), &mixer1_min, &mixer1_max);
		mixer_elem2 = initMixerChannel(options_get_str("AlsaMixer2"), &mixer2_min, &mixer2_max);
	}

	mixer_elem_curr = mixer_elem1 ? mixer_elem1 : mixer_elem2;

	if (mixer_elem_curr)
	{
		if (mixer_elem1 && (real_volume1 = readMixerRaw(mixer_elem1)) != -1)
		{
			volume1 = scale_volume1 (real_volume1);
		}
		else
		{
			mixer_elem1 = NULL;
			mixer_elem_curr = mixer_elem2;
		}

		if (mixer_elem2 && (real_volume2 = readMixerRaw(mixer_elem2)) != -1)
		{
			volume2 = scale_volume2 (real_volume2);
		}
		else
		{
			mixer_elem2 = NULL;
			mixer_elem_curr = mixer_elem1;
		}

		if (!mixer_elem_curr)
		{
			snd_mixer_close (mixer_handle);
			mixer_handle = NULL;
		}
	}
	else if (mixer_handle)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
	}

	return fillCapabilities (caps);
}

int FooAlsaPlugin::fillCapabilities (OutputDriverCaps *caps)
{
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_mask_t *format_mask;
	int err;
	unsigned val;

	if ((err = snd_pcm_open(&handle, options_get_str("AlsaDevice"), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0)
	{
		cerr << "Can't open audio: " << snd_strerror(err) << endl;

		return 0;
	}

	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
	{
		cerr << "Can't allocate alsa hardware parameters structure: " << snd_strerror(err) << endl;
		snd_pcm_close (handle);

		return 0;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0)
	{
		cerr << "Can't initialize hardware parameters structure: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free(hw_params);
		snd_pcm_close(handle);

		return 0;
	}

	if ((err = snd_pcm_hw_params_get_channels_min (hw_params, &val)) < 0)
	{
		cerr << "Can't get the minimum number of channels: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		snd_pcm_close (handle);

		return 0;
	}

	caps->min_channels = val;

	if ((err = snd_pcm_hw_params_get_channels_max (hw_params, &val)) < 0)
	{
		cerr << "Can't get the maximum number of channels: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		snd_pcm_close (handle);
		return 0;
	}

	caps->max_channels = val;

	if ((err = snd_pcm_format_mask_malloc(&format_mask)) < 0)
	{
		cerr << "Can't allocate format mask: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		snd_pcm_close (handle);

		return 0;
	}

	snd_pcm_hw_params_get_format_mask (hw_params, format_mask);

	caps->formats = SFMT_NE;

	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_S8))
	{
		caps->formats |= SFMT_S8;
	}
	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_U8))
	{
		caps->formats |= SFMT_U8;
	}
	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_S16))
	{
		caps->formats |= SFMT_S16;
	}
	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_U16))
	{
		caps->formats |= SFMT_U16;
	}
	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_S32))
	{
		caps->formats |= SFMT_S32;
	}
	if (snd_pcm_format_mask_test(format_mask, SND_PCM_FORMAT_U32))
	{
		caps->formats |= SFMT_U32;
	}

	snd_pcm_format_mask_free (format_mask);
	snd_pcm_hw_params_free (hw_params);
	snd_pcm_close (handle);
	handle = NULL;

	return 1;
}

void FooAlsaPlugin::shutdown()
{
	int err;

	if (mixer_handle && (err = snd_mixer_close(mixer_handle)) < 0)
	{
		clog << "Can't close mixer: " << snd_strerror(err) << endl;
	}
}

int FooAlsaPlugin::open (struct SoundParams *sound_params)
{
	snd_pcm_hw_params_t *hw_params;
	int err;
	unsigned int period_time;
	unsigned int buffer_time;
	snd_pcm_uframes_t chunk_frames;
	snd_pcm_uframes_t buffer_frames;
	char fmt_name[128];

	switch (sound_params->fmt & SFMT_MASK_FORMAT)
	{
		case SFMT_S8:
			params.format = SND_PCM_FORMAT_S8;
			break;

		case SFMT_U8:
			params.format = SND_PCM_FORMAT_U8;
			break;

		case SFMT_S16:
			params.format = SND_PCM_FORMAT_S16;
			break;

		case SFMT_U16:
			params.format = SND_PCM_FORMAT_U16;
			break;

		case SFMT_S32:
			params.format = SND_PCM_FORMAT_S32;
			break;

		case SFMT_U32:
			params.format = SND_PCM_FORMAT_U32;
			break;

		default:
			cerr << "Unknown sample format: " << sfmt_str(sound_params->fmt, fmt_name, sizeof(fmt_name)) << endl;
			params.format = SND_PCM_FORMAT_UNKNOWN;
			return 0;
	}

	if ((err = snd_pcm_open(&handle, options_get_str("AlsaDevice"), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0)
	{
		cerr << "Can't open audio: " << snd_strerror(err) << endl;
		return 0;
	}

	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
	{
		cerr << "Can't allocate alsa hardware parameters structure: " << snd_strerror(err) << endl;
		return 0;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0)
	{
		cerr << "Can't initialize hardware parameters structure: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);

		return 0;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		cerr << "Can't set alsa access type: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, params.format)) < 0)
	{
		cerr << "Can't set sample format: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	params.rate = sound_params->rate;
	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &params.rate, 0)) < 0)
	{
		cerr << "Can't set sample rate: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	clog << "Set rate to " << params.rate << endl;

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, sound_params->channels)) < 0)
	{
		cerr << "Can't set number of channels: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);

		return 0;
	}

	if ((err = snd_pcm_hw_params_get_buffer_time_max(hw_params, &buffer_time, 0)) < 0)
	{
		cerr << "Can't get maximum buffer time: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if (buffer_time > BUFFER_MAX_USEC)
	{
		buffer_time = BUFFER_MAX_USEC;
	}
	period_time = buffer_time / 4;

	if ((err = snd_pcm_hw_params_set_period_time_near(handle, hw_params, &period_time, 0)) < 0)
	{
		cerr << "Can't set period time: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);

		return 0;
	}

	if ((err = snd_pcm_hw_params_set_buffer_time_near(handle, hw_params, &buffer_time, 0)) < 0)
	{
		cerr << "Can't set buffer time: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0)
	{
		cerr << "Can't set audio parameters: " << snd_strerror(err) << endl;
		snd_pcm_hw_params_free (hw_params);

		return 0;
	}

	snd_pcm_hw_params_get_period_size (hw_params, &chunk_frames, 0);
	snd_pcm_hw_params_get_buffer_size (hw_params, &buffer_frames);

	bytes_per_frame = sound_params->channels * sfmt_Bps(sound_params->fmt);

	clog << "Buffer time: " << buffer_frames * bytes_per_frame << "us" << endl;

	if (chunk_frames == buffer_frames)
	{
		cerr << "Can't use period equal to buffer size (" << chunk_frames << " == " << buffer_frames << ")" << endl;
		snd_pcm_hw_params_free (hw_params);

		return 0;
	}

	chunk_size = chunk_frames * bytes_per_frame;

//	debug ("Chunk size: %d", chunk_size);

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare(handle)) < 0)
	{
		cerr << "Can't prepare audio interface for use: " << snd_strerror(err) << endl;
		return 0;
	}

//	debug ("ALSA device initialized");

	params.channels = sound_params->channels;
	alsa_buf_fill = 0;
	return 1;
}

void FooAlsaPlugin::close ()
{
	assert (handle != NULL);

	/* play what remained in the buffer */
	if (alsa_buf_fill)
	{
		assert (alsa_buf_fill < chunk_size);
		/* FIXME: why the last argument is multiplied by number of channels? */
		snd_pcm_format_set_silence (params.format, alsa_buf + alsa_buf_fill, (chunk_size - alsa_buf_fill) / bytes_per_frame * params.channels);
		playBufChunks ();
	}

	params.format = (snd_pcm_format_t)0;
	params.rate = 0;
	params.channels = 0;
	snd_pcm_close (handle);
	clog << "ALSA device closed";
	handle = NULL;
}

int FooAlsaPlugin::play (const char *buff, const size_t size)
{
	int to_write = size;
	int buf_pos = 0;

	assert (chunk_size > 0);

//	debug ("Got %d bytes to play", (int)size);

	while (to_write)
	{
		int to_copy = min((size_t)to_write, sizeof(alsa_buf) - (size_t)alsa_buf_fill);
		memcpy (alsa_buf + alsa_buf_fill, buff + buf_pos, to_copy);

		to_write -= to_copy;
		buf_pos += to_copy;
		alsa_buf_fill += to_copy;

//		debug ("Copied %d bytes to alsa_buf (now is filled with %d bytes)", to_copy, alsa_buf_fill);
		if (playBufChunks() < 0)
		{
			return -1;
		}
	}

//	debug ("Played everything");

	return size;
}

int FooAlsaPlugin::readMixer ()
{
	int curr_real_vol = readMixerRaw (mixer_elem_curr);
	int *real_vol;
	int *vol;

	if (mixer_elem_curr == mixer_elem1)
	{
		real_vol = &real_volume1;
		vol = &volume1;
	}
	else
	{
		real_vol = &real_volume2;
		vol = &volume2;
	}

	if (*real_vol != curr_real_vol)
	{
		*real_vol = curr_real_vol;
		*vol = (vol == &volume1) ? scale_volume1(*real_vol) : scale_volume2(*real_vol);
		clog << "Mixer volume has changes since we last read it." << endl;
	}

	return *vol;
}

void FooAlsaPlugin::setMixer (int vol)
{
	if (mixer_handle)
	{
		int err;
		long vol_alsa;
		long mixer_max, mixer_min;
		int *real_vol;

		if (mixer_elem_curr == mixer_elem1)
		{
			volume1 = vol;
			mixer_max = mixer1_max;
			mixer_min = mixer1_min;
			real_vol = &real_volume1;
		}
		else
		{
			volume2 = vol;
			mixer_max = mixer2_max;
			mixer_min = mixer2_min;
			real_vol = &real_volume2;
		}

		vol_alsa = vol * (mixer_max - mixer_min) / 100;

//		debug ("Setting vol to %ld", vol_alsa);

		 if ((err = snd_mixer_selem_set_playback_volume_all( mixer_elem_curr, vol_alsa)) < 0)
		 {
			 cerr << "Can't set mixer: " <<  snd_strerror(err) << endl;
		 }
		 else
		 {
			 *real_vol = vol_alsa;
		 }
	}
}

int FooAlsaPlugin::getBuffFill ()
{
	if (handle)
	{
		int err;
		snd_pcm_sframes_t delay;

		if ((err = snd_pcm_delay(handle, &delay)) < 0)
		{
			clog << "snd_pcm_delay() failed: " << snd_strerror(err) << endl;
			return 0;
		}

		/* delay can be negative when underrun occur */
		return delay >= 0 ? delay * bytes_per_frame : 0;
	}

	return 0;
}

int FooAlsaPlugin::reset()
{
	if (handle)
	{
		int err;

		if ((err = snd_pcm_drop(handle)) < 0)
		{
			cerr << "Can't reset the device: " << snd_strerror(err) << endl;
			return 0;
		}
		if ((err = snd_pcm_prepare(handle)) < 0)
		{
			cerr << "Can't prepare anfter reset: " << snd_strerror(err) << endl;
			return 0;
		}

		alsa_buf_fill = 0;
	}
	else
	{
		clog << "alsa_reset() when the device is not opened." << endl;
	}

	return 1;
}

int FooAlsaPlugin::getRate ()
{
	return params.rate;
}

void FooAlsaPlugin::toggleMixerChannel ()
{
	if (mixer_elem_curr == mixer_elem1 && mixer_elem2)
	{
		mixer_elem_curr = mixer_elem2;
	}
	else if (mixer_elem1)
	{
		mixer_elem_curr = mixer_elem1;
	}
}

char *FooAlsaPlugin::getMixerChannelName ()
{
	if (mixer_elem_curr == mixer_elem1)
	{
		return strdup (options_get_str("AlsaMixer"));
	}
#warning "poprawic na cos z cpp"
	return strdup (options_get_str("AlsaMixer2"));
}

int FooAlsaPlugin::readMixerRaw (snd_mixer_elem_t *elem)
{
	if (mixer_handle)
	{
		long volume = 0;
		int nchannels = 0;
		int i;
		int err;

		assert (elem != NULL);

		handleMixerEvents (mixer_handle);

		for (i = 0; i < SND_MIXER_SCHN_LAST; ++i)
		{
			if (snd_mixer_selem_has_playback_channel(elem, (snd_mixer_selem_channel_id_t)(1 << i)))
			{
				long vol;

				nchannels++;
				if ((err = snd_mixer_selem_get_playback_volume(elem, (snd_mixer_selem_channel_id_t)(1 << i), &vol)) < 0)
				{
					cerr << "Can't read mixer: " << snd_strerror(err) << endl;
					return -1;
				}

				/*logit ("Vol %d: %ld", i, vol);*/
				volume += vol;
			}
		}
		if (nchannels > 0)
		{
			volume /= nchannels;
		}
		else
		{
			clog << "Mixer has no channels" << endl;
			volume = -1;
		}

		/*logit ("Max: %ld, Min: %ld", mixer_max, mixer_min);*/
		return volume;
	}
	else
	{
		return -1;
	}
}

void FooAlsaPlugin::handleMixerEvents (snd_mixer_t *mixer_handle)
{
	int count;

	if ((count = snd_mixer_poll_descriptors_count(mixer_handle)) < 0)
	{
		clog << "snd_mixer_poll_descriptors_count() failed: " << snd_strerror(count) << endl;
	}
	else
	{
		struct pollfd *fds;
		int err;

		fds = new pollfd[count];

		if ((err = snd_mixer_poll_descriptors(mixer_handle, fds, count)) < 0)
		{
			clog << "snd_mixer_poll_descriptors() failed: " << snd_strerror(count) << endl;
		}
		else
		{
			err = poll (fds, count, 0);
			if (err < 0)
			{
				cerr << "poll() failed: " << strerror(errno) << endl;
			}
			else if (err > 0)
			{
//				debug ("Mixer event");
				if ((err = snd_mixer_handle_events(mixer_handle)) < 0)
				{
					clog << "snd_mixer_handle_events() failed: " << snd_strerror(err) << endl;
				}
			}
		}

		delete[] fds;
	}
}

snd_mixer_elem_t *FooAlsaPlugin::initMixerChannel (const char *name, long *vol_min, long *vol_max)
{
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem = NULL;

	snd_mixer_selem_id_malloc (&sid);
	snd_mixer_selem_id_set_index (sid, 0);
	snd_mixer_selem_id_set_name (sid, name);

	if (!(elem = snd_mixer_find_selem(mixer_handle, sid)))
	{
		cerr << "Can't find mixer " << name << endl;
	}
	else if (!snd_mixer_selem_has_playback_volume(elem))
	{
		cerr << "Mixer device has no playback volume (" << name << ")." << endl;
		elem = NULL;
	}
	else
	{
		snd_mixer_selem_get_playback_volume_range (elem, vol_min, vol_max);
		clog << "Opened mixer (" << name << "), volume range: " << *vol_min << "-" << *vol_max << endl;
	}

	snd_mixer_selem_id_free (sid);

	return elem;
}

int FooAlsaPlugin::playBufChunks()
{
	int written = 0;

	while (alsa_buf_fill >= chunk_size)
	{
		int err;

		err = snd_pcm_writei (handle, alsa_buf + written, chunk_size / bytes_per_frame);
		if (err == -EAGAIN)
		{
			if (snd_pcm_wait(handle, 500) < 0)
			{
				clog << "snd_pcm_wait() failed" << endl;
			}
		}
		else if (err == -EPIPE)
		{
			clog << "underrun!" << endl;
			if ((err = snd_pcm_prepare(handle)) < 0)
			{
				cerr << "Can't recover after underrun: " << snd_strerror(err) << endl;
				/* TODO: reopen the device */
				return -1;
			}
		}
		else if (err == -ESTRPIPE)
		{
			clog << "Suspend, trying to resume" << endl;
			while ((err = snd_pcm_resume(handle)) == -EAGAIN)
			{
				sleep (1);
			}

			if (err < 0)
			{
				clog << "Failed, restarting" << endl;
				if ((err = snd_pcm_prepare(handle)) < 0)
				{
					cerr << "Failed to restart device: " << snd_strerror(err);
					return -1;
				}
			}
		}
		else if (err < 0)
		{
			cerr << "Can't play: " << snd_strerror(err) << endl;
			return -1;
		}
		else
		{
			int written_bytes = err * bytes_per_frame;
			written += written_bytes;
			alsa_buf_fill -= written_bytes;

//			debug ("Played %d bytes", written_bytes);
		}
	}

//	debug ("%d bytes remain in alsa_buf", alsa_buf_fill);
	memmove (alsa_buf, alsa_buf + written, alsa_buf_fill);

	return written * bytes_per_frame;
}

int FooAlsaPlugin::getType()
{
	return 1;
}

Q_EXPORT_PLUGIN2(fooalsaplugin, FooAlsaPlugin)

