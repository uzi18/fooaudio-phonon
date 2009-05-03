#include <QApplication>
#include <QPluginLoader>
#include <iostream>

#include "fooaudioengine.hpp"

FooAudioEngine::FooAudioEngine ()
{
	loadPlugins ();
}

void FooAudioEngine::loadPlugins ()
{
	pluginsDir = QDir(qApp->applicationDirPath());
	pluginsDir.cd("plugins");

	QFileInfoList list = pluginsDir.entryInfoList();
	std::cout << "     Bytes Filename" << std::endl;
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
		std::cout << std::endl;
	}

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();

		std::cout << "dupa " << qPrintable(QString(fileName)) << std::endl;

		if (plugin)
		{
			pluginFileNames += fileName;
			std::cout << qPrintable(QString(fileName)) << std::endl;
		}
	}
}

QDir FooAudioEngine::getPluginsDir ()
{
	return pluginsDir;
}

QStringList FooAudioEngine::getPluginFileNames ()
{
	return pluginFileNames;
}

