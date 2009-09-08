#include "fooaudioapp.hpp"
#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"

int main (int argc, char * argv [])
{
	Q_INIT_RESOURCE (resource);

	FooaudioApp fooApp (argc, argv);

	fooApp.setApplicationName("fooaudio");
	fooApp.setQuitOnLastWindowClosed(true);

	FooPhononAudioEngine *fooAudioEngine = new FooPhononAudioEngine(&fooApp);

	FooMainWindow *fooMainWindow = new FooMainWindow(fooAudioEngine);

	fooMainWindow->show();

	return fooApp.exec ();
} // int main (int, char **);
