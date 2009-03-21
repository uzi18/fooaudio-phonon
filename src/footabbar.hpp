#ifndef _FOOTABBAR_HPP_
#define _FOOTABBAR_HPP_

#include <QTabBar>
#include "footabwidget.hpp"

class FooTabWidget;

/*
 * Tab bar with a few more features such as a context menu and shortcuts
 */

class FooTabBar : public QTabBar
{
	Q_OBJECT

signals:
	void newTab ();
	void cloneTab (int);
	void closeTab (int);
	void closeOtherTabs (int);

public:
	FooTabBar (QWidget *parent = 0);

	QTabBar::ButtonPosition freeSide ();

protected:
	void mouseDoubleClickEvent (QMouseEvent *);
	void mouseReleaseEvent (QMouseEvent *);
	void mousePressEvent (QMouseEvent *);
	void moiseMoveEvent (QMouseEvent *);

	QSize tabSizeHint (int) const;
	void tabInserted (int);
	void tabRemoved (int);

private slots:
	void selectTabAction ();
	void cloneTab ();
	void closeTab ();
	void closeOtherTabs ();
	void contextMenuRequested (const QPoint &);

private:
	void updateVisibility ();
	friend class TabWidget;

	QPoint m_dragStartPos;
};

#include <QShortcut>

/*
 * Shortcut to switch directly to a tab by index
 */

class FooTabShortcut : public QShortcut
{
	Q_OBJECT

public:
	int tab ();
	FooTabShortcut (int, const QKeySequence &, QWidget *);

private:
	int m_tab;
}

#endif // _FOOTABBAT_HPP_
