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
	char *plistGetFile(int);
};

#endif // _FOOPLAYLIST_HPP_

