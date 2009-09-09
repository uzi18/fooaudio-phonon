#include "tests.h"

#include <QPluginLoader>
#include <QtDebug>
#include <QFileInfo>

TestPhononEngine::TestPhononEngine()
{
    QCoreApplication::setApplicationName("TestPhononEngine");
}

void TestPhononEngine::loadPlugin()
{
    QFileInfo pluginPath("../../../../lib/libphononengine.so");
    QPluginLoader loader(pluginPath.absoluteFilePath());

    QObject *p = loader.instance();
    QVERIFY(p != NULL);

    if(p)
    {
        plugin = qobject_cast<FooAudio::AbstractAudioInterface*>(p);
    }
    else
    {
        qDebug() << loader.errorString();
    }

    QVERIFY(plugin != NULL);
}

void TestPhononEngine::isPlaying()
{
    QVERIFY(plugin->isPlaying() == false);
}

void TestPhononEngine::isStopped()
{
    QVERIFY(plugin->isStopped() == true);
}

void TestPhononEngine::isPaused()
{
    QVERIFY(plugin->isPaused() == false);
}

void TestPhononEngine::isMuted()
{
    plugin->setMuted(true);
    QVERIFY(plugin->isMuted() == true);
}

QTEST_MAIN(TestPhononEngine)
