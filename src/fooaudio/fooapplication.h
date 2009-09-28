#ifndef FOOAPPLICATION_H
#define FOOAPPLICATION_H

#include <QApplication>

class FooApplication : public QObject
{
    Q_OBJECT
public:
    FooApplication();
    ~FooApplication();
    int start(int argc, char *argv[]);

private:
    void InitializeLogic();
    QApplication *m_application;

public slots:
    void quitApp();
};

#endif // FOOAPPLICATION_H
