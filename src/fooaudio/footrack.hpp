/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FOOTRACK_HPP
#define FOOTRACK_HPP

#include <QString>
#include <QUrl>

class FooTrack
{
	QUrl File;

	QString Title;
	QString Artist;
	QString Year;
	QString Album;
	QString Track;

public:
	FooTrack();
	FooTrack(QUrl &file);

	const QUrl & file() { return File; }

	const QString & title() { return Title; }
	void setTitle (QString title) { Title=title; }
	const QString & artist() { return Artist; }
	void setArtist (QString artist) { Artist=artist; }
	const QString & year() { return Year; }
	void setYear (QString year) { Year=year; }
	const QString & album() { return Album; }
	void setAlbum (QString album) { Album=album; }
	const QString & track() { return Track; }
	void setTrack (QString track) { Track=track; }
	
	bool operator == (const FooTrack &compare) const;
	bool operator < (const FooTrack &compare) const;
	bool operator != (const FooTrack &compare) const;
};

#endif
