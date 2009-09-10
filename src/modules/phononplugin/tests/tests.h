#include <QtTest/QtTest>

#include <abstractaudioplugin.h>

class TestPhononEngine : public QObject
{
    Q_OBJECT

    FooAudio::AbstractAudioPlugin *plugin;

public:
    TestPhononEngine();

private slots:
    void loadPlugin();
    void isPlaying();
    void isStopped();
    void isPaused();
    void isMuted();
    void aboutToFinish();
};
