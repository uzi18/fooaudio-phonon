#include <QLabel>
#include <QStringList>
#include <QtGui>
#include <QUrl>
#include <QtDebug>

#include "foomainwindow.hpp"
#include "fooplaylistwidget.hpp"

FooPlaylistWidget::FooPlaylistWidget (QWidget *parent) : QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSortingEnabled(false);
	setIndentation(0);
	setAlternatingRowColors (true);
	// For drag and drop files, QAbstractItemView::DragDrop doesn't work (why?)
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
	setDropIndicatorShown(true);
	// Context Menu
	setContextMenuPolicy(Qt::DefaultContextMenu);
	setItemsExpandable(false);
	setRootIsDecorated(false);

// 	QStringList l;
// 	l << tr("File");
// 	setHeaderLabels(l);
// 
// 	// TODO Remove and add something normal
// 	Filters << ".mp3"  << ".wma" << ".mp4" << ".mpg" << ".mpeg" << ".m4a";
// 	Filters << ".flac" << ".ogg" << ".wav" << ".3gp" << ".ac3" << ".aac";

	// TODO .m3u .m4u
}

void FooPlaylistWidget::contextMenuEvent ( QContextMenuEvent * event )
{
	FooMainWindow *mainWindow = FooMainWindow::instance();

	if (!mainWindow)
	{
		return;
	}

	QMenu menu;
	QAction *action;

/*	QTreeWidgetItem * item = itemAt (event->pos());*/
	int index/* = indexOfTopLevelItem(item)*/;
	int countSelectedItems/* = selectedItems().count()*/;

	// TODO implement actions

	if (-1 != index)
	{
		action = menu.addAction (tr ("Play"), mainWindow, SLOT (play ()));
		action->setData (index);

		menu.addSeparator ();

		action = menu.addAction (tr ("Stop after this file"), mainWindow, SLOT (play ()));
		action->setData (index);

		menu.addSeparator ();

		action = menu.addAction (tr ("&Add To Queue"), mainWindow, SLOT (addToQueue())/*, QKeySequence::Close*/);
		action->setData (index);

		// condition
		action = menu.addAction (tr ("&Remove from Queue"), mainWindow, SLOT (removeFromQueue ()));
		action->setData (index);

		menu.addSeparator ();

		action = menu.addAction (tr ("&Remove"), mainWindow, SLOT (removeFromQueue ()));
		action->setData (index);

		menu.addSeparator ();

		action = menu.addAction (tr ("&Copy to Playlist"), mainWindow, SLOT (removeFromQueue ()));
		action->setData (index);

		action = menu.addAction (tr ("&Move to Playlist"), mainWindow, SLOT (removeFromQueue ()));
		action->setData (index);

		menu.addSeparator ();
	}
	else
	{
		menu.addSeparator ();
	}

	menu.exec (QCursor::pos ());
}

void FooPlaylistWidget::addFile (const QString &file, int index)
{
	// if it's network uri
	QUrl adress(file);

	foreach (const QString &filter, Filters)
	{
		if (file.endsWith (filter, Qt::CaseInsensitive) || (!adress.scheme().isEmpty() && !adress.host().isEmpty()))
		{
/*			QTreeWidgetItem *wid = new QTreeWidgetItem (this);

			wid->setText(0, file);
			insertTopLevelItem (index == -1 ? topLevelItemCount() : index, wid);*/
			return;
		}
	}
}

int FooPlaylistWidget::plistFindFname (const char *fname)
{
	qDebug() << "FooPlaylistWidget::plistFindFname";
// 	for (int i = 0; i < topLevelItemCount(); i++)
// 	{
// 		QTreeWidgetItem *wid = topLevelItem(i);
// 
// 		if (wid->text(0) == fname)
// 		{
// 			return i;
// 			break;
// 		}
// 	}
	return -1;
}

QString FooPlaylistWidget::plistGetFile(int i)
{
// 	QTreeWidgetItem *item = this->topLevelItem(i);
// 	if (!item)
// 		return QString();
// 
// 	return item->text(0);
return QString();
}

int FooPlaylistWidget::plistCount()
{
	return 0 /*topLevelItemCount ()*/;
}

int FooPlaylistWidget::plistNext(int i)
{
	// TODO this not support repeat etc. and it is not used?
	return (/*topLevelItemCount()*/0  == i) ? 0 : ++i;
}

QUrl FooPlaylistWidget::file(int i)
{
	qDebug() << "FooPlaylistWidget::file";
// 	qDebug() << this->topLevelItemCount();
	qDebug() << i;
// 	QTreeWidgetItem *item = this->topLevelItem(i);
// 
// 	if (!item)
// 	{
// 		return QUrl();
// 	}

	return QUrl(/*item->text(0)*/);
}

//QList<QTreeWidgetItem *> FooPlaylistWidget::itemsList()
//{
//	QList<QTreeWidgetItem *> list;
// 	for (quint64 i=0, size=topLevelItemCount();i<size;i++)
// 	{
// 		list.append(topLevelItem(i));
// 	}

//	return list;
//}

void FooPlaylistWidget::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void FooPlaylistWidget::dropEvent(QDropEvent * event)
{
	QList<QUrl> urlList;
	if (event->mimeData()->hasUrls())
	{
		urlList = event->mimeData()->urls();

		// if just text was dropped, urlList is empty (size == 0)
		if ( urlList.size() > 0)
		{
			int index /*= indexOfTopLevelItem(itemAt (event->pos()))*/;
			addFiles(index, urlList, true);
		}
	}

	event->acceptProposedAction();
}

void FooPlaylistWidget::addFiles(int index, QList<QUrl> list, bool recursive)
{
	// TODO: check if files are supported by engine
	QFileInfo info;
	QString path;

	foreach (QUrl file, list)
	{
		// toLocalPath() have problems with spaces in file/dir names - it just cut rest
		path = file.toString();
		info.setFile(path);

		if (info.isFile())
		{
			qDebug() << "FooPlaylistWidget:: addFiles at " << index;
			addFile(path, index);
		}
		else if (info.isDir() && recursive)
		{
			qDebug() << "FooPlaylistWidget:: addDir at " << index;
			QDir directory(path);
			QList<QUrl> urls;

			// add all files and dirs to list of urls
			foreach( QString fileInDirectory, directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Readable))
			{
				urls.append(QUrl(directory.filePath(fileInDirectory)));
			}
			// launch this metod one more time to add files and scan subdirs if recursive
			addFiles(index, urls, recursive);
		}
	}
}
