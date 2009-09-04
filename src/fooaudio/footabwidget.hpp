#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>
#include <QtGui>
#include "footabbar.hpp"

class FooTabBar;

class FooTabWidget : public QTabWidget
{
	Q_OBJECT

signals:
	// tabwidget signals
	void tabsChanged ();
	void lastTabClosed ();

	void itemDoubleClickedSignal(QTreeWidgetItem *, int);

public:
	FooTabWidget (QWidget *parent = 0);

	QAction *newTabAction () const;
	QAction *closeTabAction () const;
	QAction *nextTabAction () const;
	QAction *previousTabAction () const;



public slots:
	void newTab ();
//	void cloneTab (int index = -1);
	void closeTab (int index = -1);
	void closeOtherTabs (int);
	void nextTab ();
	void previousTab ();

	void itemClicked(QTreeWidgetItem *, int);

	QUrl nextFile(bool);

private:
	QAction *m_recentlyClosedTabAction;
	QAction *m_newTabAction;
	QAction *m_closeTabAction;
	QAction *m_nextTabAction;
	QAction *m_previousTabAction;

	FooTabBar *m_tabBar;

	QTreeWidgetItem * nowPlayingItem;
};

#endif // _FOOTABWIDGET_HPP_

