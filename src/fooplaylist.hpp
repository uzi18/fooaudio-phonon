#ifndef _FOOPLAYLIST_HPP_
#define _FOOPLAYLIST_HPP_

#include <QWidget>
#include "footabwidget.hpp"

class FooPlaylist : public FooTabWidget
{
	Q_OBJECT

public:
	FooPlaylist (QWidget *parent = 0);

	void addPlaylist ();

protected slots:
	void removePlaylist ();
	void renamePlaylist ();
	void newPlaylist ();
	void showTabMenu (int, QPoint, QContextMenuEvent *);
	void showTabBarMenu (QPoint, QContextMenuEvent *);

private:
	QMenu *tabBarMenu;
	QMenu *tabMenu;

	void setupTabs ();
};

#endif // _FOOPLAYLIST_HPP_

