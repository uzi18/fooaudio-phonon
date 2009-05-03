#include <QTextCodec>

#include "fooaudioapp.hpp"
#include "foomainwindow.hpp"

FooaudioApp::FooaudioApp (int argc, char *argv[]) : QApplication (argc, argv)
{
	setApplicationName ("fooaudio");
	setQuitOnLastWindowClosed (true);

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
}

