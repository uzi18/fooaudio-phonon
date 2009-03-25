#include <alsa/asoundlib.h>
#include <iostream>

using namespace std;

#include "fooalsaplugin.hpp"

int FooAlsaPlugin::init(struct outputDriverCaps *caps)
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
		mixer_elem1 = alsa_init_mixer_channel(options_get_str("AlsaMixer"), &mixer1_min, &mixer1_max);
		mixer_elem2 = alsa_init_mixer_channel(options_get_str("AlsaMixer2"), &mixer2_min, &mixer2_max);
	}

	mixer_elem_curr = mixer_elem1 ? mixer_elem1 : mixer_elem2;

	if (mixer_elem_curr)
	{
		if (mixer_elem1 && (real_volume1 = alsa_read_mixer_raw(mixer_elem1)) != -1)
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

	return fill_capabilities (caps);
}

