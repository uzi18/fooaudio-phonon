#ifndef FOOCHANGENAME_HPP
#define FOOCHANGENAME_HPP

#include <QWidget>

class QTabBar;
class QLineEdit;
class QPushButton;

class FooChangeName : public QWidget
{
	Q_OBJECT

	int index;

	QTabBar *tabBar;
	QLineEdit *lineEdit;
	QPushButton *saveButton;
	QPushButton *closeButton;

public:
	FooChangeName(int, QWidget *parent = 0);
	~FooChangeName();

public slots:
	void save();
};

#endif // FOOCHANGENAME_HPP
