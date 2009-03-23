#ifndef _FOOPLAYER_HPP_
#define _FOOPLAYER_HPP_

#include <QtGui>

class FooPlayer : public QObject
{
	Q_OBJECT

public:
	FooPlayer ();

public slots:
	void play ();
};

#endif // _FOOPLAYER_HPP_
