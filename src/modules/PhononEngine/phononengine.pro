QT += phonon
TEMPLATE = lib subdirs
DEPENDPATH += .
INCLUDEPATH += . ../../fooaudio
DESTDIR = ../../../lib

SUBDIRS = tests

HEADERS += phononengine.h

SOURCES += phononengine.cpp
