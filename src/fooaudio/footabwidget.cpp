
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

	connect(fpw, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			  this, SLOT(itemClicked(QTreeWidgetItem *, int)));
}

void FooTabWidget::cloneTab (int index)
{
	QString newName = QString(tr("Copy of %1")).arg(tabText(index));
	//FooPlaylistWidget *fpw = qobject_cast<FooPlaylistWidget *> (widget(index));
	FooPlaylistWidget *fpwCopy = new FooPlaylistWidget ();
	addTab (fpwCopy, newName);

	connect(fpwCopy, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			  this, SLOT(itemClicked(QTreeWidgetItem *, int)));

}

void FooTabWidget::closeTab (int index)
{
	if (count() > 1)
	{
		QWidget* current = widget(index);
		if (current==currentPlayingPlaylist)
		{
			setCurrentPlaylist(getCurrentPlaylistIndex()-1);
			setCurrentItem(0);
		}
		delete current;
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

void FooTabWidget::itemClicked(QTreeWidgetItem * item, int column)
{
	qDebug() << "FooTabWidget::itemClicked";
	currentPlayingItem = item;
	currentPlayingPlaylist = static_cast<FooPlaylistWidget *>(currentWidget());

	emit itemDoubleClickedSignal(item, column);
}

QUrl FooTabWidget::nextFile(bool repeat, bool follow)
{
	qDebug() << "FooTabWidget::nextFile";
	int c = count();

	qDebug() << "TabWidget: nextFile: c: " << c;

// 	for (int i = 0; i < c; i++)
// 	{
// 		qDebug() << "TabWidget: nextFile: for: i: " << i;
// 		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (widget(i));
// 		int index = wid->indexOfTopLevelItem(currentPlayingItem);
// 		int max = wid->topLevelItemCount();
// 		qDebug() << "TabWidget: nextFile: for: index: " << index;
// 		qDebug() << "TabWidget: nextFile: for: max: " << max;
// 
// 		if (index >= 0)
// 		{
// 			qDebug() << "TabWidget: nextFile: for: if: index >= 0";
// 
// 			if (index == (max - 1) && repeat)
// 			{
// 				qDebug() << "TabWidget: nextFile: for: if: repeat";
// 
// 				currentPlayingPlaylist = wid;
// 				currentPlayingItem = wid->topLevelItem(0);
// 				if (follow) wid->setCurrentItem(currentPlayingItem);
// 				return wid->file(0);
// 			}
// 			else if (index < (max - 1))
// 			{
// 				qDebug() << "TabWidget: nextFile: for: if: index < max";
// 
// 				currentPlayingPlaylist = wid;
// 				currentPlayingItem = wid->itemBelow(currentPlayingItem);
// 				if (follow) wid->setCurrentItem(currentPlayingItem);
// 				return wid->file(++index);
// 			}
// 			else if (index == (max - 1) && !repeat)
// 			{
// 				qDebug() << "TabWidget: nextFile: for: if: !repeat";
// 
// 				return QUrl();
// 			}
// 		}
// 	}

	return QUrl();
}

QUrl FooTabWidget::previousFile(bool repeat, bool follow)
{
	qDebug() << "FooTabWidget::previousFile";
	int c = count();

	qDebug() << "TabWidget: previousFile: c: " << c;

// 	for (int i = 0; i < c; i++)
// 	{
// 		qDebug() << "TabWidget: previousFile: for: i: " << i;
// 		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (widget(i));
// 		int index = wid->indexOfTopLevelItem(currentPlayingItem);
// 		int max = wid->topLevelItemCount();
// 		qDebug() << "TabWidget: previousFile: for: index: " << index;
// 		qDebug() << "TabWidget: previousFile: for: max: " << max;
// 
// 		if (index >= 0)
// 		{
// 			qDebug() << "TabWidget: previousFile: for: if: index >= 0";
// 
// 			if (index == 0 && repeat)
// 			{
// 				qDebug() << "TabWidget: previousFile: for: if: repeat";
// 
// 				currentPlayingPlaylist = wid;
// 				currentPlayingItem = wid->topLevelItem(max - 1);
// 				if (follow) wid->setCurrentItem(currentPlayingItem);
// 				return wid->file(max - 1);
// 			}
// 			else if (index > 0)
// 			{
// 				qDebug() << "TabWidget: previousFile: for: if: index > 0";
// 
// 				currentPlayingPlaylist = wid;
// 				currentPlayingItem = wid->itemAbove(currentPlayingItem);
// 				if (follow) wid->setCurrentItem(currentPlayingItem);
// 				return wid->file(--index);
// 			}
// 			else if (index == 0 && !repeat)
// 			{
// 				qDebug() << "TabWidget: previousFile: for: if: !repeat";
// 
// 				return QUrl();
// 			}
// 		}
// 	}

	return QUrl();
}

void FooTabWidget::cut (bool remove)
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	buffer.clear();

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

	qDebug() << "TabWidget: count: " << buffer.count();
	qDebug() << "TabWidget: size: " << buffer.size();

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
// 	qDebug() << "TabWidget: utwor :" << index;
// 	currentPlayingItem = currentPlayingPlaylist->topLevelItem(index);
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

	connect(fpw, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			  this, SLOT(itemClicked(QTreeWidgetItem *, int)));

}

void FooTabWidget::playlistRemoved(FooTrackList *playlist)
{
}
