#include "footracklist.hpp"

FooTrackList::FooTrackList(QString name)
{
	if (name.isEmpty())
		Name = QObject::tr("New Playlist");
	else
		Name = name;
}
