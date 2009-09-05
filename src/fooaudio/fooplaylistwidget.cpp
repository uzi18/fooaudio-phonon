#include "fooplaylistwidget.hpp"

#include <QStringList>
#include <QLabel>
#include <QDebug>
#include <QUrl>
#include <iostream>

using namespace std;

FooPlaylistWidget::FooPlaylistWidget ()
{
	setSortingEnabled(true);
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
	QLabel *lab = new QLabel (path, this);
	setItemWidget (wid, 0, lab);
	addTopLevelItem (wid);
}

int FooPlaylistWidget::plistFindFname (const char *fname)
{
	cerr << "FooPlaylistWidget::plistFindFname" << endl;
	for (int i = 0; i < topLevelItemCount(); i++)
	{
		QTreeWidgetItem *wid = topLevelItem(i);

		QLabel *lab = &((QLabel) itemWidget(wid, 0));

		if (lab->text() == fname)
		{
			return i;
			break;
		}
	}
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
	// QLabel * bar = (QLabel*)foo->itemWidget(item, 0);
	QLabel *lab = (QLabel*) itemWidget(topLevelItem(i), 0);
	cerr << "Dostałem labela" << endl;
	if (!lab)
		return QUrl();

	QUrl url = lab->text();
	cerr << "Label ma text " << lab->text().toStdString() << endl;
	return url;
}
