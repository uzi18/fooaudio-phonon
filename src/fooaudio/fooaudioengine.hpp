#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QList>
#include <QDir>
#include <QUrl>
#include "fooplaylistwidget.hpp"

namespace Phonon {
	class MediaObject;
	class AudioOutput;
}

class FooMainWindow;

class FooAudioEngine : public QObject
{
	Q_OBJECT

public:
	FooAudioEngine (QObject * parent = 0);

	bool isPlaying();
	bool isStopped();
	bool isPaused();
	bool isMuted();
	void setMuted(bool);
	qint64 totalTime();
	void seek (qint64 time);

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;

signals:
	void aboutToFinish ();
	void progress (qint64 time);

public slots:
	void stop();
	void play();
	void pause();
	void clearQueue();
	void enqueueNextFile(QUrl path);
	void playFile (QUrl path);

	void setVolume (int vol);
};

#endif // _FOOAUDIOENGINE_HPP_
