#include "fooapplication.h"

int main (int argc, char * argv [])
{
	Q_INIT_RESOURCE (resource);

    FooApplication fooApp;
    return fooApp.start(argc, argv);

} // int main (int, char **);
