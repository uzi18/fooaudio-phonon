#include "footabwidget.hpp"

#include "foomainwindow.hpp"
#include "footabbat.hpp"

#include <QCompleter>
#include <QEvent>
#include <QMenu>

#include <QDebug>

FooTabWidget::FooTabWidget (QWidget *parent) : QTabWidget (parent), m_recentlyClosedTabsAction(0), m_newTabAction(0), m_closeTabAction(0), m_nextTabAction(0), m_previousTabAction(0), m_recentlyClosedTabsMenu(0), m_lineEditCompleter(0), m_lineEdits(0), m_tabBar(new FooTabBar (this))
{
   setElideMode(Qt::ElideRight);

   new QShortcut(QKeySequence(Qt:CTRL | Qt::SHIFT | Qt::Key_T), this, SLOT(openLastTab()));

   connect(m_tabBar, SIGNAL(newTab()), this, SLOT(newTab()));
   connect(m_tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
   connect(m_tabBar, SIGNAL(cloneTab(int)), this, SLOT(cloneTab(int)));
   connect(m_tabBar, SIGNAL(closeOtherTabs(int)), this, SLOT(closeOtherTabs(int)));

   setTabBar(m_tabBar);

   setDocumentMode(true);

   connect(m_tabBar, SIGNAL(tabMoved(int, int)), this, SLOT(moveTab(int, int)));

   // Actions
   m_newTabAction = new QAction(tr("New &Tab"), this);
   m_newTabAction->setShortcuts(QKeySequence::AddTab);
   connect(m_newTabAction, SIGNAL(triggered()), this, SLOT(newTab()));

   m_closeTabAction = new QAction(tr("&Close Tab"), this);
   m_closeTabAction->setShortcuts(QKeySequence::Close);
   connect(m_closeTabAction, SIGNAL(triggered()), this, SLOT(closeTab()));

   m_nextTabAction = new QAction(tr("Show Next Tab"), this);
   QList<QKeySequence> shortcuts;
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceRight));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
   shortcuts.append(tr("Ctrl-]"));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Less));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Tab));
   m_nextTabAction->setShortcuts(shortcuts);
   connect(m_nextTabAction, SIGNAL(triggered()), this, SLOT(nextTab()));

   m_previousTabAction = new QAction(tr("Show Previous Tab"), this);
   shortcuts.clear();
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceLeft));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
   shortcuts.append(tr("Ctrl-["));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Greater));
   shortcuts.append(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Tab));
   m_previousTabAction->setShortcuts(shortcuts);
   connect(m_previousTabAction, SIGNAL(triggered()), this, SLOT(previousTab()));

   m_recentlyClosedTabsMenu = new QMenu(this);
   connect(m_recentlyClosedTabsMenu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowRecentTabsMenu()));
   connect(m_recentlyClosedTabsMenu, SIGNAL(triggered(QAction *)), this, SLOT(aboutToShowRecentTriggeredAction(QAction *)));
   m_recentlyClosedTabsAction = new QAction(tr("Recently Closed Tabs"), this);
   m_recentlyClosedTabsAction->setMenu(m_recentlyClosedTabsMenu);
   m_recentlyClosedTabsAction->setEnabled(false);

   m_tabBar->setTabsClosable(true);
   connect(m_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
   m_tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

   connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

   m_lineEdits = new QStackedWidget(this);
}

void FooTabWidget::clear()
{
   // clear the recently closed tabs
   m_recentlyClosedTabs.clear();
   m_recentlyClosedTabsAction->setEnabled(false);
   // clear the line edit history
   for (int i = 0; i < m_lineEdits->count(); ++i)
   {
	  QLineEdit *qLineEdit = lineEdit(i);
	  qLineEdit->setText(qLineEdit->text());
   }
}

// When index is -1 index chooses the current tab
void FooTabWidget::moveTab(int fromIndex, int toIndex)
{
   QWidget *lineEdit = m_lineEdits->widget(fromIndex);
   m_lineEdits->removeWidget(lineEdit);
   m_lineEdits->insertWidget(toIndex, lineEdit);
}

void FooTabWidget::currentChanged(int index)
{
   Q_ASSERT(m_lineEdits->count() == count());
   m_lineEdits->setCurrentIndex(index);
}

QAction *FooTabWidget::newTabAction() const
{
   return m_newTabAction;
}

QAction *FooTabWidget::closeTabAction() const
{
   return m_closeTabAction;
}

QAction *FooTabWidget::recentlyClosedTabsAction() const
{
   return m_recentlyClosedTabsAction;
}

QAction *FooTabWidget::nextTabAction() const
{
   return m_nextTabAction;
}

QAction *FooTabWidget::previousTabAction() const
{
   return m_previousTabAction;
}

QWidget *FooTabWidget::lineEditStack() const
{
   return m_lineEdits;
}

QLineEdit *FooTabWidget::currentLineEdit() const
{
   return lineEdit(m_lineEdits->currentIndex());
}

void FooTabWidget::newTab()
{
   makeNewTab(true);
}

FooMainWindow *FooTabWidget::mainWindow()
{
   QWidget *w = this->parentWidget();
   while (w)
   {
	  if (FooMainWindow *mw = qobject_cast<FooMainWindow*>(w))
	  {
		 return mw;
	  }
	  w = w->parentWidget();
   }

   return 0;
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

void FooTabWidget::aboutToShowRecentTabsMenu()
{
   m_recentlyClosedTabsMenu->clear();

   for (int i = 0; i < m_recentlyClosedTabs.count(); ++i)
   {
	  QAction *action = new QAction(m_recentlyClosedTabsMenu);
	  action->setData(m_recentlyClosedTabs.at(i));
	  m_recentlyClosedTabsMenu->addAction(action);
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
}

