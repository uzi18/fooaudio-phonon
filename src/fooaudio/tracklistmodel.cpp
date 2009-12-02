#include "footrack.hpp"
#include "footracklist.hpp"

#include "tracklistmodel.hpp"

TrackListModel::TrackListModel(FooTrackList *tracklist, QObject *parent)
	: QAbstractItemModel(parent), TrackList(tracklist)
{
}

TrackListModel::~TrackListModel()
{
}

int TrackListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

int TrackListModel::rowCount(const QModelIndex &parent) const
{
	return TrackList->count();
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
	FooTrack trr = track(index);
	if (trr.file().isEmpty())
		return QVariant();

	switch (role)
	{
		case Qt::DisplayRole:
			if (index.column() == 0) // long or shor name?
				return trr.file();
			else
				return QString("%1 (%2)").arg(trr.title(), trr.artist());

// 		case Qt::DecorationRole:
// 			return trr.icon().isNull()
// 				? trr.icon()
// 				: QVariant();
// 
// 		case TrackRole:
// 			return QVariant::fromValue<FooTrack>(trr);

		default:
			return QVariant();
	}
}

QVariant TrackListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
		return QString("Column %1").arg(section);
	else
		return QString("Row %1").arg(section);
}

FooTrack TrackListModel::track(const QModelIndex &index) const
{
	if (!index.isValid())
		return FooTrack();

	if (index.row() < 0 || index.row() >= rowCount())
		return FooTrack();

	return TrackList->at(index.row());
}

int TrackListModel::trackIndex(FooTrack track)
{
	return TrackList->indexOf(track);
}

QModelIndex TrackListModel::trackModelIndex(FooTrack track)
{
	return createIndex(trackIndex(track), 0, 0);
}
