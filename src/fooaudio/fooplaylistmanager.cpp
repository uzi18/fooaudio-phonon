#include "fooplaylistmanager.hpp"
#include "footracklist.hpp"

FooPlaylistManager *FooPlaylistManager::Instance = 0;

FooPlaylistManager::FooPlaylistManager()
{
	Instance = this;
	init();
}

FooPlaylistManager::~FooPlaylistManager()
{
}

void FooPlaylistManager::init()
{
	playlists = new QList<FooTrackList *>;
}

FooPlaylistManager* FooPlaylistManager::instance()
{
	if (0 == Instance)
	{
		Instance = new FooPlaylistManager();
		Instance->init();
	}

	return Instance;
}

void FooPlaylistManager::addPlaylist(FooTrackList *fooTrackList)
{
	playlists->append(fooTrackList);
}

void FooPlaylistManager::deletePlaylist(int i)
{
	if (i < 0 || i >= playlists->size())
	{
		return;
	}
	else
	{
		playlists->removeAt(i);
	}
}

void FooPlaylistManager::deletePlaylist(FooTrackList *fooTrackList)
{
	int i = playlists->indexOf(fooTrackList);

	if (i < 0)
	{
		return;
	}
	else
	{
		playlists->removeAt(i);
	}
}

QList<FooTrackList *>* FooPlaylistManager::getPlaylists()
{
	return playlists;
}

