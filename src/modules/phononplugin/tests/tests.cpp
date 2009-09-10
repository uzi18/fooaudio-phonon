#include "tests.h"

#include <abstractaudiointerface.h>
#include <abstractaudioplugin.h>

#include <QPluginLoader>
#include <QtDebug>
#include <QFileInfo>

TestPhononEngine::TestPhononEngine()
{
    QCoreApplication::setApplicationName("TestPhononEngine");
}

void TestPhononEngine::loadPlugin()
{
    QFileInfo pluginPath("../../../../lib/libphononplugin.so");
    QPluginLoader loader(pluginPath.absoluteFilePath());

    QObject *p = loader.instance();
    if(!p)
        qDebug() << loader.errorString();

    QVERIFY(p != NULL);

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
        QFAIL("Cannot cast to AbstractAudioInterfae!");
    }

    QVERIFY(aai != NULL);

    plugin = aai->GetAudioPlugin();

//        void aboutToFinish();
//        void progress(qint64 time);
//        void willPlayNow(QUrl file);

    QVERIFY(plugin != NULL);

    QObject::connect(plugin, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
//    emit plugin->aboutToFinish();
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

void TestPhononEngine::aboutToFinish()
{
}

QTEST_MAIN(TestPhononEngine)
