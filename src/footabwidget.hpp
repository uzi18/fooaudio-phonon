#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>
#include <QtGui>
#include "footabbar.hpp"

class FooTabBar;

/*
 *
 */

class FooTabWidget : public QTabWidget
{
   Q_OBJECT

signals:
	// tabwidget signals
	void tabsChanged ();
	void lastTabClosed ();

	// current tab signals
	void setCurrentTitle(const QString &);
	void showStatusBarMessage (const QString &);
	void linkHovered (const QString &);
	void loadProgress (int);
	void menuBarVisibilityChangeRequested (bool);
	void statusBarVisibilityChangeRequested (bool);
	void toolBarVisibilityChangeRequested (bool);
	
public:
	FooTabWidget (QWidget *parent = 0);

	FooTabBar *tabBar ();

	void clear ();

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

private slots:
	void currentChanged (int);
	void moveTab (int, int);

private:
	QLabel *animationLabel (int, bool);

	QAction *m_recentlyClosedTabAction;
	QAction *m_newTabAction;
	QAction *m_closeTabAction;
	QAction *m_nextTabAction;
	QAction *m_previousTabAction;

	FooTabBar *m_tabBar;
};

#endif // _FOOTABWIDGET_HPP_

