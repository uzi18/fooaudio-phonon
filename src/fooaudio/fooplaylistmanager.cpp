#include <QDebug>

#include "fooplaylistmanager.hpp"
#include "footracklist.hpp"

FooPlaylistManager *FooPlaylistManager::Instance = 0;

FooPlaylistManager::FooPlaylistManager() : CurrentPlaylist(0), CurrentlySelected(0), CurrentTruck(0)
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

void FooPlaylistManager::currentTabChanged(int tab)
{
	CurrentlySelected = Playlists.at(tab);
}

QList<FooTrackList *> FooPlaylistManager::playlists()
{
	return Playlists;
}



QUrl FooPlaylistManager::randomTrack()
{
	if (!CurrentPlaylist)
		return QUrl();

	int count = CurrentPlaylist->count();
	int randomIndex = qrand() % count;
	
	//if (isCursorFollowsPlayback())
	//	this->fooTabWidget->setCurrentItem(randomIndex);

	return (*CurrentPlaylist)[randomIndex].file();
}

QUrl FooPlaylistManager::getNextFile()
{
	QUrl file;
	if (Queue.isEmpty())
	{
		// if Queue is empty take order option
		switch (Order)
		{
		case PlayOrder::shuffleTracks:
			file = randomTrack();
			break;
		case PlayOrder::repeatTrack:
			file = (*CurrentPlaylist)[CurrentTruck].file();
			break;
		default:
			file = nextFile(Order == PlayOrder::repeatPlaylist, false/*isCursorFollowsPlayback()*/);
			break;
		}
	}
	else
	{
		// if we have Queue priority is on these files
		// TODO what about order here?
		qDebug() << "FooMainWindow::Queue";
		file = Queue.takeLast();
	}

	qDebug() << "FooMainWindow:: nextFile : " << file.toLocalFile();
	return file;
}

QUrl FooPlaylistManager::nextFile(bool repeat, bool follow)
{
	qDebug() << "FooTabWidget::nextFile";
	int c = CurrentPlaylist->count();

	qDebug() << "TabWidget: nextFile: c: " << c;

		int index = CurrentTruck;
		int max = CurrentPlaylist->count();
		qDebug() << "TabWidget: nextFile: for: index: " << index;
		qDebug() << "TabWidget: nextFile: for: max: " << max;

		if (index >= 0)
		{
			qDebug() << "TabWidget: nextFile: for: if: index >= 0";

			if (index == (max - 1) && repeat)
			{
				qDebug() << "TabWidget: nextFile: for: if: repeat";

				//CurrentPlayingPlaylist = wid;
				CurrentTruck = 0;
				//if (follow) wid->setCurrentItem(currentPlayingItem);
				return (*CurrentPlaylist)[CurrentTruck].file();
			}
			else if (index < (max - 1))
			{
				qDebug() << "TabWidget: nextFile: for: if: index < max";

				//currentPlayingPlaylist = wid;
				CurrentTruck++;
				//if (follow) wid->setCurrentItem(currentPlayingItem);
				return (*CurrentPlaylist)[CurrentTruck].file();
			}
			else if (index == (max - 1) && !repeat)
			{
				qDebug() << "TabWidget: nextFile: for: if: !repeat";

				return QUrl();
			}
		}

	return QUrl();
}

void FooPlaylistManager::addToPrevQueue (QUrl path)
{
}

void FooPlaylistManager::addFileToQueue (QUrl file)
{
	qDebug() << "FooMainWindow::addToQueue";
	qDebug() << "plik dodany do kolejki: " << file.toString();
	Queue.prepend(file);
}

void FooPlaylistManager::removeFileFromQueue (QUrl file)
{
	qDebug() << "FooMainWindow::removeFromQueue";
	qDebug() << "plik usuniety z kolejki: " << file.toString();
	Queue.removeOne(file);
}

void FooPlaylistManager::clearQueue()
{
	qDebug() << "FooMainWindow::clearQueue";
	Queue.clear();
}