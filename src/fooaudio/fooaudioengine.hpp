#ifndef _FOOAUDIOENGINE_HPP_
#define _FOOAUDIOENGINE_HPP_

#include <QDir>

class FooAudioEngine
{
//	Q_OBJECT

public:
	FooAudioEngine ();

	QDir getPluginsDir ();
	QStringList getPluginFileNames ();

private:
	QDir pluginsDir;
	QStringList pluginFileNames;

	void loadPlugins ();
};

#endif // _FOOAUDIOENGINE_HPP_

