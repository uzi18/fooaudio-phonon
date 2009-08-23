#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QList>
#include <QDir>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include "fooplaylistwidget.hpp"

class FooAudioEngine : public QObject
{
public:
	FooAudioEngine ();

	Phonon::MediaObject * getMediaObject();
	QList<Phonon::MediaSource> * getSources();

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	QList<Phonon::MediaSource> sources;
};

#endif // _FOOAUDIOENGINE_HPP_

