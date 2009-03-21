#ifndef _FOOTABWIDGET_HPP_
#define _FOOTABWIDGET_HPP_

#include <QTabWidget>
#include <QtGui>
#include "footabbar.hpp"

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
	void geometryChangeRequested(const QRect &);
	void menuBarVisibilityChangeRequested (bool);
	void statusBarVisibilityChangeRequested (bool);
	void toolBarVisibilityChangeRequested (bool);
	
public:
	enum Tab
	{
	   CurrentTab, NewTab
	};

	FooTabWidget (QWidget *parent = 0);

	FooTabBar *tabBar ()
	{
	   return m_tabBar;
	}

	void clear ();

	QAction *newTabAction () const;
	QAction *closeTabAction () const;
	QAction *recentlyClosedTabsAction () const;
	QAction *nextTabAction () const;
	QAction *previousTabAction () const;

	QWidget *lineEditStack () const;
	QLineEdit *currentLineEdit () const;
	QLineEdit *lineEdit (int) const;
	
public slots:
	void newTab ();
	void cloneTab (int index = -1);
	void closeTab (int index = -1);
	void closeOtherTabs (int);
	void nextTab ();
	void previousTab ();

private slots:
	void currentChanged (int);
	void openLastTab ();
	void aboutToShowRecentTabsMenu ();
	void aboutToShowRecentTriggeredAction (QAction *action);
	void lineEditReturnPressed ();
	void windowCloseRequested ();
	void moveTab (int, int);
	void geometryChangeRequestedCheck (const QRect &);

private:
	QLabel *animationLabel (int, bool);

	QAction *m_recentlyClosedTabAction;
	QAction *m_newTabAction;
	QAction *m_closeTabAction;
	QAction *m_nextTabAction;
	QAction *m_previousTabAction;

	QMenu *m_recentlyCLosedTabsMenu;

	static const int m_recentlyClosedTabsSize = 10;
	QCompleter *m_lineEditCompleter;
	QStackedWidget *m_lineEdits;
	FooTabBar *m_tabBar;
}

#endif // _FOOTABWIDGET_HPP_

