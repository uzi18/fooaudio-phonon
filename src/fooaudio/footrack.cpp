#include <QDebug>

#include "footrack.hpp"

FooTrack::FooTrack()
{
}

FooTrack::FooTrack(QUrl file) : File(file)
{
	qDebug() << "footrack " <<file.toString();
}

bool FooTrack::operator == (const FooTrack &compare) const
{
	return File == compare.File;
}

bool FooTrack::operator < (const FooTrack &compare) const
{
	// simple sort via filename ;P
	return File < compare.File;
}

bool FooTrack::operator != (const FooTrack &compare) const
{
	return !(*this == compare);
}
