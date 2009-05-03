#ifndef _FOOAUDIOAPP_HPP_
#define _FOOAUDIOAPP_HPP_

#include <QApplication>
#include "foomainwindow.hpp"

class FooaudioApp : public QApplication
{
	Q_OBJECT

public:
	FooaudioApp (int, char **);
};

#endif // _FOOAUDIOAPP_HPP_
