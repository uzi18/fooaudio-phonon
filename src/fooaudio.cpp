#include "fooaudioapp.hpp"
#include "foomainwindow.hpp"

int main (int argc, char * argv [])
{
	Q_INIT_RESOURCE (resource);

	FooaudioApp fooApp (argc, argv);

//	QApplication fooApp (argc, argv);
	FooMainWindow fooWindow;

	fooWindow.show ();
//	fooApp.showMainWindow ();


	return fooApp.exec ();
} // int main (int, char **);

