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

#ifndef PHONONENGINE_H__
#define PHONONENGINE_H__

#include <abstractaudiointerface.h>
#include <fooaudioexports.h>

#include <QObject>
#include <QUrl>

namespace FooAudio
{
    class FAEXPORTS PhononEngine : public QObject, public AbstractAudioInterface
    {
        Q_OBJECT
    public:
        PhononEngine();
        ~PhononEngine();
        
        void init();

        bool isPlaying();
        bool isStopped();
        bool isPaused();
        bool isMuted();
        void setMuted(bool);

        qint64 totalTime();
        void seek(qint64 time);

    signals:
        void aboutToFinish();
        void progress(qint64 time);
        void willPlayNow(QUrl file);

    public slots:
        void stop();
        void play();
        void pause();
        void clearQueue();
        void enqueueNextFile(QUrl file);
        void playFile(QUrl file);
        void setVolume(int volume);
    };    
}

#endif // PHONONENGINE_H__
