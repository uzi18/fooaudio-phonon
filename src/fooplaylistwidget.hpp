#ifndef _FOOPLAYLISTWIDGET_HPP_
#define _FOOPLAYLISTWIDGET_HPP_

#include <QWidget>
#include "footabwidget.hpp"

class FooPlaylistWidget : public FooTabWidget
{
	Q_OBJECT

public:
	FooPlaylistWidget (QWidget *parent = 0);

	void addPlaylist (QString);

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

#endif // _FOOPLAYLISTWIDGET_HPP_

