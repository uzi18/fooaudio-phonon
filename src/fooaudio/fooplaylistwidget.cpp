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
	l << tr("File") << tr("Playback") << tr("Track") << tr("Title") << tr("Artist") << tr("Album") << tr("Comment") << tr("Genre") << tr("Year") << tr("Length") << tr("Rating");
	setHeaderLabels(l);
}

void FooPlaylistWidget::addFile (QString path)
{
	cout << "FooPlaylistWidget::addFile" << endl << flush;
	QTreeWidgetItem *wid = new QTreeWidgetItem (this);
	QLabel *lab = new QLabel (path, this);
	setItemWidget (wid, 0, lab);
	addTopLevelItem (wid);
}

int FooPlaylistWidget::plistFindFname (const char *fname)
{
	cout << "FooPlaylistWidget::plistFindFname" << endl << flush;
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
	cout << "FooPlaylistWidget::nextFile" << endl << flush;
	cout << this->topLevelItemCount() << endl << flush;
	cout << i << endl << flush;
	// QLabel * bar = (QLabel*)foo->itemWidget(item, 0);
	QLabel *lab = (QLabel*) itemWidget(topLevelItem(i), 0);
	cout << "Dostałem labela" << endl << flush;
	if (!lab)
		return QUrl();

	QUrl url = lab->text();
	cout << "Label ma text " << lab->text().toStdString() << endl << flush;
	return url;
}
