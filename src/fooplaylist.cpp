#include <QtGui>
#include "fooplaylist.hpp"

FooPlaylist::FooPlaylist (QWidget *parent) : FooTabWidget (parent)
{
	tabBarMenu = new QMenu;
	tabMenu = new QMenu;

	connect (this, SIGNAL (mouseDoubleClickTabBar ()), SLOT (newPlaylist ()));
	connect (this, SIGNAL (mouseDoubleClickTab (QWidget *)), SLOT (renamePlaylist ()));
	connect (this, SIGNAL (tabBarContextMenu (QPoint, QContextMenuEvent *)), SLOT (showTabBarMenu (QPoint, QContextMenuEvent *)));
	connect (this, SIGNAL (tabContextMenu (int, QPoint, QContextMenuEvent *)), SLOT (showTabMenu (int, QPoint, QContextMenuEvent *)));

	setupTabs ();
}

void FooPlaylist::addPlaylist ()
{
	QWidget *widget = new QWidget;
	addTab (widget, tr ("Default"));
}

void FooPlaylist::removePlaylist ()
{
	;
}

void FooPlaylist::renamePlaylist ()
{
	;
}

void FooPlaylist::newPlaylist ()
{
	addPlaylist ();
}

void FooPlaylist::showTabMenu (int tab, QPoint pos, QContextMenuEvent *event)
{
	Q_UNUSED (event);
	tabMenu->clear ();

	QAction *rename = tabMenu->addAction (tr ("Rename playlist"));
	QAction *remove = tabMenu->addAction (tr ("Remove playlist"));
	QAction *add = tabMenu->addAction (tr ("Add new playlist"));
	tabMenu->addSeparator ();
	QAction *left = tabMenu->addAction (tr ("Move left"));
	QAction *right = tabMenu->addAction (tr ("Move right"));
	tabMenu->addSeparator ();
	QAction *save_all = tabMenu->addAction (tr ("Save all playlist"));
	QAction *save = tabMenu->addAction (tr ("Save playlist"));

	QAction *act = tabMenu->exec (pos);

	if (!act)
	{
		return;
	}
}

void FooPlaylist::showTabBarMenu (QPoint pos, QContextMenuEvent *event)
{
	Q_UNUSED (event);
	tabBarMenu->clear ();

	QAction *add = tabBarMenu->addAction (tr ("Add new playlist"));
	QAction *save_all = tabBarMenu->addAction (tr ("Save all playlist"));
	QAction *load = tabBarMenu->addAction (tr ("Load playlist"));

	QAction *act = tabBarMenu->exec (pos);

	if (!act)
	{
		return;
	}
	if (act == add)
	{
		addPlaylist ();
	}
}

void FooPlaylist::setupTabs ()
{
	QWidget *widget = new QWidget;
	QLabel *aaa = new QLabel ("aaa", widget);
	addTab (widget, tr ("Nowy"));
}


