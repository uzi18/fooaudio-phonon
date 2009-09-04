#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QList>
#include <QDir>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include "fooplaylistwidget.hpp"

class FooMainWindow;

class FooAudioEngine : public QObject
{
	Q_OBJECT

public:
	FooAudioEngine (QObject *);

	Phonon::MediaObject * getMediaObject();

	void setFooMainWindow(FooMainWindow *);

private:
	FooMainWindow *fooMainWindow;

	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;

	bool repeat;

public slots:
	void enqueueNextFile();
	void progress(qint64 time);
	void seek(int value);
};

#endif // _FOOAUDIOENGINE_HPP_
