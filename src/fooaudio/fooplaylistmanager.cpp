#include <QDebug>

#include "fooplaylistmanager.hpp"
#include "footracklist.hpp"

FooPlaylistManager *FooPlaylistManager::Instance = 0;

FooPlaylistManager::FooPlaylistManager()
{
}

FooPlaylistManager::~FooPlaylistManager()
{
}

void FooPlaylistManager::init()
{
	qDebug() << "MANAGER INIT";
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

void FooPlaylistManager::addPlaylist(FooTrackList *playlist)
{
	qDebug() << "MANAGER ADD" << playlist->name();

	playlists.append(playlist);
	emit playlistAdded(playlist);
}

void FooPlaylistManager::deletePlaylist(int i)
{
	if (i < 0 || i >= playlists.size())
	{
		return;
	}
	else
	{
		playlists.removeAt(i);
	}
}

void FooPlaylistManager::deletePlaylist(FooTrackList *playlist)
{
	int i = playlists.indexOf(playlist);

	if (i < 0)
	{
		return;
	}
	else
	{
		playlists.removeAt(i);
	}
	
	emit playlistRemoved(playlist);
	delete playlist;
	playlist = 0;
	
}

QList<FooTrackList *> FooPlaylistManager::getPlaylists()
{
	return playlists;
}

