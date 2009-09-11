#include <QApplication>
#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

int main (int argc, char * argv [])
{
	Q_INIT_RESOURCE (resource);

	QApplication fooApp (argc, argv);
	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
	fooApp.setApplicationName("fooaudio");
	fooApp.setQuitOnLastWindowClosed(true);

	FooPhononAudioEngine *fooAudioEngine = new FooPhononAudioEngine(&fooApp);

	FooMainWindow *fooMainWindow = new FooMainWindow();
	fooMainWindow->setAudioEngine(fooAudioEngine);

	fooMainWindow->show();

	return fooApp.exec ();
} // int main (int, char **);
