#include <QApplication>
#include <QTextCodec>
#include "foomainwindow.hpp"

int main (int argc, char * argv [])
{
	Q_INIT_RESOURCE (resource);

	QApplication fooApp (argc, argv);

	fooApp.setApplicationName ("fooaudio");
	fooApp.setQuitOnLastWindowClosed (true);

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));

	FooMainWindow fooWindow;

	fooWindow.show ();

	return fooApp.exec ();
} // int main (int, char **);

