#include <QTextCodec>

#include "fooaudioapp.hpp"
#include "foomainwindow.hpp"

#include <iostream>
using namespace std;

FooaudioApp::FooaudioApp (int argc, char *argv[]) : QApplication (argc, argv)
{
	cout << "FooaudioApp" << endl << flush;

	setApplicationName ("fooaudio");
	setQuitOnLastWindowClosed (true);

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
}
