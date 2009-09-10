#ifndef _FOOPLAYLIST_HPP_
#define _FOOPLAYLIST_HPP_

#include <QTreeWidget>

class FooPlaylistWidget : public QTreeWidget
{
public:
    FooPlaylistWidget ();

    void addFile (QString);
    int plistFindFname (const char *);
    int plistNext (int);
    int plistCount();
    QString plistGetFile(int);
    QUrl file(int);
    QList<QTreeWidgetItem *> itemsList();

protected:
    void dropEvent (QDropEvent * event);
    void dragEnterEvent (QDragEnterEvent *);
    void contextMenuEvent ( QContextMenuEvent * event );

};

#endif // _FOOPLAYLIST_HPP_

