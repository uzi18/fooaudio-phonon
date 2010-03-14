/*********************************************************************************
 * FooAudio
 * Copyright (C) 2009-2010  FooAudio Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************************/

#include "phononengine.h"

#include <QtDebug>
#include <QtPlugin>

#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/backendcapabilities.h>

namespace FooAudio
{
	const int DEFAUL_INTERVAL   = 10;

	class PhononEngine::PhononEnginePrivate
	{
	public:
		Phonon::MediaObject *mediaObject;
		Phonon::AudioOutput *audioOutput;
		Phonon::MediaObject *metaInformation;
	};

	PhononEngine::PhononEngine(QObject * parent)
		: AbstractAudioPlugin(parent)
	{
		d = new PhononEnginePrivate;

		d->audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
		d->mediaObject = new Phonon::MediaObject(this);
		d->metaInformation = new Phonon::MediaObject(this);

		Phonon::createPath(d->mediaObject, d->audioOutput);

		d->mediaObject->setTickInterval(DEFAUL_INTERVAL);

		connect(d->mediaObject, SIGNAL (tick(qint64)), this, SIGNAL (progress(qint64)));
		connect(d->mediaObject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
		connect(d->mediaObject, SIGNAL(metaDataChanged()), this, SLOT(newMetaData()));

	}

	PhononEngine::~PhononEngine()
	{
		delete d;
	}

	bool PhononEngine::isPlaying()
	{
		return d->mediaObject->state() == Phonon::PlayingState;
	}

	bool PhononEngine::isStopped()
	{
		Phonon::State state = d->mediaObject->state();
		return state == Phonon::StoppedState || state == Phonon::LoadingState;
	}

	bool PhononEngine::isPaused()
	{
		return d->mediaObject->state() == Phonon::PausedState;
	}

	bool PhononEngine::isMuted()
	{
		return d->audioOutput->isMuted();
	}

	void PhononEngine::setMuted(bool mute)
	{
		d->audioOutput->setMuted(mute);
	}

	qint64 PhononEngine::totalTime() const
	{
		return d->mediaObject->totalTime();
	}

	void PhononEngine::seek(const qint64 time)
	{
		d->mediaObject->seek(time);
	}

	void PhononEngine::stop()
	{
		d->mediaObject->stop();
	}

	void PhononEngine::play()
	{
		d->mediaObject->play();
	}

	void PhononEngine::pause()
	{
		d->mediaObject->pause();
	}

	void PhononEngine::clearQueue()
	{
		d->mediaObject->clearQueue();
	}

	void PhononEngine::enqueueNextFile(const QUrl file)
	{
		qDebug() << "PhononEngine::enqueueNextFile"
				<< "Next song: " << file.toString();

		if (!file.isEmpty())
		{
			d->mediaObject->enqueue(file.toString());
			emit willPlayNow(file);
		}
	}

	void PhononEngine::playFile(const QUrl file)
	{
		qDebug() << "PhononEngine::playfile";

		if (!file.isEmpty())
		{
			qDebug() << "PhononEngine::playFile: is not Empty: "
					<< file.toString();
			emit willPlayNow(file);
			d->mediaObject->stop();
			d->mediaObject->clearQueue();
			d->mediaObject->setCurrentSource(file.toString());
			d->mediaObject->play();
		}
		else
		{
			qDebug() << "FooPhononAudioEngine::playFile: is Empty";
			d->mediaObject->stop();
		}
	}

	void PhononEngine::setVolume(const int volume)
	{
		qreal v = volume / 100;
		d->audioOutput->setVolume(v);
	}

	QMultiMap<QString, QString> PhononEngine::metaData(const QUrl url)
	{
		d->metaInformation->setCurrentSource(url);
		return d->metaInformation->metaData();
	}

	QStringList PhononEngine::metaData(const QString key, const QUrl url)
	{
		d->metaInformation->setCurrentSource(url);
		return d->metaInformation->metaData(key);
	}

	QStringList PhononEngine::mimeTypes()
	{
		return Phonon::BackendCapabilities::availableMimeTypes();
	}

	void PhononEngine::newMetaData()
	{
		emit metaDataChanged(d->mediaObject->metaData());
	}
};

