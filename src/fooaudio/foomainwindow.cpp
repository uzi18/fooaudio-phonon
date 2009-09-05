#include <iostream>

using namespace std;

#include <QtGui>

#include "fooabout.hpp"
#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"
#include "fooplaylistwidget.hpp"

class FooTabWidget;

FooMainWindow::FooMainWindow(FooAudioEngine *fae) : QMainWindow (), maxProgress(1000)
{
	this->fooAudioEngine = fae;

	createMenus();
	createToolBars();
	createStatusBar();
	fooTabWidget = new FooTabWidget();
	setCentralWidget(fooTabWidget);
	setWindowTitle(tr("fooaudio"));

	createActions();

	connect (fooTabWidget, SIGNAL (itemDoubleClickedSignal(QTreeWidgetItem *, int)), this, SLOT (itemDoubleClicked(QTreeWidgetItem *, int)));
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
	connect (exitAction, SIGNAL (triggered ()), qApp, SLOT (quit ()));
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

	clearAction = new QAction (tr ("&Clear"), this);
	connect (clearAction, SIGNAL (triggered ()), this, SLOT (clear ()));
	editMenu->addAction (clearAction);
	clearAction->setEnabled(false);

	selectAllAction = new QAction (tr ("Select &all"), this);
	connect (selectAllAction, SIGNAL (triggered ()), this, SLOT (selectAll ()));
	editMenu->addAction (selectAllAction);
	selectAllAction->setEnabled(false);

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

	cutAction = new QAction (tr ("&Cut"), this);
	connect (cutAction, SIGNAL (triggered ()), this, SLOT (cut ()));
	layoutMenu->addAction (cutAction);
	cutAction->setEnabled(false);

	copyAction = new QAction (tr ("C&opy"), this);
	connect (copyAction, SIGNAL (triggered ()), this, SLOT (copy ()));
	layoutMenu->addAction (copyAction);
	copyAction->setEnabled(false);

	pasteAction = new QAction (tr ("&Paste"), this);
	connect (pasteAction, SIGNAL (triggered ()), this, SLOT (paste ()));
	layoutMenu->addAction (pasteAction);
	pasteAction->setEnabled(false);

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
	previousAction->setEnabled(false);

	nextAction = new QAction (tr ("&Next"), this);
	connect (nextAction, SIGNAL (triggered ()), this, SLOT (next ()));
	playbackMenu->addAction (nextAction);
	nextAction->setEnabled(false);

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

	repeatPlaylistAction = new QAction (tr ("Repeat (&playlist)"), this);
	connect (defaultOrderAction, SIGNAL (triggered ()), this, SLOT (defaultOrder ()));
	orderMenu->addAction (defaultOrderAction);
	repeatPlaylistAction->setEnabled(false);

	repeatTrackAction = new QAction (tr ("Repeat (&track)"), this);
	connect (repeatTrackAction, SIGNAL (triggered ()), this, SLOT (repeatTrack ()));
	orderMenu->addAction (repeatTrackAction);
	repeatTrackAction->setEnabled(false);

	randomOrderAction = new QAction (tr ("Ra&ndom"), this);
	connect (randomOrderAction, SIGNAL (triggered ()), this, SLOT (randomOrder ()));
	orderMenu->addAction (randomOrderAction);
	randomOrderAction->setEnabled(false);

	shuffleTracksAction = new QAction (tr ("&Shuffle (tracks)"), this);
	connect (shuffleTracksAction, SIGNAL (triggered ()), this, SLOT (shuffleTracks ()));
	orderMenu->addAction (shuffleTracksAction);
	shuffleTracksAction->setEnabled(false);

	shuffleAlbumsAction = new QAction (tr ("S&huffle (albums)"), this);
	connect (shuffleAlbumsAction, SIGNAL (triggered ()), this, SLOT (shuffleAlbums ()));
	orderMenu->addAction (shuffleAlbumsAction);
	shuffleAlbumsAction->setEnabled(false);

	shuffleFoldersAction = new QAction (tr ("Shuffle (&folders)"), this);
	connect (shuffleFoldersAction, SIGNAL (triggered ()), this, SLOT (shuffleFolders ()));
	orderMenu->addAction (shuffleFoldersAction);
	shuffleFoldersAction->setEnabled(false);

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
	trackToolBar = new QToolBar ("Track ToolBar", this);
	trackToolBar->setFloatable (false);
	trackSlider = new QSlider (Qt::Horizontal);
	trackSlider->setRange(0, maxProgress);
	trackToolBar->addWidget (trackSlider);
	addToolBar (trackToolBar);

	volumeToolBar = new QToolBar ("Volume ToolBar", this);
	volumeToolBar->setFloatable (false);
	volumeToolBarAction = new QAction (QIcon (":images/vol.png"), tr ("mute"),this);
	connect (volumeToolBarAction, SIGNAL (triggered ()), this, SLOT (mute()));
	volumeToolBar->addAction (volumeToolBarAction);
	volumeSlider = new QSlider (Qt::Horizontal);
	volumeToolBar->addWidget (volumeSlider);
	addToolBar (volumeToolBar);

	playbackToolBar = new QToolBar ("Playback ToolBar", this);
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
	//prevToolBarAction->setEnabled(false);

	nextToolBarAction = new QAction (QIcon (":images/next.png"), tr ("Next"),this);
	connect (nextToolBarAction, SIGNAL (triggered ()), this, SLOT (next ()));
	playbackToolBar->addAction (nextToolBarAction);
	//nextToolBarAction->setEnabled(false);

	randomToolBarAction = new QAction (QIcon (":images/random.png"), tr ("Random"),this);
	connect (randomToolBarAction, SIGNAL (triggered ()), this, SLOT (random ()));
	playbackToolBar->addAction (randomToolBarAction);
	randomToolBarAction->setEnabled(false);

	addToolBar (playbackToolBar);
}

void FooMainWindow::createActions()
{
	connect(this, SIGNAL(playSignal()), fooAudioEngine->getMediaObject(), SLOT(play()));
	connect(this, SIGNAL(pauseSignal()), fooAudioEngine->getMediaObject(), SLOT(pause()) );
	connect(this, SIGNAL(stopSignal()), fooAudioEngine->getMediaObject(), SLOT(stop()));
	connect(this, SIGNAL(nextSignal()), fooAudioEngine, SLOT(nextFile()));
	connect(this, SIGNAL(prevSignal()), fooAudioEngine, SLOT(previousFile()));
}

void FooMainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void FooMainWindow::itemDoubleClicked(QTreeWidgetItem * item, int column)
{
	cerr << "FooMainWindow::itemDoubleClicked" << endl;
	bool wasPlaying = fooAudioEngine->getMediaObject()->state() == Phonon::PlayingState;

	FooPlaylistWidget * foo = (FooPlaylistWidget*)fooTabWidget->currentWidget();
	QLabel * bar = (QLabel*)foo->itemWidget(item, 0);
	cerr << bar->text().toStdString() << endl;

	fooAudioEngine->getMediaObject()->stop();
	fooAudioEngine->getMediaObject()->clearQueue();

	fooAudioEngine->getMediaObject()->setCurrentSource(bar->text());

	  if (wasPlaying)
			fooAudioEngine->getMediaObject()->play();
	  else
			fooAudioEngine->getMediaObject()->stop();

	  emit playSignal();
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
}

void FooMainWindow::undo ()
{
}

void FooMainWindow::redo ()
{
}

void FooMainWindow::clear ()
{
}

void FooMainWindow::selectAll ()
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

void FooMainWindow::cut ()
{
}

void FooMainWindow::copy ()
{
}

void FooMainWindow::paste ()
{
}

void FooMainWindow::mute ()
{
}

void FooMainWindow::stop ()
{
	cerr << "FooMainWindow::stop" << endl;
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

	if (fooAudioEngine->getMediaObject()->state() == Phonon::PausedState)
	{
		cerr << "PausedState" << endl;
		fooAudioEngine->getMediaObject()->play();
	}
	else if (fooAudioEngine->getMediaObject()->state() == Phonon::StoppedState || fooAudioEngine->getMediaObject()->state() == Phonon::LoadingState)
	{
		cerr << "StoppedState" << endl;
		fooAudioEngine->getMediaObject()->clearQueue();
		FooPlaylistWidget *playlist = (FooPlaylistWidget*)fooTabWidget->currentWidget();
		fooAudioEngine->getMediaObject()->setCurrentSource(playlist->file(0).toLocalFile());
		cerr << playlist->file(0).toString().toStdString() << endl;
		fooAudioEngine->getMediaObject()->play();
	}
	else
	{
		cerr << fooAudioEngine->getMediaObject()->state() << endl;
		cerr << "Inny state" << endl;
	}

//	emit playSignal();
}

void FooMainWindow::previous ()
{
	cerr << "FooMainWindow::previous" << endl;
	emit prevSignal ();
}

void FooMainWindow::next ()
{
	cerr << "FooMainWindow::next" << endl;
	emit nextSignal ();
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

void FooMainWindow::about ()
{
	(new FooAbout(this))->show();
}

int FooMainWindow::getMaxProgress()
{
	return maxProgress;
}
