#include <QApplication>
#include <QPluginLoader>
#include <iostream>

using namespace std;

#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

FooAudioEngine::FooAudioEngine (FooMainWindow* fmw, QObject* parent) : QObject(parent)
{
	fooMainWindow = fmw;

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	connect (mediaObject, SIGNAL (aboutToFinish()), this, SLOT (enqueueNextFile()));
}

Phonon::MediaObject * FooAudioEngine::getMediaObject()
{
	return mediaObject;
}

QList<Phonon::MediaSource> * FooAudioEngine::getSources()
{
	return &sources;
}

void FooAudioEngine::enqueueNextFile()
{
	cout << "Kolejkowanie kolejnego utworu " << fooMainWindow->fooTabWidget->nextFile().toString().toStdString() << endl;
	mediaObject->enqueue(fooMainWindow->fooTabWidget->nextFile());
}
