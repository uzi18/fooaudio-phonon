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
