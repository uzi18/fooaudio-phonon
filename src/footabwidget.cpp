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

//   new QShortcut(QKeySequence(Qt:CTRL | Qt::SHIFT | Qt::Key_T), this, SLOT(openLastTab()));

   connect(m_tabBar, SIGNAL(newTab()), this, SLOT(newTab()));
   connect(m_tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
//   connect(m_tabBar, SIGNAL(cloneTab(int)), this, SLOT(cloneTab(int)));
   connect(m_tabBar, SIGNAL(closeOtherTabs(int)), this, SLOT(closeOtherTabs(int)));

   setTabBar(m_tabBar);

   setDocumentMode(true);

   connect(m_tabBar, SIGNAL(tabMoved(int, int)), this, SLOT(moveTab(int, int)));

   // Actions
   m_newTabAction = new QAction(tr("New &Tab"), this);
   m_newTabAction->setShortcuts(QKeySequence::AddTab);
   connect(m_newTabAction, SIGNAL(triggered()), this, SLOT(newTab()));

   m_closeTabAction = new QAction(tr("&Close Tab"), this);
   connect(m_closeTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));

   m_nextTabAction = new QAction(tr("Show Next Tab"), this);
   connect(m_nextTabAction, SIGNAL(triggered()), this, SLOT(nextTab()));

   m_previousTabAction = new QAction(tr("Show Previous Tab"), this);
   connect(m_previousTabAction, SIGNAL(triggered()), this, SLOT(previousTab()));

   m_tabBar->setTabsClosable(false);
   m_tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

   connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

   newTab ();
}

void FooTabWidget::clear()
{
   // clear the recently closed tabs
}

FooTabBar *FooTabWidget::tabBar ()
{
   return m_tabBar;
}

// When index is -1 index chooses the current tab
void FooTabWidget::moveTab(int fromIndex, int toIndex)
{
}

void FooTabWidget::currentChanged(int index)
{
//   Q_ASSERT(m_lineEdits->count() == count());
//   m_lineEdits->setCurrentIndex(index);
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

