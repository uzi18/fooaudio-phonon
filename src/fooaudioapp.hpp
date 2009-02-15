#ifndef _FOOAUDIOAPP_HPP_
#define _FOOAUDIOAPP_HPP_

#include <QApplication>
#include <QDir>

#include "fooplugininterfaces.hpp"

class FooaudioApp : public QApplication
{
public:
	FooaudioApp (int, char **);

	void loadPlugins ();

private:
	QDir pluginsDir;
	QStringList pluginFileNames;
};

#endif // _FOOAUDIOAPP_HPP_
