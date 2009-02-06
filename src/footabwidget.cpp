#include "footabwidget.hpp"
#include "footabbar.hpp"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedLayout>
#include <QStyle>
#include <QApplication>
#include <QMenu>
#include <QDebug>

FooTabWidget::FooTabWidget (QWidget *parent) : QWidget (parent)
{
	tabBar_ = new FooTabBar (this);
	tabBar_->setUsesScrollButtons (true);
//	setTabPosition(QTabWidget::North);

	layout_ = new QVBoxLayout(this);
	layout_->setMargin(0);
	layout_->setSpacing(0);
	
	barLayout_ = new QHBoxLayout ();
	barLayout_->setMargin(0);
	barLayout_->setSpacing(0);
	barLayout_->addWidget(tabBar_, 2);
	barLayout_->setAlignment(Qt::AlignLeft);
	layout_->addLayout (barLayout_);

	stacked_ = new QStackedLayout(layout_);

	connect (tabBar_, SIGNAL (mouseDoubleClickTab (int)), this, SLOT (mouseDoubleClickTab (int)));
	connect (tabBar_, SIGNAL (contextMenu(QContextMenuEvent *, int)), this, SLOT (tab_contextMenu (QContextMenuEvent *, int)));
	connect (tabBar_, SIGNAL (currentChanged (int)), this, SLOT (tab_currentChanged (int)));
	connect (tabBar_, SIGNAL (moveTabSignal (int, int)), this, SLOT (moveTab (int, int)));
}

FooTabWidget::~FooTabWidget ()
{
}

// liczba tabów/widgetów
int FooTabWidget::count()
{
	return tabBar_->count();
}

// Zwraca widget z aktualnego tabu
QWidget *FooTabWidget::currentPage ()
{
	if (currentPageIndex () == -1)
	{
		return 0;
	}
	
	return widgets_[currentPageIndex ()];
}

void FooTabWidget::tab_currentChanged (int tab)
{
// qt 4.4 sends -1 i case of an empty QTabbar, ignore that case.
	if (tab == -1)
	{
		return;
	}
	setCurrentPage (tab);
	emit currentChanged (currentPage ());
}

//zwraca indeks aktualnego tabu
int FooTabWidget::currentPageIndex ()
{
	return tabBar_->currentIndex ();
}

QWidget *FooTabWidget::widget (int index)
{
	return widgets_[index];
}

// dodanie widgetu do stosu
void FooTabWidget::addTab(QWidget *widget, QString name)
{
	Q_ASSERT(widget);
	if (widgets_.contains (widget))
	{
		return;
	}
	widgets_.append (widget);
	stacked_->addWidget (widget);
	tabBar_->addTab (name);
	showPage (currentPage ());
}

// wybranie strony dla zdeyfiniowanego widgetu (???)
void FooTabWidget::showPage (QWidget* widget)
{
	for (int i = 0; i < count (); i++)
	{
		if (widgets_[i] == widget)
		{
			showPageDirectly (widget);
			tabBar_->setCurrentIndex (i);
		}
	}
}

// ja + jakiś internal helper (?????)
void FooTabWidget::showPageDirectly (QWidget* widget)
{
	// FIXME move this back into showPage? should this be in the public interface?
	for (int i = 0; i < count (); i++)
	{
		if (widgets_[i] == widget)
		{
			stacked_->setCurrentWidget (widget);
			// currentChanged is handled by tabBar_
			return;
		}

	}
}

// usunięcie strony dla specyficznego widgetu
void FooTabWidget::removePage (QWidget* widget)
{
	for (int i = 0; i < count (); i++)
	{
		if (widgets_[i] == widget)
		{
			stacked_->removeWidget (widget);
			widgets_.remove (i);
			tabBar_->removeTab (i);
		}
	}
}

void FooTabWidget::removePage (int index)
{
	stacked_->removeWidget (widget (index));
	widgets_.remove (index);
	tabBar_->removeTab (index);
}

void FooTabWidget::insertPage (int index, QString name, QWidget *wid)
{
	tabBar_->insertTab (index, name);
	widgets_.insert (index, wid);
	stacked_->insertWidget (index, wid);
}

void FooTabWidget::moveTab (int from, int to)
{
	QString name = tabBar_->tabText (from);
	QWidget *wid = new QWidget;
	wid = widget (from);
	blockSignals (true);
	this->removePage (from);
	this->insertPage (to, name, wid);
	tabBar_->setCurrentIndex (to);
	this->setCurrentPage (to);
	blockSignals (false);
//	delete wid;
}

// znajdowanie indeksu 
int FooTabWidget::getIndex (QWidget* widget)
{
	for (int i = 0; i < count (); i++)
	{
		if (widgets_[i] == widget)
		{
			return i;
		}
	}
	return -1;
}

void FooTabWidget::setCurrentPage (int index)
{
	Q_ASSERT (index >= 0 && index < count());
	showPage (widgets_[index]);
}

void FooTabWidget::removeCurrentPage ()
{
	removePage (currentPage ());
}

void FooTabWidget::mouseDoubleClickTab (int tab)
{
	if (tab >= 0)
	{
		emit mouseDoubleClickTab (widget (tab));
	}
	else
	{
		emit mouseDoubleClickTabBar ();
	}
}

void FooTabWidget::tab_contextMenu (QContextMenuEvent *event, int tab)
{
	if (tab >= 0)
	{
		emit tabContextMenu (tab, tabBar_->mapToGlobal (event->pos ()), event);
	}
	else
	{
		emit tabBarContextMenu (tabBar_->mapToGlobal (event->pos ()), event);
	}
}

