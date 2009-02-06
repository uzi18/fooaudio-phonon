#ifndef _FOOTABBAR_HPP_
#define _FOOTABBAR_HPP_

#include <QTabBar>
#include <QPoint>

class FooTabBar : public QTabBar
{
	Q_OBJECT

public:
	FooTabBar (QWidget *parent);
	~FooTabBar ();

signals:
	void mouseDoubleClickTab (int tab);
	void contextMenu (QContextMenuEvent *event, int tab);
	void moveTabSignal (int, int);

protected:
	void mouseDoubleClickEvent (QMouseEvent *);
	void mouseMoveEvent (QMouseEvent *);
	void mousePressEvent (QMouseEvent *);
	void contextMenuEvent (QContextMenuEvent *);
	void wheelEvent (QWheelEvent *);

	void dragEnterEvent (QDragEnterEvent *);
	void dragMoveEvent (QDragMoveEvent *);
	void dropEvent (QDropEvent *);

private:
	QPoint dragStartPosition_;
	int dragTab_;

	int findTabUnder (const QPoint &pos);
	void moveTab (int, int);
};

#endif // _FOOTABBAT_HPP_
