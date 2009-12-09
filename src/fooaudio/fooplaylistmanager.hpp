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
	FooTrackList* playlist(int number) { return Playlists.at(number);}
	int playlist(FooTrackList* playlist) { return Playlists.indexOf(playlist);}

	void setCurrentPlaylist(FooTrackList* playlist);
	FooTrackList* currentPlaylist() { return CurrentPlaylist;}

	FooTrackList* currentlySelected() { return CurrentlySelected;}

signals:
	void playlistAdded(FooTrackList *playlist);
	void playlistRemoved(FooTrackList *playlist);
	void currentPlaylistChanged(FooTrackList *playlist);

public slots:
	void currentTabChanged(int tab);
};

#endif // FOOPLAYLISTMANAGER_HPP
