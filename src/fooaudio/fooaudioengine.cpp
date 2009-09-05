#include <QApplication>
#include <QPluginLoader>
#include <iostream>

using namespace std;

#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

FooAudioEngine::FooAudioEngine (QObject* parent) : QObject(parent)
{
	cerr << "FooAudioEngine" << endl;

	slider_pos = -1;

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	connect (mediaObject, SIGNAL (aboutToFinish()), this, SLOT (enqueueNextFile()));

	repeat = true;
}

Phonon::MediaObject * FooAudioEngine::getMediaObject()
{
	cerr << "FooAudioEngine::getMediaObject" << endl;
	return mediaObject;
}

void FooAudioEngine::enqueueNextFile()
{
	cerr << "FooAudioEngine::enqueueNextFile" << endl;
	if (fooMainWindow == NULL)
		cerr << "fooMainWindow jest null" << endl;
	else
		cerr << "fooMainWindow nie jest null" << endl;

	if (fooMainWindow->fooTabWidget == NULL)
		cerr << "fooMainWindow->fooTabWidget jest null" << endl;
	else
		cerr << "fooMainWindow->fooTabWidget nie jest null" << endl;

	cerr << "Kolejkowanie kolejnego utworu ";
	QUrl foo = fooMainWindow->fooTabWidget->nextFile(repeat);
	cerr << "Kolejna piosenka: " << foo.toString().toStdString() << endl;
	if (!foo.isEmpty())
	{
		mediaObject->enqueue(foo.toLocalFile());
	}
}

void FooAudioEngine::nextFile()
{
	cerr << "FooAudioEngine::nextFile" << endl;
	QUrl nextFile = fooMainWindow->fooTabWidget->nextFile(repeat);

	if (!nextFile.isEmpty())
	{
		cerr << "FooAudioEngine::nextFile: is not Empty: " << nextFile.toLocalFile().toStdString() << endl;
		mediaObject->stop();
		mediaObject->clearQueue();
		mediaObject->setCurrentSource(nextFile.toLocalFile());
		mediaObject->play();
	}
	else
	{
		cerr << "FooAudioEngine::nextFile: is Empty" << endl;
		mediaObject->stop();
	}
}

void FooAudioEngine::previousFile()
{
	cerr << "FooAudioEngine::previousFile" << endl;
	QUrl previousFile = fooMainWindow->fooTabWidget->previousFile(repeat);

	if (!previousFile.isEmpty())
	{
		cerr << "FooAudioEngine::previousFile: is not Empty: " << previousFile.toLocalFile().toStdString() << endl;
		mediaObject->stop();
		mediaObject->clearQueue();
		mediaObject->setCurrentSource(previousFile.toLocalFile());
		mediaObject->play();
	}
	else
	{
		cerr << "FooAudioEngine::previousFile: is Empty" << endl;
		mediaObject->stop();
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
	int progress = (int) (time*fooMainWindow->getMaxProgress()/mediaObject->totalTime());
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
	mediaObject->seek(mediaObject->totalTime()*slider_pos/fooMainWindow->getMaxProgress());
	slider_pos = -1;
}
