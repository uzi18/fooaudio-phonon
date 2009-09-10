#include <QtTest/QtTest>

#include <abstractaudiointerface.h>

class TestPhononEngine : public QObject
{
    Q_OBJECT

    FooAudio::AbstractAudioInterface *plugin;

public:
    TestPhononEngine();

private slots:
    void loadPlugin();
    void isPlaying();
    void isStopped();
    void isPaused();
    void isMuted();
};
