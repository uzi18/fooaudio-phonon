#ifndef _FOOALSAPLUGIN_HPP_
#define _FOOALSAPLUGIN_HPP_

#include <QObject>
#include <alsa/asoundlib.h>

#include "../include/fooaudiostruct.hpp"
#include "../include/fooplugininterfaces.hpp"

class FooAlsaPlugin : public QObject, public FooAudioInterface
{
	Q_OBJECT
	Q_INTERFACES(FooAudioInterface)

public:
	FooAlsaPlugin ();

	int init(OutputDriverCaps *);
	void shutdown();
	int open(SoundParams *);
	void close();
	int play(const char *, const size_t);
	int readMixer();
	void setMixer(int);
	int getBuffFill();
	int reset();
	int getRate();
	void toggleMixerChannel();
	char *getMixerChannelName();

private:
	const unsigned int BUFFER_MAX_USEC;
	snd_pcm_t *handle;

	Params params;
/*	struct
	{
		unsigned channels;
		unsigned rate;
		snd_pcm_format_t format;
	} params = { 0, 0, SND_PCM_FORMAT_UNKNOWN };
*/
	int chunk_size;
	char alsa_buf[64 * 1024];
	int alsa_buf_fill;
	int bytes_per_frame;

	snd_mixer_t *mixer_handle;
	snd_mixer_elem_t *mixer_elem1;
	snd_mixer_elem_t *mixer_elem2;
	snd_mixer_elem_t *mixer_elem_curr;
	long mixer1_min, mixer1_max;
	long mixer2_min, mixer2_max;

/* Volume for first and second mixer in range 1-100 despite the actual device
 * resolution.
 */

	int volume1;
	int volume2;

/* Real volume setting as we last read them. */
	int real_volume1;
	int real_volume2;

/* Scale the mixer value to 0-100 range for first and second channel */
#define scale_volume1(v) ((v) - mixer1_min) * 100 / (mixer1_max - mixer1_min)
#define scale_volume2(v) ((v) - mixer2_min) * 100 / (mixer2_max - mixer2_min)

	int playBufChunks();
	int fillCapabilities (OutputDriverCaps *);
	int readMixerRaw(snd_mixer_elem_t *);
	void handleMixerEvents (snd_mixer_t *);
	snd_mixer_elem_t *initMixerChannel (const char *name, long *vol_min, long *vol_max);
};

#endif

