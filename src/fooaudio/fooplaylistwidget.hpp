#ifndef _FOOPLAYLIST_HPP_
#define _FOOPLAYLIST_HPP_

#include <QTreeWidget>




class FooPlaylistWidget : public QTreeWidget
{
public:
    FooPlaylistWidget ();

    void addFile (QString file, int index = -1);
    int plistFindFname (const char *);
    int plistNext (int);
    int plistCount();
    QString plistGetFile(int);
    QUrl file(int);
    QList<QTreeWidgetItem *> itemsList();
    void addFiles(int index, QList<QUrl> list, bool recursive = true);
    //Qt::DropActions supportedDropActions ( void ) const { return Qt::CopyAction | Qt::MoveAction ; }

protected:
    void dropEvent (QDropEvent * event);
    void dragEnterEvent (QDragEnterEvent *);
    void contextMenuEvent ( QContextMenuEvent * event );

};

#endif // _FOOPLAYLIST_HPP_

