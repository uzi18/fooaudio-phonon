#include "foochangename.hpp"
#include "footabwidget.hpp"

#include <QPushButton>
#include <QLineEdit>
#include <QTabBar>

FooChangeName::FooChangeName(int index, QWidget *parent)
{
	tabBar = (QTabBar*) parent;

	Qt::WindowFlags flag = Qt::Dialog;
	setWindowFlags(flag);

	setWindowTitle(tr("Change name"));

	QSizePolicy::Policy fixed = QSizePolicy::Preferred;
	QSizePolicy sizePolicy(fixed, fixed);

	setSizePolicy(sizePolicy);
	setMaximumSize(200, 75);

	lineEdit = new QLineEdit(tabBar->tabText(index), this);
	lineEdit->setGeometry(5, 5, 190, 30);
	saveButton = new QPushButton("Save", this);
	saveButton->setGeometry(95, 40, 50, 30);
	closeButton = new QPushButton("Close", this);
	closeButton->setGeometry(145, 40, 50, 30);
	this->index = index;

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

	show();
}

FooChangeName::~FooChangeName()
{
}

void FooChangeName::save()
{
	tabBar->setTabText(index, lineEdit->text());
	this->close();
}
