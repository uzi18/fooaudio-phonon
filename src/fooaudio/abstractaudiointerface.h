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

#ifndef ABSTRACTAUDIOINTERFACE_H__
#define ABSTRACTAUDIOINTERFACE_H__

#include <QObject>
#include <QUrl>

#include "abstractaudioplugin.h"

namespace FooAudio
{
	class AbstractAudioInterface
	{
	public:
		virtual ~AbstractAudioInterface() {}
		virtual AbstractAudioPlugin* GetAudioPlugin() = 0;
	};
}

Q_DECLARE_INTERFACE(FooAudio::AbstractAudioInterface, "org.fooaudio.AudioInterface/1.0");

#endif // ABSTRACTAUDIOINTERFACE_H__
