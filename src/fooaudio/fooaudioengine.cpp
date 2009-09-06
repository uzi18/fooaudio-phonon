#include <QApplication>
#include <QPluginLoader>
#include <QUrl>
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

	// this is needed - default = 0 => no ticks
	mediaObject->setTickInterval(10);

	connect(mediaObject, SIGNAL (tick(qint64)), this, SLOT (progress(qint64)));
	connect (mediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
}

Phonon::MediaObject * FooAudioEngine::getMediaObject()
{
	cerr << "FooAudioEngine::getMediaObject" << endl;
	return mediaObject;
}

Phonon::AudioOutput * FooAudioEngine::getAudioOutput()
{
	cerr << "FooAudioEngine::getAudioOutput" << endl;
	return audioOutput;
}

bool FooAudioEngine::isPlaying()
{
	return mediaObject->state() == Phonon::PlayingState;
}

void FooAudioEngine::enqueueNextFile(QUrl path)
{
	cerr << "FooAudioEngine::enqueueNextFile" << endl;

	cerr << "Kolejna piosenka: " << path.toString().toStdString() << endl;
	if (!path.isEmpty())
	{
		mediaObject->enqueue(path.toLocalFile());
	}
}

void FooAudioEngine::playFile(QUrl path)
{
	cerr << "FooAudioEngine::playFile" << endl;

	if (!path.isEmpty())
	{
		cerr << "FooAudioEngine::playFile: is not Empty: " << path.toLocalFile().toStdString() << endl;
		mediaObject->stop();
		mediaObject->clearQueue();
		mediaObject->setCurrentSource(path.toLocalFile());
		mediaObject->play();
	}
	else
	{
		cerr << "FooAudioEngine::playFile: is Empty" << endl;
		mediaObject->stop();
	}
}

void FooAudioEngine::setFooMainWindow(FooMainWindow *fmw)
{
	this->fooMainWindow = fmw;
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

void FooAudioEngine::setVolume(int vol)
{
	qreal v = vol;
	qreal d = v / 100;

	audioOutput->setVolume(d);
}

void FooAudioEngine::stop()
{
	mediaObject->stop();
}

void FooAudioEngine::play()
{
	mediaObject->play();
}

void FooAudioEngine::pause()
{
	mediaObject->pause();
}

void FooAudioEngine::clearQueue()
{
	mediaObject->clearQueue();
}
