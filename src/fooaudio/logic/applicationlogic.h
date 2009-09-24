#ifndef LOGICTHREAD_H
#define LOGICTHREAD_H

#include <QThread>
#include <QSharedPointer>
#include <QMap>
#include <QApplication>

#include "applicationlogicptr.h"

class ApplicationLogic : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationLogic)

    QMap<QString, QSharedPointer<QObject> > controllersList;

    template <typename T>
    class deleter
    {
        public:
            inline void operator() (T *p)
            {
                p->quit();
                while(p->isRunning())
                {
                    QApplication::processEvents();
                    QApplication::instance()->thread()->msleep(100);
                }

                delete p;
            }
    };

public:
    ApplicationLogic();
    ~ApplicationLogic();

    QSharedPointer<QObject> getController(QString controllerName);
    void run();
    void InitializeControllers();

    static ApplicationLogicPtr Create()
    {
        return ApplicationLogicPtr(new ApplicationLogic, ApplicationLogic::deleter<ApplicationLogic>());
    }
};

#endif // LOGICTHREAD_H
