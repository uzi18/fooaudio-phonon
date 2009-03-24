#include <QTextCodec>
//#include <QPluginLoader>
//#include <QDir>

#include "fooaudioapp.hpp"
#include "fooplayer.hpp"
#include "foomainwindow.hpp"
//#include "fooplugininterfaces.hpp"

FooaudioApp::FooaudioApp (int argc, char *argv[]) : QApplication (argc, argv)
{
	setApplicationName ("fooaudio");
	setQuitOnLastWindowClosed (true);

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
}

//void FooaudioApp::loadPlugins ()
//{
/*	foreach (QObject *plugin, QPluginLoader::staticInstances ())
	{
		;
	}

	pluginsDir = QDir(qApp->applicationDirPath());

	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		if (plugin)
		{
			pluginFileNames += fileName;
		}
	}*/
//}

