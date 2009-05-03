#include "fooplaylistwidget.hpp"

#include <QStringList>
#include <QLabel>
#include <QDebug>

#include <iostream>

using namespace std;

FooPlaylistWidget::FooPlaylistWidget ()
{
	setSortingEnabled(true);
	setIndentation(0);
	setAlternatingRowColors (true);
	QStringList l;
	l << tr("File") << tr("Track") << tr("Title") << tr("Artist") << tr("Album") << tr("Comment") << tr("Genre") << tr("Year") << tr("Length") << tr("Rating");
	setHeaderLabels(l);
}

void FooPlaylistWidget::addFile (QString path)
{
	QTreeWidgetItem *wid = new QTreeWidgetItem (this);
	QLabel *lab = new QLabel (path, this);
	setItemWidget (wid, 0, lab);
	addTopLevelItem (wid);
}

