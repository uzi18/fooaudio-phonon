/*********************************************************************************
 * FooAudio
 * Copyright (C) 2009  Dariusz Mikulski <dariusz.mikulski@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************************/

#include "fooapplication.h"
#include "foomainwindow.hpp"
#include "logic/applicationlogic.h"
#include "abstractaudioplugin.h"
#include "abstractaudiointerface.h"

#include <QPluginLoader>
#include <QtDebug>
#include <QFileInfo>
//#include <QtTest/QtTest>

FooApplication::FooApplication()
{
	InitializeLogic();
}

FooApplication::~FooApplication()
{
	delete m_application;
}

int FooApplication::start(int argc, char *argv[])
{
	m_application = new QApplication(argc, argv);

	QObject::connect(m_application, SIGNAL(aboutToQuit()), this, SLOT(quitApp()));

	QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
	m_application->setApplicationName("fooaudio");
	m_application->setQuitOnLastWindowClosed(true);

//	FooPhononAudioEngine *fooAudioEngine = new FooPhononAudioEngine(m_application);

	FooAudio::AbstractAudioPlugin *plugin;
	QFileInfo pluginPath("../lib/libphononplugin.so");
	QPluginLoader loader(pluginPath.absoluteFilePath());

	QObject *p = loader.instance();
	if(!p)
		 qDebug() << loader.errorString();

//	QVERIFY(p != NULL);

	FooAudio::AbstractAudioInterface *aai = 0;

	if(p)
	{
		 aai = qobject_cast<FooAudio::AbstractAudioInterface*>(p);
	}
	else
	{
		 qDebug() << loader.errorString();
	}

	if(!aai)
	{
//		 QFAIL("Cannot cast to AbstractAudioInterfae!");
	}

//	QVERIFY(aai != NULL);

	plugin = aai->GetAudioPlugin();

//        void aboutToFinish();
//        void progress(qint64 time);
//        void willPlayNow(QUrl file);

//	QVERIFY(plugin != NULL);

	FooMainWindow *fooMainWindow = new FooMainWindow();
	//fooMainWindow->setAudioEngine(fooAudioEngine);
	fooMainWindow->setAudioEngine(plugin);

	fooMainWindow->show();

	return m_application->exec();
}

void FooApplication::quitApp()
{
	ApplicationLogic::Release();
}

void FooApplication::InitializeLogic()
{
	ApplicationLogic::getInstance()->start();
}

