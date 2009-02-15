#ifndef _FOOTABLEWIDGET_HPP_
#define _FOOTABLEWIDGET_HPP_

#include <QTableWidget>
#include <QStringList>

class FooTableWidget : public QTableWidget
{
	Q_OBJECT

public:
	FooTableWidget (QWidget *parent = 0);

	void setHeaders (QStringList);

private:
	QStringList headers_;
};

#endif // _FOOTABLEWIDGET_HPP_
