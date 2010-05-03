#ifndef _FOOMAINWINDOW_HPP_
#define _FOOMAINWINDOW_HPP_

#include <QtGui>
#include <QUrl>
#include "footabwidget.hpp"
#include "abstractaudioplugin.h"

class	QSystemTrayIcon;

class	FooPhononAudioEngine;

class	FooMainWindow : public QMainWindow
{
	Q_OBJECT
	Q_DISABLE_COPY(FooMainWindow)

	static FooMainWindow *Instance;

	FooMainWindow ();
	void init();

public:
	~FooMainWindow ();

	static FooMainWindow * instance();

	void setAudioEngine(FooAudio::AbstractAudioPlugin * engine);
	FooAudio::AbstractAudioPlugin * audioEngine() {return fooAudioEngine;}

	FooTabWidget *fooTabWidget;

	QSlider *trackSlider;
	QSlider *volumeSlider;

	int getMaxProgress();

	void readSettings();
	void writeSettings();

	bool isCursorFollowsPlayback();

	QUrl getNextFile();

signals:
	void playSignal();
	void prevSignal(QUrl);
	void stopSignal();
	void nextSignal(QUrl);
	void randomSignal(QUrl);
	void pauseSignal();
	void enqueueNextFile(QUrl);

public slots:
	void enqueueNextFile();

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

	void mute ();
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
	void uncheckAllOrders();
	void about ();

	void cutLayout();
	void copyLayout();
	void pasteLayout();

	// Queue
	void addToQueue ();
	void removeFromQueue();
	void clearQueue ();

	void itemDoubleClicked(QTreeWidgetItem *, int column);

	// progress slider
	void progress (qint64 time);
	void seek (int value);
	// volume slider
	void sliderReleased ();

	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void setTrayIcon ();

	// update window title according to new meta data
	void updateWindowTitle(QMultiMap<QString, QString> newMetaData);

private:
	const int maxProgress;
	FooAudio::AbstractAudioPlugin *fooAudioEngine;
	QSize *iconSize;

	QSystemTrayIcon *trayIcon;

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
	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *removeAction;
	QAction *clearAction;
	QAction *selectAllAction;
	QMenu *queueMenu;
	QAction *addToQueueAction;
	QAction *removeFromQueueAction;
	QAction *clearQueueAction;
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
	QAction *cutLayoutAction;
	QAction *copyLayoutAction;
	QAction *pasteLayoutAction;

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

	QMenu *settingsMenu;
	QAction *trayIconAction;

	QMenu *helpMenu;
	QAction *aboutAction;
	QAction *aboutQtAction;

	QMenu *trayMenu;

	QAction *volumeToolBarAction;
	QAction *stopToolBarAction;
	QAction *playToolBarAction;
	QAction *pauseToolBarAction;
	QAction *prevToolBarAction;
	QAction *nextToolBarAction;
	QAction *randomToolBarAction;

	QToolBar *trackToolBar;
	QToolBar *playbackToolBar;
	QToolBar *volumeToolBar;

	int slider_pos;

	void createMenus();
	void createToolBars();
	void createActions();
	void createStatusBar();
	void createSystrayIcon();

protected:
	void savePlaylistToPls(QString filePath);
	void savePlaylistToM3u(QString filePath);
	void closeEvent(QCloseEvent *);
};

#endif // _FOOMAINWINDOW_HPP_

