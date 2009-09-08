#include "fooplaylistwidget.hpp"

#include <QStringList>
#include <QLabel>
#include <QDebug>
#include <QUrl>
#include <iostream>

using namespace std;

FooPlaylistWidget::FooPlaylistWidget ()
	: QTreeWidget()
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSortingEnabled(false);
	setIndentation(0);
	setAlternatingRowColors (true);

	QStringList l;
	l << tr("File");
	setHeaderLabels(l);
}

void FooPlaylistWidget::addFile (QString path)
{
	cerr << "FooPlaylistWidget::addFile" << endl;
	QTreeWidgetItem *wid = new QTreeWidgetItem (this);
	wid->setText(0, path);
	addTopLevelItem (wid);
}

int FooPlaylistWidget::plistFindFname (const char *fname)
{
	cerr << "FooPlaylistWidget::plistFindFname" << endl;
	for (int i = 0; i < topLevelItemCount(); i++)
	{
		QTreeWidgetItem *wid = topLevelItem(i);

		if (wid->text(0) == fname)
		{
			return i;
			break;
		}
	}
	return -1;
}

QString FooPlaylistWidget::plistGetFile(int i)
{
	return topLevelItem(i)->text(0);
}

int FooPlaylistWidget::plistCount()
{
	return topLevelItemCount ();
}

int FooPlaylistWidget::plistNext(int i)
{
	return (topLevelItemCount() == i) ? 1 : ++i;
}

QUrl FooPlaylistWidget::file(int i)
{
	cerr << "FooPlaylistWidget::nextFile" << endl;
	cerr << this->topLevelItemCount() << endl;
	cerr << i << endl;
	QString text = topLevelItem(i)->text(0);
	QUrl url(text);
	cerr << "plik :" << text.toStdString() << endl;
	return url;
}

QList<QTreeWidgetItem *> FooPlaylistWidget::itemsList()
{

}
