#include <QDebug>
#include <QTimer>

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
}
