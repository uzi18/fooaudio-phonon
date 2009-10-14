#include <QObject>

#include "footracklist.hpp"

FooTrackList::FooTrackList(QString name)
{
	if (name.isNull())
	{
		Name = QObject::tr("New Playlist");
	}
	else
	{
		Name = name;
	}
}
