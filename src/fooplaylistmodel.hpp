#ifndef _FOOPLAYLISTMODEL_HPP_
#define _FOOPLAYLISTMODEL_HPP_

#include <QtGui>

//#include "fooplaylistfiller.hpp"
//#include "footagger.hpp"

class FooPlaylistModel : public QAbstractListModel
{
	Q_OBJECT
	
public:
	FooPlaylistModel (QObject *parent = 0);
	~FooPlaylistModel ();

	int rowCount (const QModelIndex &parent = QModelIndex ()) const;
	int columnCount (const QModelIndex &parent = QModelIndex ()) const;
	QVariant data (const QModelIndex &, int) const;
	QVariant data (int) const;
	QVariant headerData (int, Qt::Orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags (const QModelIndex &index) const;
	bool setData (const QModelIndex &, const QVariant &, int);
	bool insertRows (int, int, const QModelIndex &index = QModelIndex ());
	bool removeRows (int, int, const QModelIndex &index = QModelIndex ());
//	void appendList (QList<FooTagEntry> list);
	virtual QStringList mimeTypes () const;
	virtual QMimeData *mimeData (const QModelIndexList &) const;
	virtual bool dropMimeData (const QMimeData *, Qt::DropAction, int, int, const QModelIndex &);
	virtual Qt::DropActions supportedDropActions () const;

	enum Fields
	{
		Empty = 0, Stat, File, Track, Title, Artist, Album, Comment, Genre, Year, Length, Rating, CueStart, CueLength, DBIndex, StartTime, ColumnCount
	};

private slots:
	void addItem (QUrl, int, QList<QVariant>, long, long);

private:
	struct sData
	{
		QVariant *values [ColumnCount];
	};

	QList<struct sData> _data;
	int _current;
};

#endif // _FOOPLAYLISTMODEL_HPP_
