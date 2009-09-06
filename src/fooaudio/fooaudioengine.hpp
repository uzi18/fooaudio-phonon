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
	void clearQueue ();
	QList<QUrl> getQueue () {return queue;}

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;

	QList<QUrl> queue;

signals:
	void aboutToFinish ();
	void progress (qint64 time);

public slots:
	void prepareNextFile();
	void enqueueNextFile (QUrl path);
	void playFile (QUrl path);

	void setVolume (int vol);
};

#endif // _FOOAUDIOENGINE_HPP_
