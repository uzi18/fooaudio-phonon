#ifndef FOOPLAYLISTMANAGER_HPP
#define FOOPLAYLISTMANAGER_HPP

#include <QList>
#include <QObject>

class FooTrackList;

class FooPlaylistManager : public QObject
{
  
	Q_OBJECT

	static FooPlaylistManager *Instance;

	QList<FooTrackList *> Playlists;
	// one that played
	FooTrackList * CurrentPlaylist;
	// one that selected by tab
	FooTrackList * CurrentlySelected;

	void init();

public:
	FooPlaylistManager();
	~FooPlaylistManager();

	static FooPlaylistManager* instance();

	void addPlaylist(FooTrackList *playlist);
	void deletePlaylist(FooTrackList *playlist);
	void deletePlaylist(int);
	QList<FooTrackList*> playlists();
	
	void setCurrentPlaylist(FooTrackList* playlist) { CurrentPlaylist = playlist; }
	FooTrackList* currentPlaylist() { return CurrentPlaylist;}

	FooTrackList* currentlySelected() { return CurrentlySelected;}

signals:
	void playlistAdded(FooTrackList *playlist);
	void playlistRemoved(FooTrackList *playlist);
};

#endif // FOOPLAYLISTMANAGER_HPP
