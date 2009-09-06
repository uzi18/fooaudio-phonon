#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QList>
#include <QDir>
#include <QUrl>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include "fooplaylistwidget.hpp"

class FooMainWindow;

class FooAudioEngine : public QObject
{
	Q_OBJECT

public:
	FooAudioEngine (QObject * parent = 0);

	Phonon::MediaObject * getMediaObject ();
	Phonon::AudioOutput * getAudioOutput ();

	void addFileToQueue (QUrl file);
	void removeFileFromQueue (QUrl file);
	void clearMusicQueue ();
	QList<QUrl> getQueue () {return queue;}

	bool isPlaying();
	bool isStopped();
	bool isMuted();
	void setMuted(bool);

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;

	QList<QUrl> queue;

signals:
	void aboutToFinish ();
	void progress (qint64 time);

public slots:
	void stop();
	void play();
	void pause();
	void clearQueue();
	void prepareNextFile();
	void enqueueNextFile(QUrl path);
	void playFile (QUrl path);

	void setVolume (int vol);
};

#endif // _FOOAUDIOENGINE_HPP_
