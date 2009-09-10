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
    // For drag and drop files
    setAcceptDrops(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    QStringList l;
    l << tr("File");
    setHeaderLabels(l);

}

void FooPlaylistWidget::contextMenuEvent ( QContextMenuEvent * event )
{
    FooMainWindow *mainWindow = qobject_cast<FooMainWindow *> (parent()->parent()->parent());
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

void FooPlaylistWidget::addFile (QString path)
{
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
        urlList = event->mimeData()->urls(); // returns list of QUrls

        // if just text was dropped, urlList is empty (size == 0)
        if ( urlList.size() > 0) // if at least one QUrl is present in list
        {
            // here append files/dirs
        }
    }

    event->acceptProposedAction();
}
