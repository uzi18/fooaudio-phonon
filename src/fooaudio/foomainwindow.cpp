#include <iostream>

using namespace std;

#include <QtGui>
#include <QUrl>

#include "fooabout.hpp"
#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"
#include "fooplaylistwidget.hpp"

class FooTabWidget;

FooMainWindow::FooMainWindow(FooPhononAudioEngine *fae) : QMainWindow (), maxProgress(1000),	slider_pos(-1)
{
	this->fooAudioEngine = fae;

	fooTabWidget = new FooTabWidget();
	setCentralWidget(fooTabWidget);

	this->setWindowIcon(QIcon(":images/icon64.png"));

	createMenus();
	createToolBars();
	createStatusBar();

	createActions();

	createSystrayIcon();

	readSettings();

	connect(fooTabWidget, SIGNAL (itemDoubleClickedSignal(QTreeWidgetItem *, int)), this, SLOT (itemDoubleClicked(QTreeWidgetItem *, int)));
	// volume slider
	connect(this->volumeSlider, SIGNAL(valueChanged(int)), fooAudioEngine, SLOT(setVolume(int)));
	// track slider
	connect(fooAudioEngine, SIGNAL (progress(qint64)), this, SLOT (progress(qint64)));
	connect(this->trackSlider, SIGNAL(sliderMoved(int)), this, SLOT (seek(int)));
	connect(this->trackSlider, SIGNAL(sliderReleased()), this, SLOT (sliderReleased()));

	connect(this, SIGNAL(nextSignal(QUrl)), fooAudioEngine, SLOT(playFile(QUrl)));
	connect(this, SIGNAL(prevSignal(QUrl)), fooAudioEngine, SLOT(playFile(QUrl)));
	connect(fooAudioEngine, SIGNAL(aboutToFinish()), this, SLOT(enqueueNextFile()));
	connect(this, SIGNAL(enqueueNextFile(QUrl)), fooAudioEngine, SLOT(enqueueNextFile(QUrl)));
	connect(fooAudioEngine, SIGNAL(willPlayNow (QUrl)), this, SLOT(addToPrevQueue(QUrl)));
	
	QTimer::singleShot(200, this, SLOT(setTrayIcon()));
}

FooMainWindow::~FooMainWindow ()
{
}

void FooMainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));

	openAction = new QAction (tr ("&Open"), this);
	connect (openAction, SIGNAL (triggered ()), this, SLOT (open ()));
	fileMenu->addAction (openAction);
	openAction->setEnabled(false);

	openAudioCDAction = new QAction (tr ("Open Audio &CD"), this);
	connect (openAudioCDAction, SIGNAL (triggered ()), this, SLOT (openAudioCD ()));
	fileMenu->addAction (openAudioCDAction);
	openAudioCDAction->setEnabled(false);

	fileMenu->addSeparator ();

	addFilesAction = new QAction (tr ("&Add Files"), this);
	connect (addFilesAction, SIGNAL (triggered ()), this, SLOT (addFiles ()));
	fileMenu->addAction (addFilesAction);

	addFolderAction = new QAction (tr ("A&dd Folder"), this);
	connect (addFolderAction, SIGNAL (triggered ()), this, SLOT (addFolder ()));
	fileMenu->addAction (addFolderAction);
	addFolderAction->setEnabled(false);

	addLocationAction = new QAction (tr ("Add Lo&cation"), this);
	connect (addLocationAction, SIGNAL (triggered ()), this, SLOT (addLocation ()));
	fileMenu->addAction (addLocationAction);
	addLocationAction->setEnabled(false);

	fileMenu->addSeparator ();

	newPlaylistAction = new QAction (tr ("&New playlist"), this);
	connect (newPlaylistAction, SIGNAL (triggered ()), this, SLOT (newPlaylist ()));
	fileMenu->addAction (newPlaylistAction);
	newPlaylistAction->setEnabled(false);

	loadPlaylistAction = new QAction (tr ("&Load playlist"), this);
	connect (loadPlaylistAction, SIGNAL (triggered ()), this, SLOT (loadPlaylist ()));
	fileMenu->addAction (loadPlaylistAction);
	loadPlaylistAction->setEnabled(false);

	savePlaylistAction = new QAction (tr ("&Save playlist"), this);
	connect (savePlaylistAction, SIGNAL (triggered ()), this, SLOT (savePlaylist ()));
	fileMenu->addAction (savePlaylistAction);
	savePlaylistAction->setEnabled(false);

	fileMenu->addSeparator ();

	preferencesAction = new QAction (tr ("&Preferences"), this);
	connect (preferencesAction, SIGNAL (triggered ()), this, SLOT (preferences ()));
	fileMenu->addAction (preferencesAction);
	preferencesAction->setEnabled(false);

	fileMenu->addSeparator ();

	exitAction = new QAction (tr ("&Exit"), this);
	connect (exitAction, SIGNAL (triggered ()), this, SLOT (exit()));
	fileMenu->addAction (exitAction);

	editMenu = menuBar ()->addMenu (tr ("&Edit"));

	undoAction = new QAction (tr ("&Undo"), this);
	connect (undoAction, SIGNAL (triggered ()), this, SLOT (undo ()));
	editMenu->addAction (undoAction);
	undoAction->setEnabled(false);

	redoAction = new QAction (tr ("&Redo"), this);
	connect (redoAction, SIGNAL (triggered ()), this, SLOT (redo ()));
	editMenu->addAction (redoAction);
	redoAction->setEnabled(false);

	editMenu->addSeparator ();

	cutAction = new QAction (tr ("&Cut"), this);
	connect (cutAction, SIGNAL (triggered ()), fooTabWidget, SLOT (cut ()));
	editMenu->addAction (cutAction);

	copyAction = new QAction (tr ("C&opy"), this);
	connect (copyAction, SIGNAL (triggered ()), fooTabWidget, SLOT (copy ()));
	editMenu->addAction (copyAction);

	pasteAction = new QAction (tr ("&Paste"), this);
	connect (pasteAction, SIGNAL (triggered ()), fooTabWidget, SLOT (paste ()));
	editMenu->addAction (pasteAction);

	removeAction = new QAction (tr ("R&emove"), this);
	connect (removeAction, SIGNAL (triggered ()), fooTabWidget, SLOT (remove ()));
	editMenu->addAction (removeAction);

	clearAction = new QAction (tr ("&Clear"), this);
	connect (clearAction, SIGNAL (triggered ()), fooTabWidget, SLOT (clear ()));
	editMenu->addAction (clearAction);

	selectAllAction = new QAction (tr ("Select &all"), this);
	connect (selectAllAction, SIGNAL (triggered ()), fooTabWidget, SLOT (selectAll ()));
	editMenu->addAction (selectAllAction);

	// Queue SubMenu
	queueMenu = new QMenu (tr ("&Queue"), editMenu);
	editMenu->addAction (queueMenu->menuAction ());

	addToQueueAction = new QAction (tr ("&add to queue"), this);
	connect (addToQueueAction, SIGNAL (triggered ()), this, SLOT (addToQueue ()));
	queueMenu->addAction (addToQueueAction);

	removeFromQueueAction = new QAction (tr ("&remove from queue"), this);
	connect (removeFromQueueAction, SIGNAL (triggered ()), this, SLOT (removeFromQueue()));
	queueMenu->addAction (removeFromQueueAction);

	clearQueueAction = new QAction (tr ("&clear queue"), this);
	connect (clearQueueAction, SIGNAL (triggered ()), this, SLOT (clearQueue ()));
	queueMenu->addAction (clearQueueAction);

	// Sort SubMenu
	sortMenu = new QMenu (tr ("&Sort"), editMenu);
	editMenu->addAction (sortMenu->menuAction ());

	randomizeAction = new QAction (tr ("&Randomize"), this);
	connect (randomizeAction, SIGNAL (triggered ()), this, SLOT (randomize ()));
	sortMenu->addAction (randomizeAction);
	randomizeAction->setEnabled(false);

	reverseAction = new QAction (tr ("R&everse"), this);
	connect (reverseAction, SIGNAL (triggered ()), this, SLOT (reverse ()));
	sortMenu->addAction (reverseAction);
	reverseAction->setEnabled(false);

	sortByFilePatchAction = new QAction (tr ("Sort by file &patch"), this);
	connect (sortByFilePatchAction, SIGNAL (triggered ()), this, SLOT (sortByFilePatch ()));
	sortMenu->addAction (sortByFilePatchAction);
	sortByFilePatchAction->setEnabled(false);

	sortByAlbumAction = new QAction (tr ("Sort by &album"), this);
	connect (sortByAlbumAction, SIGNAL (triggered ()), this, SLOT (sortByAlbum ()));
	sortMenu->addAction (sortByAlbumAction);
	sortByAlbumAction->setEnabled(false);

	sortByTrackNumberAction = new QAction (tr ("Sort by track &number"), this);
	connect (sortByTrackNumberAction, SIGNAL (triggered ()), this, SLOT (sortByTrackNumber ()));
	sortMenu->addAction (sortByTrackNumberAction);
	sortByTrackNumberAction->setEnabled(false);

	sortByTitleAction = new QAction (tr ("Sort by &title"), this);
	connect (sortByTitleAction, SIGNAL (triggered ()), this, SLOT (sortByTitle ()));
	sortMenu->addAction (sortByTitleAction);
	sortByTitleAction->setEnabled(false);

	searchAction = new QAction (tr ("Sear&ch"), this);
	connect (searchAction, SIGNAL (triggered ()), this, SLOT (search ()));
	editMenu->addAction (searchAction);
	searchAction->setEnabled(false);

	removeDuplicatesAction = new QAction (tr ("Remove &duplicates"), this);
	connect (removeDuplicatesAction, SIGNAL (triggered ()), this, SLOT (removeDuplicates ()));
	editMenu->addAction (removeDuplicatesAction);
	removeDuplicatesAction->setEnabled(false);

	removeDeadItemsAction = new QAction (tr ("Remove dead &items"), this);
	connect (removeDeadItemsAction, SIGNAL (triggered ()), this, SLOT (removeDeadItems ()));
	editMenu->addAction (removeDeadItemsAction);
	removeDeadItemsAction->setEnabled(false);

	viewMenu = menuBar ()->addMenu (tr ("&View"));

	alwaysOnTopAction = new QAction (tr ("Always on &top"), this);
	connect (alwaysOnTopAction, SIGNAL (triggered ()), this, SLOT (alwaysOnTop ()));
	viewMenu->addAction (alwaysOnTopAction);
	alwaysOnTopAction->setEnabled(false);

	visualisationsMenu = new QMenu (tr ("&Visualisations"), viewMenu);
	viewMenu->addAction (visualisationsMenu->menuAction ());

	consoleAction = new QAction (tr ("&Console"), this);
	connect (consoleAction, SIGNAL (triggered ()), this, SLOT (console ()));
	viewMenu->addAction (consoleAction);
	consoleAction->setEnabled(false);

	equalizerAction = new QAction (tr ("&Equalizer"), this);
	connect (equalizerAction, SIGNAL (triggered ()), this, SLOT (equalizer ()));
	viewMenu->addAction (equalizerAction);
	equalizerAction->setEnabled(false);

	playlistManagerAction = new QAction (tr ("&Playlist Manager"), this);
	connect (playlistManagerAction, SIGNAL (triggered ()), this, SLOT (playlistManager ()));
	viewMenu->addAction (playlistManagerAction);
	playlistManagerAction->setEnabled(false);

	layoutMenu = new QMenu (tr ("&Layout"), viewMenu);
	viewMenu->addAction (layoutMenu->menuAction ());

	quickSetupAction = new QAction (tr ("&Quick setup"), this);
	connect (quickSetupAction, SIGNAL (triggered ()), this, SLOT (quickSetup ()));
	layoutMenu->addAction (quickSetupAction);
	quickSetupAction->setEnabled(false);

	enableLayoutEditionModeAction = new QAction (tr ("&Enable layout edition mode"), this);
	connect (enableLayoutEditionModeAction, SIGNAL (triggered ()), this, SLOT (enableLayoutEditionMode ()));
	layoutMenu->addAction (enableLayoutEditionModeAction);
	enableLayoutEditionModeAction->setEnabled(false);

	createScratchboxAction = new QAction (tr ("Create &scratchbox"), this);
	connect (createScratchboxAction, SIGNAL (triggered ()), this, SLOT (createScratchbox ()));
	layoutMenu->addAction (createScratchboxAction);
	createScratchboxAction->setEnabled(false);

	layoutMenu->addSeparator ();

	// what is this ?
	cutLayoutAction = new QAction (tr ("&Cut"), this);
	connect (cutLayoutAction, SIGNAL (triggered ()), this, SLOT (cutLayout ()));
	layoutMenu->addAction (cutLayoutAction);
	cutLayoutAction->setEnabled(false);

	copyLayoutAction = new QAction (tr ("C&opy"), this);
	connect (copyLayoutAction, SIGNAL (triggered ()), this, SLOT (copyLayout ()));
	layoutMenu->addAction (copyLayoutAction);
	copyLayoutAction->setEnabled(false);

	pasteLayoutAction = new QAction (tr ("&Paste"), this);
	connect (pasteLayoutAction, SIGNAL (triggered ()), this, SLOT (pasteLayout ()));
	layoutMenu->addAction (pasteLayoutAction);
	pasteLayoutAction->setEnabled(false);

	playbackMenu = menuBar ()->addMenu (tr ("&Playback"));

	stopAction = new QAction (tr ("&Stop"), this);
	connect (stopAction, SIGNAL (triggered ()), this, SLOT (stop ()));
	playbackMenu->addAction (stopAction);

	pauseAction = new QAction (tr ("Pa&use"), this);
	connect (pauseAction, SIGNAL (triggered ()), this, SLOT (pause ()));
	playbackMenu->addAction (pauseAction);

	playAction = new QAction (tr ("&Play"), this);
	connect (playAction, SIGNAL (triggered ()), this, SLOT (play ()));
	playbackMenu->addAction (playAction);

	previousAction = new QAction (tr ("Pr&evious"), this);
	connect (previousAction, SIGNAL (triggered ()), this, SLOT (previous ()));
	playbackMenu->addAction (previousAction);

	nextAction = new QAction (tr ("&Next"), this);
	connect (nextAction, SIGNAL (triggered ()), this, SLOT (next ()));
	playbackMenu->addAction (nextAction);

	randomAction = new QAction (tr ("&Random"), this);
	connect (randomAction, SIGNAL (triggered ()), this, SLOT (random ()));
	playbackMenu->addAction (randomAction);
	randomAction->setEnabled(false);

	playbackMenu->addSeparator ();

	orderMenu = new QMenu (tr ("&Order"), playbackMenu);
	playbackMenu->addAction (orderMenu->menuAction ());

	defaultOrderAction = new QAction (tr ("&Default"), this);
	connect (defaultOrderAction, SIGNAL (triggered ()), this, SLOT (defaultOrder ()));
	orderMenu->addAction (defaultOrderAction);
	defaultOrderAction->setEnabled(false);
	defaultOrderAction->setCheckable(true);

	repeatPlaylistAction = new QAction (tr ("Repeat (&playlist)"), this);
	connect (repeatPlaylistAction, SIGNAL (triggered ()), this, SLOT (repeatPlaylist ()));
	orderMenu->addAction (repeatPlaylistAction);
	repeatPlaylistAction->setCheckable(true);

	repeatTrackAction = new QAction (tr ("Repeat (&track)"), this);
	connect (repeatTrackAction, SIGNAL (triggered ()), this, SLOT (repeatTrack ()));
	orderMenu->addAction (repeatTrackAction);
	repeatTrackAction->setEnabled(false);
	repeatTrackAction->setCheckable(true);

	randomOrderAction = new QAction (tr ("Ra&ndom"), this);
	connect (randomOrderAction, SIGNAL (triggered ()), this, SLOT (randomOrder ()));
	orderMenu->addAction (randomOrderAction);
	randomOrderAction->setEnabled(false);
	randomOrderAction->setCheckable(true);

	shuffleTracksAction = new QAction (tr ("&Shuffle (tracks)"), this);
	connect (shuffleTracksAction, SIGNAL (triggered ()), this, SLOT (shuffleTracks ()));
	orderMenu->addAction (shuffleTracksAction);
	shuffleTracksAction->setEnabled(false);
	shuffleTracksAction->setCheckable(true);

	shuffleAlbumsAction = new QAction (tr ("S&huffle (albums)"), this);
	connect (shuffleAlbumsAction, SIGNAL (triggered ()), this, SLOT (shuffleAlbums ()));
	orderMenu->addAction (shuffleAlbumsAction);
	shuffleAlbumsAction->setEnabled(false);
	shuffleAlbumsAction->setCheckable(true);

	shuffleFoldersAction = new QAction (tr ("Shuffle (&folders)"), this);
	connect (shuffleFoldersAction, SIGNAL (triggered ()), this, SLOT (shuffleFolders ()));
	orderMenu->addAction (shuffleFoldersAction);
	shuffleFoldersAction->setEnabled(false);
	shuffleFoldersAction->setCheckable(true);

	stopAfterCurrentAction = new QAction (tr ("S&top after current"), this);
	connect (stopAfterCurrentAction, SIGNAL (triggered ()), this, SLOT (stopAfterCurrent ()));
	playbackMenu->addAction (stopAfterCurrentAction);
	stopAfterCurrentAction->setEnabled(false);

	playbackFollowsCursorAction = new QAction (tr ("Playback &follows cursor"), this);
	connect (playbackFollowsCursorAction, SIGNAL (triggered ()), this, SLOT (playbackFollowsCursor ()));
	playbackMenu->addAction (playbackFollowsCursorAction);
	playbackFollowsCursorAction->setEnabled(false);

	cursorFollowsPlaybackAction = new QAction (tr ("&Cursor follows playback"), this);
	connect (cursorFollowsPlaybackAction, SIGNAL (triggered ()), this, SLOT (cursorFollowsPlayback ()));
	playbackMenu->addAction (cursorFollowsPlaybackAction);
	cursorFollowsPlaybackAction->setEnabled(false);

	libraryMenu = menuBar ()->addMenu (tr ("&Library"));

	albumListAction = new QAction (tr ("&Album list"), this);
	connect (albumListAction, SIGNAL (triggered ()), this, SLOT (albumList ()));
	libraryMenu->addAction (loadPlaylistAction);
	albumListAction->setEnabled(false);

	searchAlbumAction = new QAction (tr ("&Search"), this);
	connect (searchAlbumAction, SIGNAL (triggered ()), this, SLOT (searchAlbum ()));
	libraryMenu->addAction (searchAlbumAction);
	searchAlbumAction->setEnabled(false);

	libraryMenu->addSeparator ();

	configureAction = new QAction (tr ("&Configure"), this);
	connect (configureAction, SIGNAL (triggered ()), this, SLOT (configure ()));
	libraryMenu->addAction (configureAction);
	configureAction->setEnabled(false);

	settingsMenu = menuBar ()->addMenu (tr ("Se&ttings"));

	trayIconAction = new QAction (tr ("&Tray Icon"), this);
	connect (trayIconAction, SIGNAL (triggered ()), this, SLOT (setTrayIcon ()));
	settingsMenu->addAction (trayIconAction);
	trayIconAction->setCheckable (true); 
	trayIconAction->setEnabled (QSystemTrayIcon::isSystemTrayAvailable());

	helpMenu = menuBar ()->addMenu (tr ("&Help"));

	aboutAction = new QAction (tr ("&About"), this);
	connect (aboutAction, SIGNAL (triggered ()), this, SLOT (about ()));
	helpMenu->addAction (aboutAction);

	aboutQtAction = new QAction (tr ("About &Qt"), this);
	connect (aboutQtAction, SIGNAL (triggered ()), qApp, SLOT (aboutQt ()));
	helpMenu->addAction (aboutQtAction);

}

void FooMainWindow::createToolBars ()
{
	trackToolBar = new QToolBar (this);
	trackToolBar->setFloatable (false);
	trackSlider = new QSlider (Qt::Horizontal);
	trackSlider->setRange(0, maxProgress);
	trackToolBar->addWidget (trackSlider);
	addToolBar (trackToolBar);

	volumeToolBar = new QToolBar (this);
	volumeToolBar->setFloatable (false);
	volumeToolBarAction = new QAction (tr("Mute"),this);
	bool mute = fooAudioEngine->isMuted();
	volumeToolBarAction->setIcon(QIcon (mute ? ":images/mute.png" : ":images/vol.png"));
	connect (volumeToolBarAction, SIGNAL (triggered ()), this, SLOT (mute()));
	volumeToolBar->addAction (volumeToolBarAction);
	volumeSlider = new QSlider (Qt::Horizontal);
	volumeSlider->setRange(0, 100);
	volumeSlider->setTracking(true);
	volumeToolBar->addWidget (volumeSlider);
	addToolBar (volumeToolBar);

	playbackToolBar = new QToolBar (this);
	playbackToolBar->setFloatable (false);

	stopToolBarAction = new QAction (QIcon (":images/stop.png"), tr ("Stop"),this);
	connect (stopToolBarAction, SIGNAL (triggered ()), this, SLOT (stop ()));
	playbackToolBar->addAction (stopToolBarAction);

	playToolBarAction = new QAction (QIcon (":images/play.png"), tr ("Play"),this);
	connect (playToolBarAction, SIGNAL (triggered ()), this, SLOT (play ()));
	playbackToolBar->addAction (playToolBarAction);

	pauseToolBarAction = new QAction (QIcon (":images/pause.png"), tr ("Pause"),this);
	connect (pauseToolBarAction, SIGNAL (triggered ()), this, SLOT (pause ()));
	playbackToolBar->addAction (pauseToolBarAction);

	prevToolBarAction = new QAction (QIcon (":images/prev.png"), tr ("Previous"),this);
	connect (prevToolBarAction, SIGNAL (triggered ()), this, SLOT (previous ()));
	playbackToolBar->addAction (prevToolBarAction);

	nextToolBarAction = new QAction (QIcon (":images/next.png"), tr ("Next"),this);
	connect (nextToolBarAction, SIGNAL (triggered ()), this, SLOT (next ()));
	playbackToolBar->addAction (nextToolBarAction);

	randomToolBarAction = new QAction (QIcon (":images/random.png"), tr ("Random"),this);
	connect (randomToolBarAction, SIGNAL (triggered ()), this, SLOT (random ()));
	playbackToolBar->addAction (randomToolBarAction);
	randomToolBarAction->setEnabled(false);

	addToolBar (playbackToolBar);
}

void FooMainWindow::createActions()
{
	connect(this, SIGNAL(playSignal()), fooAudioEngine, SLOT(play()));
	connect(this, SIGNAL(pauseSignal()), fooAudioEngine, SLOT(pause()) );
	connect(this, SIGNAL(stopSignal()), fooAudioEngine, SLOT(stop()));
}

void FooMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void FooMainWindow::createSystrayIcon()
{
	if (QSystemTrayIcon::isSystemTrayAvailable())
	{
		trayIcon = new QSystemTrayIcon(this);
		trayIcon->setIcon(QIcon(":images/icon64.png"));
		trayIcon->setContextMenu(playbackMenu);

		connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

	}
}

void FooMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		setVisible(!isVisible());
		break;
	//case QSystemTrayIcon::DoubleClick:
	//    	break;
	//case QSystemTrayIcon::MiddleClick:
	//    ;
	//    	break;
	//case QSystemTrayIcon::Context:
	//    	break;
	default:
	    ;
	}
}

void FooMainWindow::itemDoubleClicked(QTreeWidgetItem * item, int column)
{
	cerr << "FooMainWindow::itemDoubleClicked" << endl;

	QString nextOne = item->text(0);
	cerr << nextOne.toStdString() << endl;

	emit nextSignal(nextOne);
}

QUrl FooMainWindow::getNextFile()
{
	QUrl file;

	if (queue.isEmpty())
	{
		file = fooTabWidget->nextFile(true);
	}
	else
	{
		cerr << "FooMainWindow::Queue" << endl;
		file = queue.takeLast();
	}

	cerr << "FooMainWindow:: nextFile : " << file.toLocalFile().toStdString() << endl;
	return file;
}

void FooMainWindow::enqueueNextFile()
{
	emit enqueueNextFile(getNextFile());
}

void FooMainWindow::writeSettings()
{
	cerr << "Save Settings" << endl;
	QSettings settings("fooaudio", "fooaudio");

	settings.clear();

	settings.beginGroup("FooMainWindow");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("trackToolBar", trackToolBar->geometry());
	settings.setValue("playbackToolBar", playbackToolBar->geometry());
	settings.setValue("volumeToolBar", volumeToolBar->geometry());
	settings.setValue("trayIcon", trayIconAction->isChecked());
	settings.endGroup();

	settings.beginGroup("Volume");
	settings.setValue("volume", volumeSlider->value());
	settings.setValue("muted", fooAudioEngine->isMuted());
	settings.endGroup();

	QSettings playlists("fooaudio", "playlists");

	playlists.beginWriteArray("playlists");
	for (int i = 0; i < fooTabWidget->count(); ++i)
	{
		playlists.setArrayIndex(i);
		playlists.setValue("name", fooTabWidget->tabText(i));

		playlists.beginWriteArray("playlist");

		FooPlaylistWidget * fooPlaylistWidget = (FooPlaylistWidget*) fooTabWidget->widget(i);
		for (int j = 0; j < fooPlaylistWidget->topLevelItemCount(); ++j)
		{
			playlists.setArrayIndex(j);
			playlists.setValue("path", fooPlaylistWidget->file(j).toString());
		}
		playlists.endArray();
	}
	playlists.endArray();

	playlists.beginGroup("current");
	playlists.setValue("playlist", fooTabWidget->getCurrentPlaylistIndex());
	playlists.setValue("path", fooTabWidget->getCurrentItemIndex());
	playlists.endGroup();

}

void FooMainWindow::readSettings()
{
	cerr << "Read Settings" << endl;
	QSettings settings("fooaudio", "fooaudio");

	settings.beginGroup("FooMainWindow");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	trackToolBar->setGeometry(settings.value("trackToolBar", QRect()).toRect());
	playbackToolBar->setGeometry(settings.value("playbackToolBar", QRect()).toRect());
	volumeToolBar->setGeometry(settings.value("volumeToolBar", QRect()).toRect());
	trayIconAction->setChecked(settings.value("trayIcon", true).toBool());
	settings.endGroup();

	settings.beginGroup("Volume");
	fooAudioEngine->setVolume(settings.value("volume", 100).toInt());
	volumeSlider->setValue(settings.value("volume", 100).toInt());
	fooAudioEngine->setMuted(settings.value("muted", false).toBool());
	settings.endGroup();

	QSettings playlists("fooaudio", "playlists");

	int tabsCount = playlists.beginReadArray("playlists");

	if (tabsCount > 0)
	{
		for (int i = 0; i < tabsCount; ++i)
		{
			playlists.setArrayIndex(i);
			fooTabWidget->newTab(playlists.value("name").toString());

			int songsCount = playlists.beginReadArray("playlist");

			FooPlaylistWidget * fooPlaylistWidget = (FooPlaylistWidget*) fooTabWidget->widget(i);
			for (int j = 0; j < songsCount; ++j)
			{
				playlists.setArrayIndex(j);

				fooPlaylistWidget->addFile(playlists.value("path").toString());
			}
			playlists.endArray();
		}
	}
	else
	{
		fooTabWidget->newTab("Default");
	}
	playlists.endArray();

	playlists.beginGroup("current");
	fooTabWidget->setCurrentPlaylist(playlists.value("playlist", 0).toInt());
	fooTabWidget->setCurrentItem(playlists.value("path", 0).toInt());
	playlists.endGroup();

}

 void FooMainWindow::closeEvent(QCloseEvent *event)
 {
	writeSettings();
	event->accept();
 }

void FooMainWindow::open ()
{
}

void FooMainWindow::openAudioCD ()
{
}

void FooMainWindow::addFiles ()
{
	cerr << "FooMainWindow::addFiles" << endl;
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Music Files"), QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

	if (files.isEmpty())
	{
		 return;
	}

	foreach (QString string, files)
	{
		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (fooTabWidget->currentWidget());
		wid->addFile (string);
	}
}

void FooMainWindow::addFolder ()
{
}

void FooMainWindow::addLocation ()
{
}

void FooMainWindow::newPlaylist ()
{
}

void FooMainWindow::loadPlaylist ()
{
}

void FooMainWindow::savePlaylist ()
{
}

void FooMainWindow::preferences ()
{
}

void FooMainWindow::exit ()
{
	writeSettings();
	qApp->quit();
}

void FooMainWindow::undo ()
{
}

void FooMainWindow::redo ()
{
}

void FooMainWindow::sortBy ()
{
}

void FooMainWindow::randomize ()
{
}

void FooMainWindow::reverse ()
{
}

void FooMainWindow::sortByFilePatch ()
{
}

void FooMainWindow::sortByAlbum ()
{
}

void FooMainWindow::sortByTrackNumber ()
{
}

void FooMainWindow::sortByTitle ()
{
}

void FooMainWindow::search ()
{
}

void FooMainWindow::removeDuplicates ()
{
}

void FooMainWindow::removeDeadItems ()
{
}

void FooMainWindow::alwaysOnTop ()
{
}

void FooMainWindow::console ()
{
}

void FooMainWindow::equalizer ()
{
}

void FooMainWindow::playlistManager ()
{
}

void FooMainWindow::quickSetup ()
{
}

void FooMainWindow::enableLayoutEditionMode ()
{
}

void FooMainWindow::createScratchbox ()
{
}

void FooMainWindow::mute ()
{
	bool newMute = !fooAudioEngine->isMuted();
	fooAudioEngine->setMuted(newMute);
	volumeToolBarAction->setIcon(QIcon (newMute ? ":images/mute.png" : ":images/vol.png"));
}

void FooMainWindow::stop ()
{
	cerr << "FooMainWindow::stop" << endl;
	trackSlider->setValue(0);
	emit stopSignal();
}

void FooMainWindow::pause ()
{
	cerr << "FooMainWindow::pause" << endl;
	emit pauseSignal ();
}

void FooMainWindow::play ()
{
	cerr << "FooMainWindow::play" << endl;

	if (fooAudioEngine->isPaused())
	{
		cerr << "was PausedState" << endl;
		emit playSignal();
	}
	else if (fooAudioEngine->isStopped())
	{
		cerr << "was StoppedState" << endl;
		FooPlaylistWidget *playlist = (FooPlaylistWidget*)fooTabWidget->currentWidget();
		if (!playlist)
			return;

		fooTabWidget->setCurrentPlaylist(fooTabWidget->currentIndex());
		fooTabWidget->setCurrentItem(0);
		QUrl file = playlist->file(0);
		cerr << file.toString().toStdString() << endl;

		emit nextSignal (file);
	}
	else
	{
		// implement this
		cerr << "Inny state" << endl;
	}
}

void FooMainWindow::previous ()
{
	cerr << "FooMainWindow::previous" << endl;
	emit prevSignal (fooTabWidget->previousFile(true));
}

void FooMainWindow::next ()
{
	cerr << "FooMainWindow::next" << endl;

	emit nextSignal (getNextFile());
}

void FooMainWindow::addToQueue ()
{
			FooPlaylistWidget * foo = (FooPlaylistWidget*)fooTabWidget->currentWidget();
	if (!foo)
		return;

	foreach (QTreeWidgetItem * item, foo->selectedItems())
	{
		if (item)
		{
			addFileToQueue(QUrl(item->text(0)));
		}
	}
}

void FooMainWindow::removeFromQueue()
{
			FooPlaylistWidget * foo = (FooPlaylistWidget*)fooTabWidget->currentWidget();
	if (!foo)
		return;

	foreach (QTreeWidgetItem * item, foo->selectedItems())
	{
		if (item)
		{
			removeFileFromQueue(QUrl(item->text(0)));
		}
	}
}

void FooMainWindow::random ()
{
}

void FooMainWindow::defaultOrder ()
{
}

void FooMainWindow::repeatPlaylist ()
{
}

void FooMainWindow::repeatTrack ()
{
}

void FooMainWindow::randomOrder ()
{
}

void FooMainWindow::shuffleTracks ()
{
}

void FooMainWindow::shuffleAlbums ()
{
}

void FooMainWindow::shuffleFolders ()
{
}

void FooMainWindow::stopAfterCurrent ()
{
}

void FooMainWindow::playbackFollowsCursor ()
{
}

void FooMainWindow::cursorFollowsPlayback ()
{
}

void FooMainWindow::albumList ()
{
}

void FooMainWindow::searchAlbum ()
{
}

void FooMainWindow::configure ()
{
}

void FooMainWindow::setTrayIcon ()
{
	trayIcon->setVisible(trayIconAction->isChecked());
}

void FooMainWindow::cutLayout ()
{
}

void FooMainWindow::copyLayout ()
{
}

void FooMainWindow::pasteLayout ()
{
}

void FooMainWindow::about ()
{
	(new FooAbout(this))->show();
}

int FooMainWindow::getMaxProgress()
{
	return maxProgress;
}

void FooMainWindow::progress(qint64 time)
{
	qint64 total = fooAudioEngine->totalTime();
	int progress;

	if (total == 0)
		progress = 0;
	else
		progress = (int) (time*maxProgress/total);

	if (progress >= 0 && !trackSlider->isSliderDown())
		trackSlider->setValue(progress);
}

void FooMainWindow::seek(int value)
{
	if (slider_pos != value)
		slider_pos = value;
}

void FooMainWindow::sliderReleased()
{
	if (slider_pos == -1)
		return;
	// think to check if value is valid for seek
	fooAudioEngine->seek(fooAudioEngine->totalTime()*slider_pos/maxProgress);
	slider_pos = -1;
}

void FooMainWindow::addToPrevQueue (QUrl path)
{
}

void FooMainWindow::addFileToQueue (QUrl file)
{
	cerr << "FooMainWindow::addToQueue" << endl;
	cerr << "plik dodany do kolejki: " << file.toString().toStdString() << endl;
	queue.prepend(file);
}

void FooMainWindow::removeFileFromQueue (QUrl file)
{
	cerr << "FooMainWindow::removeFromQueue" << endl;
	cerr << "plik usuniety z kolejki: " << file.toString().toStdString() << endl;
	queue.removeOne(file);
}

void FooMainWindow::clearQueue()
{
	cerr << "FooMainWindow::clearQueue" << endl;
	queue.clear();
}
