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

	Phonon::MediaObject * getMediaObject();
	Phonon::AudioOutput * getAudioOutput();

	void setFooMainWindow(FooMainWindow *);

private:
	FooMainWindow *fooMainWindow;

	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	int slider_pos;

	bool repeat;

signals:
	void enqueueNextFile();

public slots:
	void aboutToFinish();
	void enqueueNextFile(QUrl);
	void playFile(QUrl);

	void progress(qint64 time);
	void seek(int value);
	void sliderReleased();

	void setVolume(int);
};

#endif // _FOOAUDIOENGINE_HPP_
