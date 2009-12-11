
#include "footabwidget.hpp"
#include "footabbar.hpp"
#include "fooplaylistmanager.hpp"
#include "tracklistmodel.hpp"
#include "fooplaylistwidget.hpp"
#include "footracklist.hpp"

#include <QCompleter>
#include <QEvent>
#include <QMenu>

#include <QtDebug>

FooTabWidget::FooTabWidget (QWidget *parent) : QTabWidget (parent), m_newTabAction(0), m_closeTabAction(0), m_nextTabAction(0), m_previousTabAction(0), m_tabBar(new FooTabBar (this))
{
	setElideMode(Qt::ElideRight);

	connect(m_tabBar, SIGNAL(newTab()), this, SLOT(newTab()));
	connect(m_tabBar, SIGNAL(cloneTab(int)), this, SLOT(cloneTab(int)));
	connect(m_tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
	connect(m_tabBar, SIGNAL(closeOtherTabs(int)), this, SLOT(closeOtherTabs(int)));

	setTabBar(m_tabBar);

	setDocumentMode(true);

	// Actions
	m_newTabAction = new QAction(tr("New &Tab"), this);
	connect(m_newTabAction, SIGNAL(triggered()), this, SLOT(newTab()));

	m_closeTabAction = new QAction(tr("&Close Tab"), this);
	connect(m_closeTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));

	m_nextTabAction = new QAction(tr("Show Next Tab"), this);
	connect(m_nextTabAction, SIGNAL(triggered()), this, SLOT(nextTab()));

	m_previousTabAction = new QAction(tr("Show Previous Tab"), this);
	connect(m_previousTabAction, SIGNAL(triggered()), this, SLOT(previousTab()));

	m_tabBar->setTabsClosable(false);
	m_tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
	
	
	connect(FooPlaylistManager::instance(), SIGNAL(playlistAdded(FooTrackList*)),
		this, SLOT(playlistAdded(FooTrackList*)));
	connect(FooPlaylistManager::instance(), SIGNAL(playlistRemoved(FooTrackList*)),
		this, SLOT(playlistRemoved(FooTrackList*)));
	connect(FooPlaylistManager::instance(), SIGNAL(currentPlaylistChanged(FooTrackList*)),
		this, SLOT(currentPlaylistChanged(FooTrackList*)));
	connect(this, SIGNAL(currentChanged(int)),
		FooPlaylistManager::instance(), SLOT(currentTabChanged(int)));

}

QAction *FooTabWidget::newTabAction() const
{
	return m_newTabAction;
}

QAction *FooTabWidget::closeTabAction() const
{
	return m_closeTabAction;
}

QAction *FooTabWidget::nextTabAction() const
{
	return m_nextTabAction;
}

QAction *FooTabWidget::previousTabAction() const
{
	return m_previousTabAction;
}

void FooTabWidget::newTab(QString name)
{
	FooPlaylistWidget *fpw = new FooPlaylistWidget ();
	addTab (fpw, (name.isEmpty() ? "New Playlist" : name));

	connect(fpw, SIGNAL(doubleClicked ( const QModelIndex & index )),
			  this, SLOT(doubleClicked ( const QModelIndex & index )));
}

void FooTabWidget::cloneTab (int index)
{
	QString newName = QString(tr("Copy of %1")).arg(tabText(index));
	//FooPlaylistWidget *fpw = qobject_cast<FooPlaylistWidget *> (widget(index));
	FooPlaylistWidget *fpwCopy = new FooPlaylistWidget ();
	addTab (fpwCopy, newName);

	connect(fpwCopy, SIGNAL(doubleClicked ( const QModelIndex & index )),
			  this, SLOT(doubleClicked ( const QModelIndex & index )));

}

void FooTabWidget::closeTab (int index)
{
	if (count() > 1)
	{
		QWidget* current = widget(index);

		if (FooPlaylistManager::instance()->currentPlaylistIndex() == index)
		{
			emit currentChanged(index - 1);
			FooPlaylistManager::instance()->useSelectedPlaylist();
		}
		delete current;
		FooPlaylistManager::instance()->deletePlaylist(index);
	}
}

void FooTabWidget::closeOtherTabs(int index)
{
	if (-1 == index)
	{
		return;
	}

	for (int i = count() - 1; i > index; --i)
	{
		closeTab(i);
	}
	for (int i = index - 1; i >= 0; --i)
	{
		closeTab(i);
	}
}

void FooTabWidget::nextTab()
{
	int next = currentIndex() + 1;
	if (next == count())
	{
		next = 0;
	}
	setCurrentIndex(next);
}

void FooTabWidget::previousTab()
{
	int next = currentIndex() - 1;
	if (next < 0)
	{
		next = count() - 1;
	}
	setCurrentIndex(next);
}

// 
void FooTabWidget::cut (bool remove)
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

//	buffer.clear();

// 	foreach (QTreeWidgetItem * item, foo->selectedItems())
// 	{
// 		if (item)
// 		{
// 			qDebug() << "TabWidget: bufor2 :" << item->text(0);
// 			buffer.append(item->clone());
// 
// 			if (remove)
// 				delete item;
// 		}
// 	}
}

void FooTabWidget::remove ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

// 	foreach (QTreeWidgetItem * item, foo->selectedItems())
// 	{
// 		if (item)
// 		{
// 			delete item;
// 		}
// 	}
}

void FooTabWidget::copy ()
{
	cut(false);
}

void FooTabWidget::paste ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	//qDebug() << "TabWidget: count: " << buffer.count();
	//qDebug() << "TabWidget: size: " << buffer.size();

/*	int line = foo->indexOfTopLevelItem(foo->currentItem ());
	qDebug() << "TabWidget: line :" << line;
	if (line < 0)
		line = foo->plistCount();
	else
		line++;

	foreach (QTreeWidgetItem * item, buffer)
	{
		if (item)
		{
			qDebug() << "TabWidget: bufor2 :" << item->text(0);
			// if line not selected
			foo->insertTopLevelItem(line, item->clone());
			line++;
		}
	}*/
}

void FooTabWidget::clear ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

// 	foo->clear ();
}

void FooTabWidget::selectAll ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

// 	foo->selectAll ();
}

void FooTabWidget::setCurrentPlaylist(int index)
{
	qDebug() << "TabWidget: playlista :" << index;
// 	currentPlayingPlaylist = static_cast<FooPlaylistWidget *> (widget(index));
// 	setCurrentIndex(index);
}

int FooTabWidget::getCurrentPlaylistIndex()
{
// 	qDebug() << "TabWidget: playlista :" << indexOf(currentPlayingPlaylist);
// 	return indexOf(currentPlayingPlaylist);
}

void FooTabWidget::setCurrentItem(int index)
{
//	qDebug() << "TabWidget: utwor :" << index;
//	currentPlayingItem = currentPlayingPlaylist->topLevelItem(index);
// 	currentPlayingPlaylist->setCurrentItem(currentPlayingItem);
}

int FooTabWidget::getCurrentItemIndex()
{
// 	qDebug() << "TabWidget: utwor :" << currentPlayingPlaylist->indexOfTopLevelItem(currentPlayingItem);
// 	return currentPlayingPlaylist->indexOfTopLevelItem(currentPlayingItem);
}

void FooTabWidget::playlistAdded(FooTrackList *playlist)
{
	qDebug() << "TabWidget: playlista :" << playlist->name();

	FooPlaylistWidget *fpw = new FooPlaylistWidget (this);
	fpw->setModel(new TrackListModel(playlist, this));
	addTab (fpw, (playlist->name()));

	connect(fpw, SIGNAL(doubleClicked ( const QModelIndex & index )),
			  this, SLOT(doubleClicked ( const QModelIndex & index )));

}

void FooTabWidget::playlistRemoved(FooTrackList *playlist)
{
}

void FooTabWidget::currentPlaylistChanged(FooTrackList *playlist)
{
	if (!playlist)
		return;
	
	setCurrentIndex(FooPlaylistManager::instance()->playlist(playlist));
}
