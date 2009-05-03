#ifndef _FOOPLAYLIST_HPP_
#define _FOOPLAYLIST_HPP_

#include <QTreeWidget>

class FooPlaylistWidget : public QTreeWidget
{
public:
	FooPlaylistWidget ();

	void addFile (QString);
};

#endif // _FOOPLAYLIST_HPP_

