#include <QTextCodec>
#include <iostream>

using namespace std;

#include "fooaudioapp.hpp"

FooaudioApp::FooaudioApp (int argc, char *argv[]) : QApplication (argc, argv)
{
	cerr << "FooaudioApp" << endl;

//	setOrganizationName("fooaudio Team");
	setApplicationName ("fooaudio");
	setQuitOnLastWindowClosed (true);

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
}
