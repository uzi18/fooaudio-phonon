QT += phonon
TEMPLATE = app
DEPENDPATH += . ../../../fooaudio
INCLUDEPATH += . ../../../fooaudio
DESTDIR = .
CONFIG += qtestlib

HEADERS +=  tests.h \
            abstractaudioplugin.h \

SOURCES +=  tests.cpp \
            abstractaudioplugin.cpp \

