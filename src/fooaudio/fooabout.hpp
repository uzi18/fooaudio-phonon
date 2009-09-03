/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FOOABOUT_H
#define FOOABOUT_H

#include <QtGui/QWidget>

class FooAbout : public QWidget
{
	Q_OBJECT

	void keyPressEvent(QKeyEvent *);

public:
	FooAbout(QWidget *parent = 0);
	~FooAbout();

};

#endif
