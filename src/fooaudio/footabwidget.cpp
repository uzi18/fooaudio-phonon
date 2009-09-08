#include <iostream>

using namespace std;

#include "footabwidget.hpp"
#include "footabbar.hpp"
#include "fooplaylistwidget.hpp"

#include <QCompleter>
#include <QEvent>
#include <QMenu>

#include <QDebug>

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
	//FooPlaylistWidget *fpw = qobject_cast<FooPlaylistWidget *> widget(index);
	//FooPlaylistWidget *fpw2 = new FooPlaylistWidget (fpw);
	//delete current;
}

void FooTabWidget::closeTab (int index)
{
	QWidget* current = widget(index);
	delete current;

	if (count() == 0)
	{
		newTab("Default");
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
	cerr << "FooTabWidget::itemClicked" << endl;
	currentPlayingItem = item;
	currentPlayingPlaylist = static_cast<FooPlaylistWidget *>(currentWidget());

	emit itemDoubleClickedSignal(item, column);
}

QUrl FooTabWidget::nextFile(bool repeat)
{
	cerr << "FooTabWidget::nextFile" << endl;
	int c = count();

	cerr << "TabWidget: nextFile: c: " << c << endl;

	for (int i = 0; i < c; i++)
	{
		cout << "TabWidget: nextFile: for: i: " << i << endl;
		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (widget(i));
		int index = wid->indexOfTopLevelItem(currentPlayingItem);
		int max = wid->topLevelItemCount();
		cerr << "TabWidget: nextFile: for: index: " << index << endl;
		cerr << "TabWidget: nextFile: for: max: " << max << endl;

			if (index >= 0)
			{
				cerr << "TabWidget: nextFile: for: if: index >= 0" << endl;

				if (index == (max - 1) && repeat)
				{
					cerr << "TabWidget: nextFile: for: if: repeat" << endl;

					currentPlayingPlaylist = wid;
					currentPlayingItem = wid->topLevelItem(0);
					wid->setCurrentItem(currentPlayingItem);
					return wid->file(0);
				}
				else if (index < (max - 1))
				{
					cerr << "TabWidget: nextFile: for: if: index < max" << endl;

					currentPlayingPlaylist = wid;
					currentPlayingItem = wid->itemBelow(currentPlayingItem);
					wid->setCurrentItem(currentPlayingItem);
					return wid->file(++index);
				}
				else if (index == (max - 1) && !repeat)
				{
					cerr << "TabWidget: nextFile: for: if: !repeat" << endl;

					return QUrl();
				}
			}
	}

	return QUrl();
}

QUrl FooTabWidget::previousFile(bool repeat)
{
	cerr << "FooTabWidget::previousFile" << endl;
	int c = count();

	cerr << "TabWidget: previousFile: c: " << c << endl;

	for (int i = 0; i < c; i++)
	{
		cout << "TabWidget: previousFile: for: i: " << i << endl;
		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (widget(i));
		int index = wid->indexOfTopLevelItem(currentPlayingItem);
		int max = wid->topLevelItemCount();
		cerr << "TabWidget: previousFile: for: index: " << index << endl;
		cerr << "TabWidget: previousFile: for: max: " << max << endl;

			if (index >= 0)
			{
				cerr << "TabWidget: previousFile: for: if: index >= 0" << endl;

				if (index == 0 && repeat)
				{
					cerr << "TabWidget: previousFile: for: if: repeat" << endl;

					currentPlayingPlaylist = wid;
					currentPlayingItem = wid->topLevelItem(wid->topLevelItemCount());
					wid->setCurrentItem(currentPlayingItem);
					return wid->file(wid->topLevelItemCount() - 1);
				}
				else if (index > 0)
				{
					cerr << "TabWidget: previousFile: for: if: index > 0" << endl;

					currentPlayingPlaylist = wid;
					currentPlayingItem = wid->itemAbove(currentPlayingItem);
					wid->setCurrentItem(currentPlayingItem);
					return wid->file(--index);
				}
				else if (index == 0 && !repeat)
				{
					cerr << "TabWidget: previousFile: for: if: !repeat" << endl;

					return QUrl();
				}
			}
	}

	return QUrl();
}

void FooTabWidget::cut (bool remove)
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	buffer.clear();

	foreach (QTreeWidgetItem * item, foo->selectedItems())
	{
		if (item)
		{
			cerr << "TabWidget: bufor2 :" << item->text(0).toStdString() << endl;
			buffer.append(item->clone());

			if (remove)
				delete item;
		}
	}
}

void FooTabWidget::remove ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	foreach (QTreeWidgetItem * item, foo->selectedItems())
	{
		if (item)
		{
			delete item;
		}
	}
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

	cerr << "TabWidget: count: " << buffer.count() << endl;
	cerr << "TabWidget: size: " << buffer.size() << endl;

	int line = foo->indexOfTopLevelItem(foo->currentItem ());
	cerr << "TabWidget: line :" << line << endl;
	if (line < 0)
		line = foo->plistCount();
	else
		line++;

	foreach (QTreeWidgetItem * item, buffer)
	{
		if (item)
		{
			cerr << "TabWidget: bufor2 :" << item->text(0).toStdString() << endl;
			// if line not selected
			foo->insertTopLevelItem(line, item->clone());
			line++;
		}
	}
}

void FooTabWidget::clear ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	foo->clear ();
}

void FooTabWidget::selectAll ()
{
	FooPlaylistWidget * foo = static_cast<FooPlaylistWidget *> (currentWidget());
	if (!foo)
		return;

	foo->selectAll ();
}

void FooTabWidget::setCurrentPlaylist(int index)
{
  	cerr << "TabWidget: playlista :" << index << endl;
	currentPlayingPlaylist = static_cast<FooPlaylistWidget *> (widget(index));
	setCurrentIndex(index);
}

int FooTabWidget::getCurrentPlaylistIndex()
{
	cerr << "TabWidget: playlista :" << indexOf(currentPlayingPlaylist) << endl;
	return indexOf(currentPlayingPlaylist);
}

void FooTabWidget::setCurrentItem(int index)
{
  	cerr << "TabWidget: utwor :" << index << endl;
	currentPlayingItem = currentPlayingPlaylist->topLevelItem(index);
	currentPlayingPlaylist->setCurrentItem(currentPlayingItem);
}

int FooTabWidget::getCurrentItemIndex()
{
  	cerr << "TabWidget: utwor :" << currentPlayingPlaylist->indexOfTopLevelItem(currentPlayingItem) << endl;
	return currentPlayingPlaylist->indexOfTopLevelItem(currentPlayingItem);
}
