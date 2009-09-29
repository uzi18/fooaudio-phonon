#include <QApplication>
#include <QPluginLoader>

#include <Phonon/AudiOutput>
#include <Phonon/MediaObject>
#include <Phonon/BackendCapabilities>

#include <QtDebug>

#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

FooPhononAudioEngine::FooPhononAudioEngine (QObject* parent) : FooAudioEnginePlugin(parent)
{
        qDebug() <<  "FooPhononAudioEngine";

	audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	mediaObject = new Phonon::MediaObject(this);
	metaInformation = new Phonon::MediaObject(this);

	Phonon::createPath(mediaObject, audioOutput);

	// this is needed - default = 0 => no ticks
	mediaObject->setTickInterval(10);

	connect(mediaObject, SIGNAL (tick(qint64)), this, SIGNAL (progress(qint64)));
	connect(mediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
}

bool FooPhononAudioEngine::isPlaying()
{
	return mediaObject->state() == Phonon::PlayingState;
}

bool FooPhononAudioEngine::isPaused()
{
	return mediaObject->state() == Phonon::PausedState;
}

bool FooPhononAudioEngine::isStopped()
{
	Phonon::State state = mediaObject->state();
	return state == Phonon::StoppedState || state == Phonon::LoadingState;
}

bool FooPhononAudioEngine::isMuted()
{
	return audioOutput->isMuted();
}

void FooPhononAudioEngine::setMuted(bool mute)
{
	audioOutput->setMuted(mute);
}

qint64 FooPhononAudioEngine::totalTime()
{
	return mediaObject->totalTime();
}

void FooPhononAudioEngine::seek (qint64 time)
{
	mediaObject->seek(time);
}

void FooPhononAudioEngine::enqueueNextFile (QUrl path)
{
        qDebug() <<  "FooPhononAudioEngine::enqueueNextFile";

        qDebug() <<  "Kolejna piosenka: " << path.toString();
	if (path.isEmpty())
		return;

	mediaObject->enqueue(path.toString());
	emit willPlayNow (path);
}

void FooPhononAudioEngine::playFile (QUrl path)
{
        qDebug() <<  "FooPhononAudioEngine::playFile";

	if (!path.isEmpty())
		  {
                                         qDebug() <<  "FooPhononAudioEngine::playFile: is not Empty: " << path.toString();
					 emit willPlayNow (path);
		mediaObject->stop();
		mediaObject->clearQueue();
					 mediaObject->setCurrentSource(path.toString());
		mediaObject->play();
	}
	else
	{
                qDebug() <<  "FooPhononAudioEngine::playFile: is Empty";
		mediaObject->stop();
	}
}

void FooPhononAudioEngine::setVolume (int vol)
{
	qreal v = vol;
	qreal d = v / 100;

	audioOutput->setVolume(d);
}

void FooPhononAudioEngine::stop()
{
	mediaObject->stop();
}

void FooPhononAudioEngine::play()
{
	mediaObject->play();
}

void FooPhononAudioEngine::pause()
{
	mediaObject->pause();
}

void FooPhononAudioEngine::clearQueue()
{
	mediaObject->clearQueue();
}

void FooPhononAudioEngine::metaData(QUrl url)
{
	metaInformation->setCurrentSource(url);
	emit metaData(metaInformation->metaData());
}

void FooPhononAudioEngine::metaData (const QString &key, const QUrl url)
{
	metaInformation->setCurrentSource(url);
	emit metaData(metaInformation->metaData(key));
}

void FooPhononAudioEngine::mimeTypes()
{
	emit mimeTypes(Phonon::BackendCapabilities::availableMimeTypes());
}
