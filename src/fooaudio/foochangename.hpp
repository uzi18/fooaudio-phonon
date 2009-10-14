#ifndef FOOCHANGENAME_HPP
#define FOOCHANGENAME_HPP

#include <QWidget>

class QTabBar;
class QLineEdit;
class QPushButton;

class FooChangeName : public QWidget
{
	Q_OBJECT

public:
	FooChangeName(int, QWidget *parent = 0);
	~FooChangeName();

private:
	int index;

	QTabBar *tabBar;
	QLineEdit *lineEdit;
	QPushButton *saveButton;
	QPushButton *closeButton;

public slots:
	void save();
};

#endif // FOOCHANGENAME_HPP
