#ifndef FOOPLAYLISTMANAGER_HPP
#define FOOPLAYLISTMANAGER_HPP

#include <QList>

class FooTrackList;

class FooPlaylistManager
{
	static FooPlaylistManager *Instance;

	QList<FooTrackList *> *playlists;

	void init();

public:
	FooPlaylistManager();
	~FooPlaylistManager();

	static FooPlaylistManager* instance();

	void addPlaylist(FooTrackList *);
	void deletePlaylist(FooTrackList *);
	void deletePlaylist(int);
	QList<FooTrackList *>* getPlaylists();
};

#endif // FOOPLAYLISTMANAGER_HPP
