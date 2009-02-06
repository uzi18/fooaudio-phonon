#include "footabbar.hpp"
#include "footabwidget.hpp"
#include <QMouseEvent>
#include <QApplication>

FooTabBar::FooTabBar (QWidget *parent) : QTabBar (parent)
{
	setAcceptDrops (true);
}

FooTabBar::~FooTabBar ()
{
}

void FooTabBar::mouseDoubleClickEvent (QMouseEvent *event)
{
	const QPoint pos = event->pos ();
	int tab = findTabUnder (pos);

	if (tab < count ())
	{
		emit mouseDoubleClickTab (tab);
	}
}

int FooTabBar::findTabUnder (const QPoint &pos)
{
	for (int i = 0; i < count (); i++)
	{
		if (tabRect (i).contains (pos))
		{
			return i;
		}
	}

	return -1;
}

void FooTabBar::mousePressEvent (QMouseEvent *event)
{
	if (event->button () == Qt::LeftButton)
	{
		int tabno = findTabUnder (event->pos ());

		dragStartPosition_ = event->pos ();
		dragTab_ = tabno;

		if (tabno != -1)
		{
			setCurrentIndex (tabno);
		}
	}

	event->accept ();
}

void FooTabBar::mouseMoveEvent (QMouseEvent *event)
{
	if (!(event->buttons () & Qt::LeftButton))
	{
		return;
	}

	if ((event->pos () - dragStartPosition_).manhattanLength () <QApplication::startDragDistance ())
	{
		return;
	}

	if (dragTab_ != -1)
	{
		QDrag *drag = new QDrag (this);
		QMimeData *mimeData = new QMimeData;
		QByteArray data;

		data.setNum (dragTab_);

		mimeData->setData (FOOTABDRAGMIMETYPE, data);
		drag->setMimeData (mimeData);

		Qt::DropAction dropAction = drag->start (Qt::MoveAction);
		Q_UNUSED (dropAction);
	}

	event->accept ();
}

void FooTabBar::contextMenuEvent (QContextMenuEvent *event)
{
	event->accept ();
	emit contextMenu (event, findTabUnder (event->pos ()));
}

void FooTabBar::wheelEvent (QWheelEvent *event)
{
	int numDegrees = event->delta () / 8;
	int numSteps = numDegrees / 15;

	int newIndex = currentIndex () - numSteps;

	while (newIndex < 0)
	{
		newIndex += count ();
	}

	newIndex = newIndex % count ();

	setCurrentIndex (newIndex);

	event->accept ();
}

void FooTabBar::dragEnterEvent (QDragEnterEvent *e)
{
	if (e->mimeData ()->hasFormat(FOOTABDRAGMIMETYPE))
	{
		if (e->source () == this)
		{
			e->setDropAction (Qt::MoveAction);
			e->accept ();
		}
		else
		{
			e->acceptProposedAction ();
		}
	}
	else
	{
		e->ignore ();
	}
}

void FooTabBar::dragMoveEvent (QDragMoveEvent *e)
{
	if (e->mimeData ()->hasFormat(FOOTABDRAGMIMETYPE))
	{
		if (e->source () == this)
		{
			e->setDropAction (Qt::MoveAction);
			e->accept ();
		}
		else
		{
			e->acceptProposedAction ();
		}
	}
	else
	{
		e->ignore ();
	}
}

void FooTabBar::dropEvent (QDropEvent *e)
{
	if (e->mimeData ()->hasFormat(FOOTABDRAGMIMETYPE))
	{
		QByteArray itemData = e->mimeData()->data(FOOTABDRAGMIMETYPE);
		int movingTabId = itemData.toUInt ();
		int destinationTabId;
		if (findTabUnder (e->pos ()) != -1)
		{
			destinationTabId = findTabUnder (e->pos ());
		}
		else
		{
			destinationTabId = count() - 1;
		}

		if (movingTabId != -1 && destinationTabId != movingTabId)
			moveTab(movingTabId, destinationTabId);
	}

	e->accept ();
}

void FooTabBar::moveTab (int from, int to)
{
	emit moveTabSignal (from, to);
}

