#include <QDebug>

#include "fooplaylistmanager.hpp"
#include "footracklist.hpp"

FooPlaylistManager *FooPlaylistManager::Instance = 0;

FooPlaylistManager::FooPlaylistManager() : CurrentPlaylist(0), CurrentlySelected(0)
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

	Playlists.append(playlist);
	emit playlistAdded(playlist);
	
	//set Current playlist as first added
	if (CurrentPlaylist = 0)
		CurrentPlaylist = playlist;
}

void FooPlaylistManager::deletePlaylist(int i)
{
	if (i < 0 || i >= Playlists.size())
	{
		return;
	}
	else
	{
		Playlists.removeAt(i);
	}
}

void FooPlaylistManager::deletePlaylist(FooTrackList *playlist)
{
	int i = Playlists.indexOf(playlist);

	if (i < 0)
	{
		return;
	}
	else
	{
		Playlists.removeAt(i);
	}
	
	emit playlistRemoved(playlist);
	delete playlist;
	playlist = 0;
	
}

void FooPlaylistManager::setCurrentPlaylist(FooTrackList* playlist)
{
	CurrentPlaylist = playlist;
	emit currentPlaylistChanged(playlist);
}

QList<FooTrackList *> FooPlaylistManager::playlists()
{
	return Playlists;
}

