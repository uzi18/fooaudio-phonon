#include <iostream>
using namespace std;

#include "footabbar.hpp"
#include "foochangename.hpp"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QMenu>
#include <QStyle>
#include <QUrl>

#include <QDebug>

FooTabBar::FooTabBar (QWidget *parent) : QTabBar (parent), m_showTabBarWhenOneTab(true)
{
	setContextMenuPolicy (Qt::CustomContextMenu);
	setAcceptDrops (true);
	setElideMode (Qt::ElideRight);
	setUsesScrollButtons (true);

	connect (this, SIGNAL (customContextMenuRequested (const QPoint &)), this, SLOT (contextMenuRequested (const QPoint &)));

	setMovable (true);
}

void FooTabBar::contextMenuRequested (const QPoint &position)
{
	QMenu menu;
	menu.addAction (tr ("New &Tab"), this, SIGNAL (newTab ()), QKeySequence::AddTab);

	int index = tabAt (position);

	if (-1 != index)
	{
	  QAction *action = menu.addAction (tr ("DuplicateTab"), this, SLOT (cloneTab ()));
	  action->setData (index);

	  menu.addSeparator ();

	  action = menu.addAction (tr ("&Close Tab"), this, SLOT (closeTab ()), QKeySequence::Close);
	  action->setData (index);

	  menu.addSeparator ();

	  action = menu.addAction (tr ("&Close Other Tabs"), this, SLOT (closeOtherTabs()), QKeySequence::Close);
	  action->setData (index);

	  menu.addSeparator ();

	  action = menu.addAction (tr ("&Rename Tab"), this, SLOT (renameTab ()));
	  action->setData (index);

	  menu.addSeparator ();

	  action->setData (index);

	  menu.addSeparator ();

	}
	else
	{
	  menu.addSeparator ();
	}

	menu.exec (QCursor::pos ());
}

void FooTabBar::cloneTab ()
{
	if (QAction *action = qobject_cast<QAction *> (sender ()))
	{
	  int index = action->data().toInt();
	  emit cloneTab(index);
	}
}

void FooTabBar::closeTab()
{
	if (QAction *action = qobject_cast<QAction *> (sender ()))
	{
		cerr << "FooTabBar::closeTab" << endl;
	  int index = action->data().toInt();
	  emit closeTab (index);
	}
}

void FooTabBar::closeOtherTabs()
{
	if (QAction *action = qobject_cast<QAction*> (sender()))
	{
	  int index = action->data().toInt();
	  emit closeOtherTabs(index);
	}
}

void FooTabBar::renameTab ()
{
	if (QAction *action = qobject_cast<QAction *> (sender ()))
	{
	  int index = action->data().toInt();
	  emit renameTab(index);
	}
}

void FooTabBar::mouseDoubleClickEvent (QMouseEvent *event)
{
	// Remove the line beloe when QTabWidget does not have a one pixel frame
	//  && event->pos().y() < (y() + height()))
	if (!childAt(event->pos ()))
	{
		int i = tabAt(event->pos());
		if (i < 0)
		{
			emit newTab();
		}
		else
		{
			FooChangeName *fooChangeName = new FooChangeName(i, this);
		}

		return;
	}

	QTabBar::mouseDoubleClickEvent (event);
}

void FooTabBar::mousePressEvent (QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
	  m_dragStartPos = event->pos();
	}

	QTabBar::mousePressEvent (event);
}

void FooTabBar::mouseMoveEvent (QMouseEvent *event)
{
	if (event->buttons () == Qt::LeftButton)
	{
	  int diffX = event->pos().x() - m_dragStartPos.x();
	  int diffY = event->pos().y() - m_dragStartPos.y();

	  if ((event->pos () - m_dragStartPos).manhattanLength () > QApplication::startDragDistance () && diffX < 3 && diffX > -3 && diffY < -10)
	  {
		 QDrag *drag = new QDrag (this);
		 QMimeData *mimeData = new QMimeData;
		 int index = tabAt (event->pos ());
		 mimeData->setText(tabText (index));
		 mimeData->setData(QLatin1String("action"), "tab-reordering");
		 drag->setMimeData(mimeData);
		 drag->exec();
	  }
	}

	QTabBar::mouseMoveEvent (event);
}

QSize FooTabBar::tabSizeHint (int index) const
{
	QSize sizeHint=QTabBar::tabSizeHint(index);
	QFontMetrics fm = fontMetrics ();

	return sizeHint.boundedTo(QSize(fm.width(QLatin1Char('M')) * 18, sizeHint.height()));
}

void FooTabBar::tabInserted (int position)
{
	Q_UNUSED(position);
	updateVisibility ();
}

void FooTabBar::tabRemoved (int position)
{
	Q_UNUSED(position);
	updateVisibility();
}

void FooTabBar::updateVisibility()
{
//   setVisible(true);
//   m_viewTabBarAction->setEnabled(count() == 1);
//   updateViewToolAction();
}
