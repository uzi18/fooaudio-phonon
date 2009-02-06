#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>
#include <QTabBar>
#include <QDragEnterEvent>
#include "footabbar.hpp"

class QVBoxLayout;
class QHBoxLayout;
class QToolButton;
class QStackedLayout;
class QMenu;

class FooTabWidget : public QWidget
{
	Q_OBJECT
	
public:
	FooTabWidget (QWidget *parent = 0);
	~FooTabWidget ();

	int count ();
	QWidget *currentPage ();
	int currentPageIndex ();
	QWidget *widget (int);
	void addTab (QWidget *, QString);
	void showPage (QWidget *);
	void showPageDirectly (QWidget *);

	void removePage (QWidget *);
	void removePage (int);
	void insertPage (int, QString, QWidget *);
	int getIndex (QWidget *);

public slots:
	void setCurrentPage (int);
	void removeCurrentPage ();
	void moveTab (int, int);

signals:
	void mouseDoubleClickTab (QWidget *);
	void mouseDoubleClickTabBar ();
	void tabBarContextMenu (QPoint, QContextMenuEvent *);
	void tabContextMenu (int, QPoint, QContextMenuEvent *);
	void currentChanged (QWidget *);

private slots:
	void mouseDoubleClickTab (int);
	void tab_currentChanged (int);
	void tab_contextMenu (QContextMenuEvent *, int);

private:
	QVector<QWidget*> widgets_;
	FooTabBar *tabBar_;
	QVBoxLayout *layout_;
	QHBoxLayout *barLayout_;
	QStackedLayout *stacked_;
};

#define FOOTABDRAGMIMETYPE "x-drag-drop/x-foo-tab-drag"

#endif // _FOOTABWIDGET_HPP_

