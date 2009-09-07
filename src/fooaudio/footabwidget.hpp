#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>
#include <QtGui>
#include "footabbar.hpp"
#include "fooplaylistwidget.hpp"

class FooTabBar;

class FooTabWidget : public QTabWidget
{
	Q_OBJECT

signals:
	// tabwidget signals
	void tabsChanged ();
	void lastTabClosed ();

	void itemDoubleClickedSignal(QTreeWidgetItem *item, int column);

public:
	FooTabWidget (QWidget *parent = 0);

	QAction *newTabAction () const;
	QAction *closeTabAction () const;
	QAction *nextTabAction () const;
	QAction *previousTabAction () const;

	QTreeWidgetItem * nowPlayingItem;

public slots:
	void newTab (QString name = QString());
	void cloneTab (int index = -1);
	void closeTab (int index = -1);
	void closeOtherTabs (int index);
	void nextTab ();
	void previousTab ();

	void itemClicked(QTreeWidgetItem *, int);

	QUrl nextFile(bool repeat);
	QUrl previousFile(bool repeat);

	void cut (bool remove = true);
	void remove ();
	void copy ();
	void paste ();
	void clear ();
	void selectAll ();

private:
	QAction *m_recentlyClosedTabAction;
	QAction *m_newTabAction;
	QAction *m_closeTabAction;
	QAction *m_nextTabAction;
	QAction *m_previousTabAction;

	FooTabBar *m_tabBar;

	FooPlaylistWidget buffer;
};

#endif // _FOOTABWIDGET_HPP_

