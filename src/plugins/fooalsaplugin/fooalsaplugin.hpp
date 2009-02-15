#include <alsa/asoundlib.h>
#include "../fooplugininterfaces.hpp"

#define scale_volume1(v) ((v) - mixer1_min) * 100 / (mixer1_max - mixer1_min)
#define scale_volume2(v) ((v) - mixer2_min) * 100 / (mixer2_max - mixer2_min)

class FooAlsaPlugin : public FooAudioInterface
{
	Q_OBJECT
	Q_INTERFACES (FooAudioInterface)

public:
	int init (/*struct output_driver_caps *caps*/);
	void shutdown ();
	int open (/*struct sound_params *sound_params*/);
	void close ();
	int play (/*const char *buff, const size_t size*/);
	int readMixer ();
	void setMixer (int vol);
	int getBuffFill ();
	int reset ();
	int getRate ();
	void toggleMixerChannel ();
	char *getMixerChannelName ();

private:
	const int BUFFER_MAX_USEC  = 300000;

	snd_pcm_t *handle = NULL;

	struct
	{
		unsigned channels;
		unsigned rate;
		snd_pcm_format_t format;
	} params = { 0, 0, SND_PCM_FORMAT_UNKNOWN };

	int chunk_size = -1; /* in frames */ 
	char alsa_buf[512 * 1024];
	int alsa_buf_fill = 0;
	int bytes_per_frame;

	snd_mixer_t *mixer_handle = NULL;
	snd_mixer_elem_t *mixer_elem1 = NULL;
	snd_mixer_elem_t *mixer_elem2 = NULL;
	snd_mixer_elem_t *mixer_elem_curr = NULL;
	long mixer1_min = -1, mixer1_max = -1;
	long mixer2_min = -1, mixer2_max = -1;

	// Volume for first and second mixer in range 1-100 despite the actual device resolution.
	int volume1 = -1;
	int volume2 = -1;

	// Real volume setting as we last read them.
	int real_volume1 = -1;
	int real_volume2 = -1;

	// Scale the mixer value to 0-100 range for first and second channel
//#define scale_volume1(v) ((v) - mixer1_min) * 100 / (mixer1_max - mixer1_min)
//#define scale_volume2(v) ((v) - mixer2_min) * 100 / (mixer2_max - mixer2_min)

	int fillCapabilities (struct output_driver_caps *);
	void handleMixerEvents (snd_mixer_t *);
	int alsaReadMixerRaw (snd_mixer_elem_t *);
	snd_mixer_elem_t *InitMixerChannel (const char *, long *, long *);
	int playBufChunks ();

};

