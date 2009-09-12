#include <QDebug>
#include <QLabel>
#include <QStringList>
#include <QtGui>
#include <QUrl>

#include <iostream>

using namespace std;

#include "foomainwindow.hpp"

#include "fooplaylistwidget.hpp"

FooPlaylistWidget::FooPlaylistWidget ()
        : QTreeWidget()
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

    QStringList l;
    l << tr("File");
    setHeaderLabels(l);

}

void FooPlaylistWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    FooMainWindow *mainWindow = FooMainWindow::instance();
    if (!mainWindow)
        return;

    QMenu menu;
    QAction *action;

    QTreeWidgetItem * item = itemAt (event->pos());
    int index = indexOfTopLevelItem(item);
    int countSelectedItems = selectedItems().count();

    // TODO implement actions

    if (-1 != index)
    {
        action = menu.addAction (tr ("Play w/o stop after"), mainWindow, SLOT (play ()));
        action->setData (index);

        menu.addSeparator ();

        action = menu.addAction (tr ("Play"), mainWindow, SLOT (play ()));
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

void FooPlaylistWidget::addFile (QString file, int index)
{
    QTreeWidgetItem *wid = new QTreeWidgetItem (this);

    wid->setText(0, file);
    insertTopLevelItem (index == -1 ? topLevelItemCount() : index, wid);
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
    cerr << "FooPlaylistWidget::file" << endl;
    cerr << this->topLevelItemCount() << endl;
    cerr << i << endl;
    QString text = topLevelItem(i)->text(0);
    QUrl url(text);
    return url;
}

QList<QTreeWidgetItem *> FooPlaylistWidget::itemsList()
{
    return QList<QTreeWidgetItem *> ();
}

void FooPlaylistWidget::dragEnterEvent(QDragEnterEvent * event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
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
            int index = indexOfTopLevelItem(itemAt (event->pos()));
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
        path = file.toLocalFile();
        info.setFile( path );
        if ( info.isFile() )
        {
            cerr << "FooPlaylistWidget:: addFiles at " << index << endl;
            addFile(path, index);
        }

        else if ( info.isDir() && recursive)
        {
            cerr << "FooPlaylistWidget:: addDir at " << index << endl;
            QDir directory(path);

            QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Readable);

            QList<QUrl> urls;
            foreach( QString string, files){
                urls.append(QUrl(path+"/"+string));
                cerr << "FooPlaylistWidget:: inDir " << string.toStdString() << endl;
            }
            addFiles(index, urls, recursive);
        }
    }
}
