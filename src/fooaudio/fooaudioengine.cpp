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

	slider_pos = -1;

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	connect (mediaObject, SIGNAL (aboutToFinish()), this, SLOT (enqueueNextFile()));

	repeat = true;
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

	cout << "Kolejkowanie kolejnego utworu " << endl << flush;
	QUrl foo = fooMainWindow->fooTabWidget->nextFile(repeat);
	cout << "Kolejna piosenka: " << foo.toString().toStdString() << endl << flush;
	if (!foo.isEmpty())
	{
		mediaObject->enqueue(foo.toLocalFile());
	}
}

void FooAudioEngine::setFooMainWindow(FooMainWindow *fmw)
{
	this->fooMainWindow = fmw;
	// to move progress bar
	connect (mediaObject, SIGNAL (tick(qint64)), this, SLOT (progress(qint64)));
	connect (this->fooMainWindow->trackSlider, SIGNAL (sliderMoved(int)), this, SLOT (seek(int)));
	connect (this->fooMainWindow->trackSlider, SIGNAL (sliderReleased()), this, SLOT (sliderReleased()));
	// this is needed - default = 0 => no ticks
	mediaObject->setTickInterval(10);
}

void FooAudioEngine::progress(qint64 time)
{
	int progress = (int) (time*MAX_PROGRESS/mediaObject->totalTime());
	QSlider *slider = this->fooMainWindow->trackSlider;
	if (progress >= 0 && !slider->isSliderDown())
		slider->setValue(progress);
}
void FooAudioEngine::seek(int value)
{
	if (slider_pos != value)
		slider_pos = value;
}

void FooAudioEngine::sliderReleased()
{
	if (slider_pos == -1)
		return;
	// think to check if value is valid for seek
	mediaObject->seek(mediaObject->totalTime()*slider_pos/MAX_PROGRESS);
	slider_pos = -1;
}
