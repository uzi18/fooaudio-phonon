#ifndef _FOOPLAYLISTVIEW_HPP_
#define _FOOPLAYLISTVIEW_HPP_

#include <unistd.h>
#include <QtGui>
#include "fooplaylistmodel.hpp"

class FooPlaylistView : public QTreeView
{
	Q_OBJECT

public:
	FooPlaylistView (QWidget *parent = 0);
	~FooPlaylistView ();
	void dropEvent (QDropEvent *);
	void startDrag (Qt::DropActions);

private:
	FooPlaylistModel model;
	QSortFilterProxyModel pmodel;
	QModelIndex insindex;
	QModelIndex curindex;
	QModelIndex plindex;
	QString info;
	bool correct;
	bool dragStarted;
	int error_count;
    
private slots:
	void addItem (QVariant, int, QModelIndex* index = 0);
	void onDoubleClick (const QModelIndex &);
	void setColVisible (int, bool);
	void setColWidth (int, int);
};

#endif // _FOOPLAYLISTVIEW_HPP_
