#ifndef _FOOPHONONAUDIOENGINE_HPP_
#define _FOOPHONONAUDIOENGINE_HPP_

#include "fooaudioengineplugin.hpp"

namespace Phonon {
	class MediaObject;
	class AudioOutput;
}
class QUrl;

class FooPhononAudioEngine : public FooAudioEnginePlugin
{

public:
	FooPhononAudioEngine (QObject * parent = 0);

virtual	bool isPlaying();
virtual	bool isStopped();
virtual	bool isPaused();
virtual	bool isMuted();
virtual	void setMuted(bool);
virtual	qint64 totalTime();
virtual	void seek (qint64 time);

public slots:
virtual	void stop();
virtual	void play();
virtual	void pause();
virtual	void clearQueue();
virtual	void enqueueNextFile(QUrl path);
virtual	void playFile (QUrl path);

virtual	void setVolume (int vol);

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
};

#endif // _FOOPHONONAUDIOENGINE_HPP_
