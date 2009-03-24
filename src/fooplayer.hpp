#ifndef _FOOPLAYER_HPP_
#define _FOOPLAYER_HPP_

#include <QtGui>
#include "foomainwindow.hpp"

class FooPlayer : public QObject
{
	Q_OBJECT

public:
	FooPlayer (FooMainWindow *);

public slots:
	void play ();
	void prev ();
	void stop ();
	void next ();
	void pause ();
private:
	FooMainWindow *mainWindow;
};

#endif // _FOOPLAYER_HPP_
