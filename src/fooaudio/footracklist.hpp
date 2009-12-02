/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FOOTRACKLIST_HPP
#define FOOTRACKLIST_HPP

#include <QList>
#include <QUuid>
#include <QString>

#include "footrack.hpp"

class FooTrackList : public QList<FooTrack>
{
	QString Name;
	QUuid Uuid;

public:
	FooTrackList(QString name = "", QUuid uuid = 0);

	const QUuid & uuid() { return Uuid; }
	const QString & name() { return Name; }
	void setName (const QString &name) { Name = name; }
};

#endif
