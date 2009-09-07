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

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	// this is needed - default = 0 => no ticks
	mediaObject->setTickInterval(10);

	connect(mediaObject, SIGNAL (tick(qint64)), this, SIGNAL (progress(qint64)));
	connect (mediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
}

Phonon::MediaObject * FooAudioEngine::getMediaObject ()
{
	return mediaObject;
}

Phonon::AudioOutput * FooAudioEngine::getAudioOutput ()
{
	return audioOutput;
}

bool FooAudioEngine::isPlaying()
{
	return mediaObject->state() == Phonon::PlayingState;
}

bool FooAudioEngine::isStopped()
{
	return mediaObject->state() == (Phonon::StoppedState || Phonon::LoadingState);
}

bool FooAudioEngine::isMuted()
{
	return audioOutput->isMuted();
}

void FooAudioEngine::setMuted(bool mute)
{
	audioOutput->setMuted(mute);
}

void FooAudioEngine::enqueueNextFile (QUrl path)
{
	cerr << "FooAudioEngine::enqueueNextFile" << endl;

	cerr << "Kolejna piosenka: " << path.toString().toStdString() << endl;
	if (!path.isEmpty())
	{
		mediaObject->enqueue(path.toLocalFile());
	}
}

void FooAudioEngine::playFile (QUrl path)
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

void FooAudioEngine::setVolume (int vol)
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
