#include <QApplication>
#include <QPluginLoader>
#include <iostream>

using namespace std;

#include "fooaudioengine.hpp"

FooAudioEngine::FooAudioEngine ()
{
	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);
}

Phonon::MediaObject * FooAudioEngine::getMediaObject()
{
	return mediaObject;
}

QList<Phonon::MediaSource> * FooAudioEngine::getSources()
{
	return &sources;
}
