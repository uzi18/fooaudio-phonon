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

#include "phononengine.h"

#include <QDebug>

namespace FooAudio
{
    PhononEngine::PhononEngine()
    {
    }

    PhononEngine::~PhononEngine()
    {
    }    

    bool PhononEngine::isPlaying()
    {
        return false;
    }

    bool PhononEngine::isStopped()
    {
        return !isPlaying();
    }

    bool PhononEngine::isPaused()
    {
        return false;
    }

    bool PhononEngine::isMuted()
    {
        return false;
    }

    void PhononEngine::setMuted(bool)
    {
    }

    qint64 PhononEngine::totalTime()
    {
    }

    void PhononEngine::seek(qint64 time)
    {
    }

    void PhononEngine::stop()
    {
    }

    void PhononEngine::play()
    {
    }

    void PhononEngine::pause()
    {
    }

    void PhononEngine::clearQueue()
    {
    }

    void PhononEngine::enqueueNextFile(QUrl file)
    {
    }

    void PhononEngine::playFile(QUrl file)
    {
    }

    void PhononEngine::setVolume(int volume)
    {
    }
};
