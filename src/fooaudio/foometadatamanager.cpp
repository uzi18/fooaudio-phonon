#include <QDebug>
#include <QTimer>

#include "foomainwindow.hpp"
#include "abstractaudioplugin.h"
#include "foometadatamanager.hpp"
#include "footrack.hpp"

FooMetaDataManager *FooMetaDataManager::Instance = 0;

FooMetaDataManager::FooMetaDataManager() : CurrentTrack(0)
{
	Refresh = new QTimer(this);
	connect(Refresh, SIGNAL(timeout()), this, SLOT(timeout()));
}

FooMetaDataManager::~FooMetaDataManager()
{
	Refresh->stop();
	delete Refresh;
}

void FooMetaDataManager::init()
{
	qDebug() << "FooMetaDataManager: INIT";
	timeout();
}

FooMetaDataManager* FooMetaDataManager::instance()
{
	if (0 == Instance)
	{
		Instance = new FooMetaDataManager();
		Instance->init();
	}

	return Instance;
}

void FooMetaDataManager::addTrack(FooTrack *track)
{
	if (!track || Tracks.contains(track))
		return;

	qDebug() << "FooMetaDataManager: ADD" << track->file();
	Tracks.append(track);
}

void FooMetaDataManager::deleteTrack(FooTrack *track)
{
	if (Tracks.contains(track))
		Tracks.removeAll(track);
}

QList<FooTrack *> FooMetaDataManager::tracks()
{
	return Tracks;
}

void FooMetaDataManager::clear()
{
	qDebug() << "FooMetaDataManager: clear()";
	Tracks.clear();
}

void FooMetaDataManager::timeout()
{
	qDebug() << "FooMetaDataManager: timeout()";
	Refresh->start(500);

	if (Tracks.isEmpty() || !FooMainWindow::instance()->audioEngine())
		return;

	// if unsuccesfull so move to end
	if (CurrentTrack)
	{
		deleteTrack(CurrentTrack);
		addTrack(CurrentTrack);
		CurrentTrack = 0;
		return;
	}

	CurrentTrack = Tracks.first();

	qDebug() << "FooMetaDataManager: " << CurrentTrack->file().toString();
	QMultiMap<QString, QString> data = FooMainWindow::instance()->audioEngine()->metaData(CurrentTrack->file());
	if (!data.isEmpty())
	{
		QString value;
		foreach(QString key, data.keys())
		{
			QList<QString> values = data.values(key);
			value = values.takeFirst();
			//for (int i = 0; i < values.size(); ++i)
			//qDebug() << "PE:" << key << values.at(i);
			qDebug() << "PE:" << key << value;

			// TODO: add some logic here
			if (key == "ARTIST")
				CurrentTrack->setArtist(value);

			else if (key == "ALBUM")
				CurrentTrack->setAlbum(value);

			else if (key == "TITLE")
				CurrentTrack->setTitle(value);

			else if (key == "DATE")
				CurrentTrack->setYear(value);

			else if (key == "TRACKNUMBER")
				CurrentTrack->setTrack(value);

			// "GENRE"
			// "DESCRIPTION"
		}
	}
	deleteTrack(CurrentTrack);
	CurrentTrack = 0;
}
