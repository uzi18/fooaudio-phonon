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

	newTab ();
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

void FooTabWidget::newTab()
{
	FooPlaylistWidget *fpw = new FooPlaylistWidget ();
	addTab (fpw, "dupa");

		connect(fpw, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
				 this, SLOT(itemClicked(QTreeWidgetItem *, int)));
}

void FooTabWidget::closeTab (int index)
{
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
	cout << "FooTabWidget::itemClicked" << endl << flush;
	nowPlayingItem = item;
	emit itemDoubleClickedSignal(item, column);
}

QUrl FooTabWidget::nextFile(bool repeat)
{
	cout << "FooTabWidget::nextFile" << endl << flush;
	int c = count();

	cout << "TabWidget: nextFile: c: " << c << endl << flush;

	for (int i = 0; i < c; i++)
	{
		cout << "TabWidget: nextFile: for: i: " << i << endl << flush;
		FooPlaylistWidget *wid = static_cast<FooPlaylistWidget *> (widget(i));
		int index = wid->indexOfTopLevelItem(nowPlayingItem);
		int max = wid->topLevelItemCount();
		cout << "TabWidget: nextFile: for: index: " << index << endl << flush;
		cout << "TabWidget: nextFile: for: max: " << max << endl << flush;

			if (index >= 0)
			{
				cout << "TabWidget: nextFile: for: if: index >= 0" << endl << flush;

				if (index == max && repeat)
				{
					cout << "TabWidget: nextFile: for: if: repeat" << endl << flush;

					nowPlayingItem = wid->topLevelItem(0);
					return wid->file(0);
				}
				else if (index < max)
				{
					cout << "TabWidget: nextFile: for: if: index < max" << endl << flush;

					nowPlayingItem = wid->itemBelow(nowPlayingItem);
					return wid->file(++i);
				}
				else if (index == max && !repeat)
				{
					cout << "TabWidget: nextFile: for: if: !repeat" << endl << flush;

					return QUrl();
				}
			}
	}

	return QUrl();
}
