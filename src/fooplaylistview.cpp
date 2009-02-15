#include "fooplaylistview.hpp"

FooPlaylistView::FooPlaylistView (QWidget *parent) : QTreeView (parent), correct (false), dragStarted (false), error_count (0)
{
	pmodel.setDynamicSortFilter (true);
	pmodel.setSourceModel (&model);
	setModel (&pmodel);
	setEditTriggers (QAbstractItemView::NoEditTriggers); 
	setSelectionMode (QAbstractItemView::ExtendedSelection);
	setSortingEnabled(true);
	setAlternatingRowColors (true);

	connect (this, SIGNAL (doubleClicked (const QModelIndex &)), this, SLOT (onDoubleClick (const QModelIndex &)));
	
	hideColumn (FooPlaylistModel::Empty);
	hideColumn (FooPlaylistModel::CueStart);
	hideColumn (FooPlaylistModel::CueLength);
	hideColumn (FooPlaylistModel::DBIndex);
	hideColumn (FooPlaylistModel::StartTime);
}

FooPlaylistView::~FooPlaylistView ()
{
}

void FooPlaylistView::dropEvent (QDropEvent *event)
{
	if (dragStarted)
	{
		QModelIndex index = indexAt (event->pos ());
		if (model.dropMimeData (event->mimeData (), Qt::MoveAction, index.row (), index.column (), index))
		{
			event->setDropAction (Qt::MoveAction);
			event->accept ();
		}
		
		stopAutoScroll ();
		setState (NoState);
	}
	else
	{
		QTreeView::dropEvent (event);
	}
}

void FooPlaylistView::startDrag (Qt::DropActions supportedActions)
{
	dragStarted = true;
	QTreeView::startDrag (supportedActions);
	dragStarted = false;
}

void FooPlaylistView::addItem (QVariant item, int id, QModelIndex* ind)
{
	if (!ind)
	{
		ind = &insindex;
	}

	if (id == FooPlaylistModel::File)
	{
		model.insertRows (ind->row () < 0 ? model.rowCount () : ind->row (), 1);
	}

	model.setData (ind->row () < 0 ? model.index (model.rowCount () - 1, id) : model.index (ind->row (), id), item, Qt::EditRole);
}

void FooPlaylistView::onDoubleClick(const QModelIndex &index)
{
}

void FooPlaylistView::setColVisible(int c, bool v)
{
	if(v) showColumn(c);
	else hideColumn(c);
}

void FooPlaylistView::setColWidth(int c, int w)
{
	setColumnWidth(c, w);
}

