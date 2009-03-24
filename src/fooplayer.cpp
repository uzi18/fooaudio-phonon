#include <iostream>

using namespace std;

#include "fooplayer.hpp"

FooPlayer::FooPlayer (FooMainWindow *fooMainWindow)
{
   	mainWindow = fooMainWindow;
	connect (mainWindow, SIGNAL(playSignal()), this, SLOT(play()));
	connect (mainWindow, SIGNAL(nextSignal()), this, SLOT(next()));
	connect (mainWindow, SIGNAL(prevSignal()), this, SLOT(prev()));
	connect (mainWindow, SIGNAL(stopSignal()), this, SLOT(stop()));
	connect (mainWindow, SIGNAL(pauseSignal()), this, SLOT(pause()));
}

void FooPlayer::play ()
{
   cerr << "play" << endl;
}

void FooPlayer::next ()
{
   cerr << "next" << endl;
}

void FooPlayer::stop ()
{
   cerr << "stop" << endl;
}

void FooPlayer::prev ()
{
   cerr << "prev" << endl;
}

void FooPlayer::pause ()
{
   cerr << "pause" << endl;
}


