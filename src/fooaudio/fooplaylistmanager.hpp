#ifndef FOOPLAYLISTMANAGER_HPP
#define FOOPLAYLISTMANAGER_HPP

#include <QList>
#include <QObject>
#include <QUrl>

namespace PlayOrder
{
	enum PlayOrder
	{
		repeatPlaylist,
		repeatTrack,
		shuffleTracks,
		shuffleAlbums,
		shuffleFolders,
		defaultOrder,
		randomOrder
	};
}

class FooTrackList;

class FooPlaylistManager : public QObject
{
  
	Q_OBJECT

	static FooPlaylistManager *Instance;

	QList<FooTrackList *> Playlists;
	// one that played
	FooTrackList * CurrentPlaylist;
	int CurrentTruck;
	// one that selected by tab
	FooTrackList * CurrentlySelected;

	PlayOrder::PlayOrder Order;
	
	QList<QUrl> Queue;
	QList<QUrl> PrevQueue;
	int prevqueueindex;

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
	void useSelectedPlaylist() {CurrentPlaylist = CurrentlySelected; CurrentTruck = 0;}

	void setOrder(PlayOrder::PlayOrder order) { Order = order;}
	PlayOrder::PlayOrder order() { return Order;}

	QUrl getNextFile();
	QUrl nextFile(bool repeat, bool follow = true);
	QUrl previousFile(bool repeat, bool follow = true);

	void clearQueue ();
	void addFileToQueue (QUrl file);
	void removeFileFromQueue (QUrl file);
	QList<QUrl> getQueue () {return Queue;}
	void addToPrevQueue (QUrl path);
	
	QUrl randomTrack();

signals:
	void playlistAdded(FooTrackList *playlist);
	void playlistRemoved(FooTrackList *playlist);
	void currentPlaylistChanged(FooTrackList *playlist);

public slots:
	void currentTabChanged(int tab);
};

#endif // FOOPLAYLISTMANAGER_HPP
