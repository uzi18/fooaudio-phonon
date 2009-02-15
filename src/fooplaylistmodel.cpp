#include "fooplaylistmodel.hpp"

FooPlaylistModel::FooPlaylistModel (QObject *parent) : QAbstractListModel (parent), _current (1)
{
	setSupportedDragActions (Qt::CopyAction | Qt::MoveAction);
}

FooPlaylistModel::~FooPlaylistModel ()
{
	removeRows (0, _data.count ());
}

QStringList FooPlaylistModel::mimeTypes () const
{
	QStringList list;
	list.append ("text/uri-list");
	list.append ("text/plain");
	return list;
}

QMimeData *FooPlaylistModel::mimeData (const QModelIndexList &indexes) const
{
	QMimeData *mime = QAbstractListModel::mimeData (indexes);
	QList<QUrl> list;
	QList<int> rows;

	foreach (QModelIndex index, indexes)
	{
		bool none = true;
		foreach (int r, rows)
		{
			if (index.row () == r)
			{
				none = false;
				break;
			}
		}

		if (none)
		{
			rows << index.row ();
		}
	}

	foreach (int r, rows)
	{
		list.append (data (index (r, File), Qt::UserRole).toUrl ());
	}

	mime->setUrls (list);
	return mime;
}

bool FooPlaylistModel::dropMimeData (const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (action == Qt::IgnoreAction)
	{
		return true;
	}

	int beginRow;
	QList<QUrl> list;

	if (data->hasUrls ())
	{
		list = data->urls ();
	}
	else if (data->hasText ())
	{
		list << QUrl (data->text ());
	}
	else
	{
		return true;
	}

	if (row != -1)
	{
		beginRow = row;
	}
	else
	{
		beginRow = rowCount ();
	}

//	FooPlaylistFiller *fooPlaylistFiller = new FooPlaylistFiller (list, beginRow);

//	if (!connect (fooPlaylistFiller, SIGNAL (sendFile (QUrl, int, QList<QVariant>, long, long)), this, SLOT (addItem (QUrl, int, QList<QVariant, long, long)), Qt::QueuedConnection))
//	{
		// cerr << "Error!";
//	}

//	fooPlaylistFiller->start (QThread::IdlePriority);

	return true;
}

void FooPlaylistModel::addItem (QUrl path, int row, QList<QVariant> l, long start, long length)
{
	try
	{
		if (l.count () >= 9)
		{
			insertRows (row, 1);
			
			if (row >= rowCount ())
			{
				row = rowCount () - 1;
			}
			
			*_data.at (row).values[File] = path;
			*_data.at (row).values[Title] = l[0];
			*_data.at (row).values[Artist] = l[1];
			*_data.at (row).values[Album] = l[2];
			*_data.at (row).values[Comment] = l[3];
			*_data.at (row).values[Genre] = l[4];
			*_data.at (row).values[Length] = l[5];
			*_data.at (row).values[Track] = l[6];
			*_data.at (row).values[Year] = l[7];
			*_data.at (row).values[Rating] = l[8];
			*_data.at (row).values[CueStart] = QVariant ((qlonglong) start);
			*_data.at (row).values[CueLength] = QVariant ((qlonglong) length);
			*_data.at (row).values[DBIndex] = QVariant ((qlonglong) 0);
			
			emit dataChanged (index (row, 0), index (row, ColumnCount - 1));
		}
	}
	catch (char *mes)
	{
		//Console::Self().error(QString(mes));
		// cerr << "error!";
	}
}

Qt::DropActions FooPlaylistModel::supportedDropActions () const 
{
	return Qt::CopyAction | Qt::MoveAction;
}

int FooPlaylistModel::rowCount (const QModelIndex &parent) const
{
    return _data.count ();
}

int FooPlaylistModel::columnCount (const QModelIndex & parent) const
{
	return ColumnCount;
}

QVariant FooPlaylistModel::data (int role) const
{
	if (role == Qt::BackgroundRole)
	{
		return new QBrush (QColor::fromRgb (128, 200, 200));
	}

	return QVariant ();
}

QVariant FooPlaylistModel::data (const QModelIndex &index, int role) const
{
	if (!index.isValid ())
	{
		 return QVariant ();
	}

	if (index.row () >= _data.size ())
	{
		return QVariant ();
	}

	if (index.column () == File)
	{
		if (role == Qt::UserRole)
		{
			return *_data.at (index.row ()).values[index.column ()];
		}
		else if (role == Qt::DisplayRole)
		{
			//return QFileInfo(_data.at(index.row()).values[index.column()]->toUrl().toLocalFile()).fileName(); 
			return _data.at (index.row ()).values[File]->toUrl ().toString ();
		}
	}
	
	if (index.row () == _current)
	{
		if (role == Qt::FontRole)
		{
			return new QFont ("arial", -1, -1, true);
		}
		else if (role == Qt::BackgroundRole) return new QBrush (QColor::fromRgb (128, 200, 200));
	}
    
	switch (role)
	{
		case Qt::DisplayRole:
			return *_data.at (index.row ()).values[index.column ()];

			//case Qt::DecorationRole:

			//case Qt::StatusTipRole: return stringList.at(index.row());

		case Qt::ToolTipRole:
			return *_data.at(index.row ()).values[index.column ()];

		default:
			return QVariant ();
	}
} 

QVariant FooPlaylistModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::BackgroundRole)
	{
		return new QBrush (QColor::fromRgb (128, 200, 200));
	}
	
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}
	
	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
			case 2:
				return tr ("File");

			case 3:
				return tr ("Track");

			case 4:
				return tr ("Title");

			case 5:
				return tr ("Artist");

			case 6:
				return tr ("Album");

			case 7:
				return tr ("Comment");

			case 8:
				return tr ("Genre");

			case 9:
				return tr ("Year");

			case 10:
				return tr ("Length");

			case 11:
				return tr ("Rating");

			default:
				return "";
		}
		//return tr(colnames[section]);
	}
	else
	{
		return QString ("%1").arg (section);
	}
}

Qt::ItemFlags FooPlaylistModel::flags (const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags (index);

	if (index.isValid ())
	{
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	}
	else
	{
		return Qt::ItemIsDropEnabled | defaultFlags;
	}
}

bool FooPlaylistModel::setData (const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid () && role == Qt::EditRole)
	{
		*_data.at (index.row ()).values[index.column ()] = value;
		
		emit dataChanged (index, index);
		
		return true;
	}

	return false;
}

bool FooPlaylistModel::insertRows (int position, int rows, const QModelIndex &parent)
{
	if (position > _data.count ())
	{
		position = _data.count ();
	}

	if (position < 0)
	{
		position = 0;
	}
	
	beginInsertRows (QModelIndex (), position, position + rows - 1);

	for (int row = 0; row < rows; row++)
	{
		struct sData d;

		for (int i=0; i < ColumnCount; i++)
		{
			d.values[i] = new QVariant ();
		}

		if (position >= _data.count ())
		{
			_data.append (d);
		}
    	else
		{
			_data.insert (position, d);
		}
	}

	endInsertRows ();
	return true;
}

bool FooPlaylistModel::removeRows (int position, int rows, const QModelIndex &parent)
{
	if (position > _data.count ())
	{
		position = _data.count ();
	}

	if (position < 0)
	{
		position = 0;
	}

	beginRemoveRows (QModelIndex (), position, position + rows - 1);

	for (int row = 0; row < rows; row++)
	{
		for (int i = 0; i < ColumnCount; i++)
		{
			delete _data.at(position).values[i];
		}
		
		_data.removeAt(position);
	}

	endRemoveRows ();

	return true;
}

/*void FooPlaylistModel::appendList (QList<FooTagEntry> list)
{
	beginInsertRows (QModelIndex (), rowCount (), rowCount () + list.size ());
	
	foreach (FooTagEntry tag, list)
	{
		struct sData d;
    	
		d.values[FooPlaylistModel::Empty] = new QVariant ();
		d.values[FooPlaylistModel::Stat] = new QVariant ();
		d.values[FooPlaylistModel::File] = new QVariant (tag.url);
		d.values[FooPlaylistModel::Title] = new QVariant (tag.title);
		d.values[FooPlaylistModel::Artist] = new QVariant (tag.artist);
		d.values[FooPlaylistModel::Comment] = new QVariant (tag.comment);
		d.values[FooPlaylistModel::Album] = new QVariant (tag.album);
		d.values[FooPlaylistModel::Track] = new QVariant (tag.track);
		d.values[FooPlaylistModel::CueStart] = new QVariant ((qlonglong) tag.start);
		d.values[FooPlaylistModel::CueLength] = new QVariant ((qlonglong) tag.length);
		d.values[FooPlaylistModel::DBIndex] = new QVariant ((qlonglong) tag.dbindex);
		d.values[FooPlaylistModel::Genre] = new QVariant (tag.genre);
		d.values[FooPlaylistModel::Length] = new QVariant (tag.slength);
		d.values[FooPlaylistModel::Year] = new QVariant (tag.year);
		d.values[FooPlaylistModel::Rating] = new QVariant (tag.rating);
		d.values[FooPlaylistModel::StartTime] = new QVariant();
		
		_data.append (d); 
	}
	
	endInsertRows ();
}*/

