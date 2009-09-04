#include <QApplication>
#include <QPluginLoader>
#include <iostream>

using namespace std;

#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

FooAudioEngine::FooAudioEngine (QObject* parent) : QObject(parent)
{
	cout << "FooAudioEngine" << endl << flush;
//	fooMainWindow = fmw;

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	connect (mediaObject, SIGNAL (aboutToFinish()), this, SLOT (enqueueNextFile()));
}

Phonon::MediaObject * FooAudioEngine::getMediaObject()
{
	cout << "FooAudioEngine::getMediaObject" << endl << flush;
	return mediaObject;
}

void FooAudioEngine::enqueueNextFile()
{
	cout << "FooAudioEngine::enqueueNextFile" << endl << flush;
	if (fooMainWindow == NULL)
		cout << "fooMainWindow jest null" << endl << flush;
	else
		cout << "fooMainWindow nie jest null" << endl << flush;

	if (fooMainWindow->fooTabWidget == NULL)
		cout << "fooMainWindow->fooTabWidget jest null" << endl << flush;
	else
		cout << "fooMainWindow->fooTabWidget nie jest null" << endl << flush;

	cout << "Kolejkowanie kolejnego utworu " << fooMainWindow->fooTabWidget->nextFile().toString().toStdString() << endl << flush;
	QUrl foo = fooMainWindow->fooTabWidget->nextFile();
	cout << "Kolejna piosenka: " << foo.toString().toStdString() << endl << flush;
	mediaObject->enqueue(foo.toLocalFile());
}

void FooAudioEngine::setFooMainWindow(FooMainWindow *fmw)
{
	this->fooMainWindow = fmw;
	// to move progress bar
	connect (mediaObject, SIGNAL (tick(qint64)), this, SLOT (progress(qint64)));
	// this is needed - default = 0 => no ticks
	mediaObject->setTickInterval(10);
}

void FooAudioEngine::progress(qint64 time)
{
	int progress = (int) (time*MAX_PROGRESS/mediaObject->totalTime());
	if (progress >= 0)
		this->fooMainWindow->trackSlider->setValue(progress);
}
