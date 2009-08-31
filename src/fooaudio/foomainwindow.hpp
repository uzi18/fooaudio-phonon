#ifndef _FOOMAINWINDOW_HPP_
#define _FOOMAINWINDOW_HPP_

#include <QtGui>
#include "footabwidget.hpp"

class FooAudioEngine;

class FooMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	FooMainWindow (FooAudioEngine *);
	~FooMainWindow ();

signals:
	void playSignal ();
	void prevSignal ();
	void stopSignal ();
	void nextSignal ();
	void pauseSignal ();

private slots:
	void open ();
	void openAudioCD ();
	void addFiles ();
	void addFolder ();
	void addLocation ();
	void newPlaylist ();
	void loadPlaylist ();
	void savePlaylist ();
	void preferences ();
	void exit ();

	void undo ();
	void redo ();
	void clear ();
	void selectAll ();
	void sortBy ();
	void randomize ();
	void reverse ();
	void sortByFilePatch ();
	void sortByAlbum ();
	void sortByTrackNumber ();
	void sortByTitle ();
	void search ();
	void removeDuplicates ();
	void removeDeadItems ();

	void alwaysOnTop ();
	void console ();
	void equalizer ();
	void playlistManager ();
	void quickSetup ();
	void enableLayoutEditionMode ();
	void createScratchbox ();
	void cut ();
	void copy ();
	void paste ();

	void stop ();
	void pause ();
	void play ();
	void previous ();
	void next ();
	void random ();
	void defaultOrder ();
	void repeatPlaylist ();
	void repeatTrack ();
	void randomOrder ();
	void shuffleTracks ();
	void shuffleAlbums ();
	void shuffleFolders ();
	void stopAfterCurrent ();
	void playbackFollowsCursor ();
	void cursorFollowsPlayback ();

	void albumList ();
	void searchAlbum ();
	void configure ();

	void about ();

	void itemDoubleClicked(QTreeWidgetItem *, int column);

private:
	FooAudioEngine *fooAudioEngine;

	FooTabWidget *fooTabWidget;

	QMenu *fileMenu;
		QAction *openAction;
		QAction *openAudioCDAction;
		QAction *addFilesAction;
		QAction *addFolderAction;
		QAction *addLocationAction;
		QAction *newPlaylistAction;
		QAction *loadPlaylistAction;
		QAction *savePlaylistAction;
		QAction *preferencesAction;
		QAction *exitAction;

	QMenu *editMenu;
		QAction *undoAction;
		QAction *redoAction;
		QAction *clearAction;
		QAction *selectAllAction;
		QMenu *sortMenu;
			QAction *sortByAction;
			QAction *randomizeAction;
			QAction *reverseAction;
			QAction *sortByFilePatchAction;
			QAction *sortByAlbumAction;
			QAction *sortByTrackNumberAction;
			QAction *sortByTitleAction;
		QAction *searchAction;
		QAction *removeDuplicatesAction;
		QAction *removeDeadItemsAction;

	QMenu *viewMenu;
		QAction *alwaysOnTopAction;
		QMenu *visualisationsMenu;
		QAction *consoleAction;
		QAction *equalizerAction;
		QAction *playlistManagerAction;
		QMenu *layoutMenu;
			QAction *quickSetupAction;
			QAction *enableLayoutEditionModeAction;
			QAction *createScratchboxAction;
			QAction *cutAction;
			QAction *copyAction;
			QAction *pasteAction;

	QMenu *playbackMenu;
		QAction *stopAction;
		QAction *pauseAction;
		QAction *playAction;
		QAction *previousAction;
		QAction *nextAction;
		QAction *randomAction;
		QMenu *orderMenu;
			QAction *defaultOrderAction;
			QAction *repeatPlaylistAction;
			QAction *repeatTrackAction;
			QAction *randomOrderAction;
			QAction *shuffleTracksAction;
			QAction *shuffleAlbumsAction;
			QAction *shuffleFoldersAction;
		QAction *stopAfterCurrentAction;
		QAction *playbackFollowsCursorAction;
		QAction *cursorFollowsPlaybackAction;

	QMenu *libraryMenu;
		QAction *albumListAction;
		QAction *searchAlbumAction;
		QAction *configureAction;

	QMenu *helpMenu;
		QAction *aboutAction;
		QAction *aboutQtAction;

	QAction *stopToolBarAction;
	QAction *playToolBarAction;
	QAction *pauseToolBarAction;
	QAction *prevToolBarAction;
	QAction *nextToolBarAction;
	QAction *randomToolBarAction;

	QToolBar *trackToolBar;
	QToolBar *playbackToolBar;
	QToolBar *volumeToolBar;

	QSlider *trackSlider;
	QSlider *volumeSlider;

	void createMenus ();
	void createToolBars ();
	void createActions();
};

#endif // _FOOMAINWINDOW_HPP_

