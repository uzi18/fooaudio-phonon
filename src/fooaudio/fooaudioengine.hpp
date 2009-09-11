#ifndef _FOOPHONONAUDIOENGINE_HPP_
#define _FOOPHONONAUDIOENGINE_HPP_

#include "fooaudioengineplugin.hpp"
#include <QMultiMap>
#include <QStringList>

namespace Phonon {
	class MediaObject;
	class AudioOutput;
}
class QUrl;

class FooPhononAudioEngine : public FooAudioEnginePlugin
{
	Q_OBJECT

public:
	FooPhononAudioEngine (QObject * parent = 0);

virtual	bool isPlaying();
virtual	bool isStopped();
virtual	bool isPaused();
virtual	bool isMuted();
virtual	void setMuted(bool);
virtual	qint64 totalTime();
virtual	void seek(qint64 time);

public slots:
virtual	void stop();
virtual	void play();
virtual	void pause();
virtual	void clearQueue();
virtual	void enqueueNextFile(QUrl);
virtual	void playFile(QUrl);

virtual	void setVolume(int);

virtual void metaData(QUrl);
virtual void metaData(const QString &, QUrl);

signals:
void metaData(QMultiMap<QString, QString>);
void metaData(QStringList);

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;

	Phonon::MediaObject *metaInformation;
};

#endif // _FOOPHONONAUDIOENGINE_HPP_
