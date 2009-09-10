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

#ifndef ABSTRACTAUDIOPLUGIN_H__
#define ABSTRACTAUDIOPLUGIN_H__

#include <QObject>
#include <QUrl>

namespace FooAudio
{
    class AbstractAudioPlugin : public QObject
    {
        Q_OBJECT

    public:
        AbstractAudioPlugin(QObject *parent = 0);
        virtual ~AbstractAudioPlugin() {}

        virtual bool isPlaying() = 0;
        virtual bool isStopped() = 0;
        virtual bool isPaused() = 0;
        virtual bool isMuted() = 0;
        virtual void setMuted(bool) = 0;

        virtual qint64 totalTime() = 0;
        virtual void seek(qint64 time) = 0;

    signals:
        void aboutToFinish();
        void progress(qint64 time);
        void willPlayNow(QUrl file);

    public slots:
        virtual void stop() = 0;
        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void clearQueue() = 0;
        virtual void enqueueNextFile(QUrl file) = 0;
        virtual void playFile(QUrl file) = 0;
        virtual void setVolume(int volume) = 0;
    };
}

#endif // ABSTRACTAUDIOPLUGIN_H__
