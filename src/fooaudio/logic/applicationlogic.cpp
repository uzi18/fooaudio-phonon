#include "applicationlogic.h"

#include <QDebug>

ApplicationLogic::ApplicationLogic()
{
}

ApplicationLogic::~ApplicationLogic()
{
}

void ApplicationLogic::InitializeControllers()
{
// example:
//    controllersList["Controller"] = QSharedPointer<Controller>(new Controller);
}

QSharedPointer<QObject> ApplicationLogic::getController(QString controllerName)
{
    return controllersList[controllerName];
}

void ApplicationLogic::run()
{
    InitializeControllers();

    exec();
}
