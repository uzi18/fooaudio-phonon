#include "footablewidget.hpp"

FooTableWidget::FooTableWidget (QWidget *parent) : QTableWidget (parent)
{
	QStringList tmp;
	tmp << "Playling" << "Track" << "Name" << "Album" << "Author" << "Duration";
	setColumnCount (tmp.count ());
	setHeaders (tmp);

	setEditTriggers (QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
	
	setSelectionBehavior (QAbstractItemView::SelectRows);

//	showGrid (false);
	setCornerButtonEnabled (false);

	insertRow (10);
}

void FooTableWidget::setHeaders (QStringList headers)
{
	headers_ = headers;
	setHorizontalHeaderLabels (headers_);
}

