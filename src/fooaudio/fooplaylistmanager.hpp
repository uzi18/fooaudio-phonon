#ifndef FOOPLAYLISTMANAGER_HPP
#define FOOPLAYLISTMANAGER_HPP

#include <QList>
#include <QObject>

class FooTrackList;

class FooPlaylistManager : public QObject
{
  
	Q_OBJECT

	static FooPlaylistManager *Instance;

	QList<FooTrackList *> playlists;

	void init();

public:
	FooPlaylistManager();
	~FooPlaylistManager();

	static FooPlaylistManager* instance();

	void addPlaylist(FooTrackList *playlist);
	void deletePlaylist(FooTrackList *playlist);
	void deletePlaylist(int);
	QList<FooTrackList*> getPlaylists();
	
signals:
	void playlistAdded(FooTrackList *playlist);
	void playlistRemoved(FooTrackList *playlist);
};

#endif // FOOPLAYLISTMANAGER_HPP
