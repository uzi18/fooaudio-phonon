#include "fooapplication.h"
#include "fooaudioengine.hpp"
#include "foomainwindow.hpp"
#include "logic/applicationlogic.h"

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

    FooPhononAudioEngine *fooAudioEngine = new FooPhononAudioEngine(m_application);

    FooMainWindow *fooMainWindow = new FooMainWindow();
    fooMainWindow->setAudioEngine(fooAudioEngine);

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
