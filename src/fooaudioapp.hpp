#ifndef _FOOAUDIOAPP_HPP_
#define _FOOAUDIOAPP_HPP_

//#include "fooplugininterfaces.hpp"
#include <QApplication>
//#include "foomainwindow.hpp"

class FooaudioApp : public QApplication
{
	Q_OBJECT

public:
	FooaudioApp (int, char **);

//	void showMainWindow ();

private:
//	FooMainWindow fooWindow;
};

#endif // _FOOAUDIOAPP_HPP_
