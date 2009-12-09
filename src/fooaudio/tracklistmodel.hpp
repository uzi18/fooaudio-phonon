#ifndef TRACKLISTMODEL_HPP
#define TRACKLISTMODEL_HPP

#include <QtCore/QAbstractItemModel>

const int FooRoles = 1000;

const int FooTrackRole = FooRoles + 1;
const int FileRole = FooRoles + 2;
const int TitleRole = FooRoles + 3;
const int ArtistRole = FooRoles + 4;
const int YearRole = FooRoles + 5;
const int AlbumRole = FooRoles + 6;
const int TrackRole = FooRoles + 7;

class QModelIndex;

class FooTrack;
class FooTrackList;

class TrackListModel : public QAbstractItemModel
{
	Q_OBJECT
	
	FooTrackList * TrackList;

public:
	TrackListModel(FooTrackList *tracklist = 0, QObject *parent = 0);
	virtual ~TrackListModel();

	virtual int columnCount(const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	FooTrack track(const QModelIndex &index) const;
	int trackIndex(FooTrack track);
	QModelIndex trackModelIndex(FooTrack track);
};

#endif // TRACKLISTMODEL_HPP
