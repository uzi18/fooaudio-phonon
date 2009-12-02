#include "footracklist.hpp"

FooTrackList::FooTrackList(QString name, QUuid uuid) : Uuid(uuid.isNull() ? QUuid() : uuid)
{
	if (name.isEmpty())
		Name = QObject::tr("New Playlist");
	else
		Name = name;
}
