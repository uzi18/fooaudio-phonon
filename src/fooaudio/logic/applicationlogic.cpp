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

#include "applicationlogic.h"

#include <QDebug>

ApplicationLogic* ApplicationLogic::Instance = 0;

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

ApplicationLogic* ApplicationLogic::getInstance()
{
    if(!Instance)
        Instance = new ApplicationLogic;

    return Instance;
}

void ApplicationLogic::Release()
{
    if(Instance)
        return;

    Instance->quit();
    while(Instance->isRunning())
    {
        QApplication::processEvents();
        QApplication::instance()->thread()->msleep(100);
    }

    delete Instance;
    Instance = 0;
}

void ApplicationLogic::run()
{
    InitializeControllers();

    exec();
}
