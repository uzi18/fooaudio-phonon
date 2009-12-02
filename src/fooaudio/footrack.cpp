#include "footrack.hpp"

FooTrack::FooTrack()
{
}

FooTrack::FooTrack(QUrl &file) : File(file)
{
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
