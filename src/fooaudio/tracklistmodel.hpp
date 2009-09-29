#ifndef TRACKLISTMODEL_HPP
#define TRACKLISTMODEL_HPP

#include <QtCore/QAbstractItemModel>

class QModelIndex;

const int FooRoles = 1000;

const int FileRoles = FooRoles + 0;
const int TitleRole = FooRoles + 1;
const int ArtistRole = FooRoles + 2;
const int YearRole = FooRoles + 3;
const int AlbumRole = FooRoles + 4;
const int TrackRole = FooRoles + 5;

class TrackListModel : public QAbstractItemModel
{
    Q_OBJECT


public:
    TrackListModel();
};

#endif // TRACKLISTMODEL_HPP
