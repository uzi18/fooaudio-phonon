#include <alsa/asoundlib.h>
#include <iostream>

#include "fooalsaplugin.hpp"

using namespace std;

/*static*/ int FooAlsaPlugin::init (/*struct output_driver_caps *caps*/)
{
	int err;

	if ((err = snd_mixer_open (&mixer_handle, 0)) < 0)
	{
		cerr << "Can't open ALSA mixer: " << snd_strerror (err);
		mixer_handle = NULL;
	}
	else if ((err = snd_mixer_attach (mixer_handle, options_get_str ("AlsaDevice"))) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't attach mixer: " << snd_strerror (err);
	}
	else if ((err = snd_mixer_selem_register (mixer_handle, NULL, NULL)) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't register mixer: " << snd_strerror (err);
	}
	else if ((err = snd_mixer_load(mixer_handle)) < 0)
	{
		snd_mixer_close (mixer_handle);
		mixer_handle = NULL;
		cerr << "Can't load mixer: " snd_strerror (err);
	}

	if (mixer_handle)
	{
		mixer_elem1 = alsa_init_mixer_channel (options_get_str ("AlsaMixer"), &mixer1_min, &mixer1_max);
		mixer_elem2 = alsa_init_mixer_channel (options_get_str ("AlsaMixer2"), &mixer2_min, &mixer2_max);
	}

	mixer_elem_curr = mixer_elem1 ? mixer_elem1 : mixer_elem2;

	if (mixer_elem_curr)
	{
		if (mixer_elem1 && (real_volume1 = alsa_read_mixer_raw (mixer_elem1)) = -1)
		{
			volume1 = scale_volume1 (real_volume1);
		}
		else
		{
			mixer_elem1 = NULL;
			mixer_elem_curr = mixer_elem2;
		}

		if (mixer_elem2 && (real_volume2 = alsa_read_mixer_raw(mixer_elem2)) != -1)
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

/*static*/ void FooAlsaPlugin::shutdown ()
{
	int err;

	if (mixer_handle && (err = snd_mixer_close (mixer_handle)) < 0)
	{
		clog << "Can't close mixer: " snd_strerror (err);
	}
}

/*static*/ int FooAlsaPlugin::open (/*struct sound_params *sound_params*/)
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
			cerr << "Unknown sample format: " << sfmt_str(sound_params->fmt, fmt_name, sizeof(fmt_name));
			params.format = SND_PCM_FORMAT_UNKNOWN;
			return 0;
	}

	if ((err = snd_pcm_open(&handle, options_get_str ("AlsaDevice"), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0)
	{
		cerr << "Can't open audio: " << snd_strerror (err);
		return 0;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0)
	{
		cerr << "Can't initialize hardware parameters structure: " snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		cerr << "Can't set alsa access type: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, params.format)) < 0)
	{
		cerr << "Can't set sample format: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	params.rate = sound_params->rate;

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &params.rate, 0)) < 0)
	{
		cerr << "Can't set sample rate: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	clog << "Set rate to " << params.rate << endl;

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, sound_params->channels)) < 0)
	{
		cerr << "Can't set number of channels: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params_get_buffer_time_max (hw_params, &buffer_time, 0)) < 0)
	{
		cerr << "Can't get maximum buffer time: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if (buffer_time > BUFFER_MAX_USEC)
	{
		buffer_time = BUFFER_MAX_USEC;
	}
	period_time = buffer_time / 4;

	if ((err = snd_pcm_hw_params_set_period_time_near (handle, hw_params, &period_time, 0)) < 0)
	{
		cerr << "Can't set period time: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params_set_buffer_time_near (handle, hw_params, &buffer_time, 0)) < 0)
	{
		cerr << "Can't set buffer time: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0)
	{
		cerr << "Can't set audio parameters: " << snd_strerror (err);
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	snd_pcm_hw_params_get_period_size (hw_params, &chunk_frames, 0);
	snd_pcm_hw_params_get_buffer_size (hw_params, &buffer_frames);

	bytes_per_frame = sound_params->channels * sfmt_Bps (sound_params->fmt);

	clog << "Buffer time: " << buffer_frames * bytes_per_frame << endl;

	if (chunk_frames == buffer_frames)
	{
		cerr <<"Can't use period equal to buffer size (" << chunk_frames << "==" << buffer_frames;
		snd_pcm_hw_params_free (hw_params);
		return 0;
	}

	chunk_size = chunk_frames * bytes_per_frame;

	cout << "Chunk size: " << chunk_size << endl;

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare(handle)) < 0)
	{
		cerr << "Can't prepare audio interface for use: " << snd_strerror (err);
		return 0;
	}

	cout << "ALSA device initialized" << endl;

	params.channels = sound_params->channels;
	alsa_buf_fill = 0;
	return 1;
}

/*static*/ void FooAlsaPlugin::close ()
{
	assert (handle != NULL);

	/* play what remained in the buffer */
	if (alsa_buf_fill)
	{
		assert (alsa_buf_fill < chunk_size);

		// FIXME: why the last argument is multiplied by number of channels?
		snd_pcm_format_set_silence (params.format, alsa_buf + alsa_buf_fill, (chunk_size - alsa_buf_fill) / bytes_per_frame * params.channels);
		play_buf_chunks ();
	}

	params.format = 0;
	params.rate = 0;
	params.channels = 0;
	snd_pcm_close (handle);
	clog << "Alsa device closed" << endl;
	handle = NULL;
}

/*static*/ int FooAlsaPlugin::play (/*const char *buff, const size_t size*/)
{
	int to_write = size;
	int buf_pos = 0;

	assert (chunk_size > 0);

	cout << "Got " << (int) size << " bytes to play" << endl;
	while (to_write)
	{
		int to_copy = MIN((size_t)to_write, sizeof(alsa_buf) - (size_t)alsa_buf_fill);
		memcpy (alsa_buf + alsa_buf_fill, buff + buf_pos, to_copy);
		to_write -= to_copy;
		buf_pos += to_copy;
		alsa_buf_fill += to_copy;

		cout << "Copied " << to_copy << " bytes to alsa_buf (now is filled with " << alsa_buf_fill " bytes)" << endl;

		if (play_buf_chunks () < 0)
		{
			return -1;
		}
	}

	cout << "Played everything" << endl;

	return size;
}

/*static*/ void FooAlsaPlugin::setMixer (int vol)
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

		cout << "Setting vol to " << vol_alsa << endl;

		if ((err = snd_mixer_selem_set_playback_volume_all (mixer_elem_curr, vol_alsa)) < 0)
		{
			cerr << "Can't set mixer: " << snd_strerror (err);
		}
		else
		{
			*real_vol = vol_alsa;
		}
	}
}

/*static*/ int FooAlsaPlugin::getBuffFill ()
{
	if (handle)
	{
		int err;
		snd_pcm_sframes_t delay;

		if ((err = snd_pcm_delay(handle, &delay)) < 0)
		{
			clog << "snd_pcm_delay() failed: " << snd_strerror (err);
			return 0;
		}

		/* delay can be negative when underrun occur */
		return delay >= 0 ? delay * bytes_per_frame : 0;
	}

	return 0;
}

/*static*/ int FooAlsaPlugin::reset ()
{
	if (handle)
	{
		int err;

		if ((err = snd_pcm_drop(handle)) < 0)
		{
			cerr << "Can't reset the device: " << snd_strerror (err) << endl;
			return 0;
		}

		if ((err = snd_pcm_prepare(handle)) < 0)
		{
			cerr << "Can't prepare anfter reset: " << snd_strerror (err) << endl;
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

/*static*/ int FooAlsaPlugin::getRate ()
{
	return params.rate;
}

/*static*/ void FooAlsaPlugin::toggleMixerChannel ()
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

/*static*/ char *FooAlsaPlugin::getMixerChannelName ()
{
	if (mixer_elem_curr == mixer_elem1)
	{
		return xstrdup (options_get_str ("AlsaMixer"));
	}

	return xstrdup (options_get_str ("AlsaMixer2"));
}

