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
	QList<Phonon::MediaSource> * getSources();

	void setFooMainWindow(FooMainWindow *);

private:
	FooMainWindow *fooMainWindow;

	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	QList<Phonon::MediaSource> sources;

public slots:
	void enqueueNextFile();
};

#endif // _FOOAUDIOENGINE_HPP_
