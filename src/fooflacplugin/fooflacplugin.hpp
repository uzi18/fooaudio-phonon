#ifndef _FOOFLACPLUGIN_HPP_
#define _FOOFLACPLUGIN_HPP_

#include <QObject>
#include <<FLAC/all.h>

#include "../include/fooaudiostruct.hpp"
#include "fooalsaplugin.hpp"
#include "../include/fooplugininterfaces.hpp"

class FooFlacPlugin : public QObject, public FooMusicFormatInterface
{
	Q_OBJECT
	Q_INTERFACES(FooMusicFormatInterface)

public:
	void *open (const char *);

private:
	void *openInternal (const char *, const int);
	void close (void *);
};

#endif // _FOOFLACPLUGIN_HPP_

