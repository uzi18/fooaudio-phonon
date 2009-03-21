#include "footabbar.hpp"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QMenu>
#include <QStyle>
#include <QUrl>

#include <QDebug>

FooTabShortcut::FooTabShortcut (int tab, const QKeySequence &key, QWidget *parent) : QShortcut (key, parent), m_tab (tab)
{
}

int FooTabShortbut::tab ()
{
	return m_tab;
}

FooTabBar::FooTabBar (QWidget *parent) : QTabBar (parent) 
{
   setContextMenuPolicy (Qt::CustomContexMenu);
   setAcceptDrops (true);
   setElideMode (Qt::ElideRight);
   setUsesScrollButtons (true);

   connect (this, SIGNAL (customContextMenuRequested (const QPoint &)), this, SLOT (contextMenuRequested (const QPoint &)));

   QString alt = QLatin1String ("Ctrl+%1");
   for (int i = 0; i< 10; i++)
   {
	  int key = i == 9 ? 0 : i + 1;
	  FooTabShortcut *fooTabShortCut = new FooTabShortcut (i, alt.arg (key), this);
	  connect (fooTabShortCut, SIGNAL (activated ()), this, SLOT (selectTabAction ()));
   }

   setMovable (true);
}

QTabBar::ButtonPosition FooTabBar::freeSide ()
{
   QTabBar::ButtonPosition side = (QTabBar::ButtonPosition) style ()->styleHint (QStyle::SH_TabBar_CloseButtonPosition, 0, this);
   side = (side == QTabBar::LeftSide) ? QTabBar::RightSide : QTabBar::LeftSide;

   return side;
}

void FooTabBar::selectTabAction ()
{
   int index = qobject_cast<TabShortcut *> (sender ())->tab ();
   setCurrentIndex (index);
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

	  action->seData (index);

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
	  int index = action->data ().toInt ();
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

void FooTabBar::mouseDoubleClickEvent (QMouseEvent *event)
{
   if (!childAt(event->pos ())
		 // Remove the line beloe when QTabWidget does not have a one pixel frame
		 && event->pos().y() < (y() + height()))
   {
	  emit newTab();
	  return;
   }

   QTabBar::mouseDoubleClickEvent (event);
}

void FooTabBar::mouseReleaseEvent (QMouseEvent *event)
{
   if (event->button() == Qt::MidButton)
   {
	  int index = tabAt (event->pos ());
	  if (index != -1)
	  {
		 emit closeTab(index);
		 return;
	  }
   }

   QTabBar::mouseReleaseEvent(event);
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

	  if ((event->pos () - m_dragStartPos).ManhattanLength () > QApplication::startDragDistance () && diffX < 3 && diffX > -3 && diffY < -10)
	  {
		 QDrag *drag = new QDrag (this);
		 QMimeData *mimeData = new QMimeData;
		 int index = tabAt (event->pos ());
		 mimeData->setText(tabTExt (index));
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
   setVisible((count ()) > 1);
}
