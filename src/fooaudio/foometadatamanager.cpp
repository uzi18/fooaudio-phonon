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

	qDebug() << "FooMetaDataManager: " << QUrl(CurrentTrack->file()).toString();
	QMultiMap<QString, QString> data = FooMainWindow::instance()->audioEngine()->metaData(CurrentTrack->file());
	if (!data.isEmpty())
	{
		// TODO: add some logic here
		CurrentTrack->setArtist(data.key("ARTIST"));
		CurrentTrack->setAlbum(data.key("ARLBUM"));
		CurrentTrack->setTitle(data.key("TITLE"));
		CurrentTrack->setYear(data.key("DATE"));
		// "GENRE"
		CurrentTrack->setTrack(data.key("TRACKNUMBER"));
		// "DESCRIPTION"
	}
	deleteTrack(CurrentTrack);
	CurrentTrack = 0;
}
