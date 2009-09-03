/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtGui/QBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>

#include "fooabout.h"

FooAbout::FooAbout(QWidget *parent)
	: QWidget(parent, Qt::Window)
{
	setWindowTitle(tr("About"));
	setAttribute(Qt::WA_DeleteOnClose);

	QWidget *top = new QWidget;

	QLabel *l_icon = new QLabel;
	l_icon->setPixmap(QPixmap(":images/icon64.png"));

	QWidget *blank = new QWidget;
	blank->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding));

	QLabel *l_info = new QLabel;
	l_info->setText(QString("<span style=\"font-size: 16pt\"><center>fooaudio</span>"));
	l_info->setWordWrap(true);
	l_info->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

	QHBoxLayout *left_layout = new QHBoxLayout(top);
	left_layout->addWidget(l_icon);
	left_layout->addWidget(l_info);
	left_layout->addWidget(blank);

	// our TabWidget
	QTabWidget *tw_about = new QTabWidget(this);
	tw_about->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
	// end our TabWidget

	// create our info widgets
	// info
	QTextEdit *tb_info = new QTextEdit(tw_about);
	tb_info->setReadOnly(true);
	tb_info->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	tb_info->setWordWrapMode(QTextOption::NoWrap);
	QString info_text;
	info_text += "This is foobar-like audio player";
	tb_info->setText(info_text);

	// authors
	QTextEdit *tb_authors = new QTextEdit(tw_about);
	tb_authors->setReadOnly(true);
	tb_authors->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	tb_authors->setWordWrapMode(QTextOption::NoWrap);
	QString authors_text;
	authors_text += "Mateusz \'Matthew\' Marek\n";
	authors_text += "email: matthew at matthew.org.pl\n\n";
	authors_text += "Bartłomiej \'uzi\' Zimoń\n";
	authors_text += "email: uzi18 at o2.pl\n";
	tb_authors->setText(authors_text);

	// people to thank
	QTextEdit *tb_thanks = new QTextEdit(tw_about);
	tb_thanks->setReadOnly(true);
	tb_thanks->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	tb_thanks->setWordWrapMode(QTextOption::NoWrap);

	// license
	QTextEdit *tb_license = new QTextEdit(tw_about);
	tb_license->setReadOnly(true);
	tb_license->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	tb_license->setWordWrapMode(QTextOption::WordWrap);
	tb_license->setText(QString("GPL v2"));

	// changelog
	//QTextEdit *tb_changelog = new QTextEdit(tw_about);
	//tb_changelog->setReadOnly(true);
	//tb_changelog->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
	//tb_changelog->setWordWrapMode(QTextOption::NoWrap);

	// add tabs
	tw_about->addTab(tb_info, tr("&Info"));
	tw_about->addTab(tb_authors, tr("A&uthors"));
	tw_about->addTab(tb_thanks, tr("&Thanks"));
	tw_about->addTab(tb_license, tr("&License"));
	//tw_about->addTab(tb_changelog, tr("&ChangeLog"));
	// end create our info widgets

	QWidget *bottom = new QWidget;

	QWidget *blank2 = new QWidget;
	blank2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));

	QPushButton *pb_close = new QPushButton(tr("Close"));
	connect(pb_close, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout *bottom_layout = new QHBoxLayout(bottom);
	bottom_layout->addWidget(blank2);
	bottom_layout->addWidget(pb_close);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(top);
	layout->addWidget(tw_about);
	layout->addWidget(bottom);

}

FooAbout::~FooAbout()
{
}

void FooAbout::keyPressEvent(QKeyEvent *ke_event)
{
	if (ke_event->key() == Qt::Key_Escape)
		close();
}
