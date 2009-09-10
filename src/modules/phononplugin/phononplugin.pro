QT += phonon
TEMPLATE = lib
DEPENDPATH += . ../../fooaudio
INCLUDEPATH += . ../../fooaudio
DESTDIR = ../../../lib
CONFIG += plugin

HEADERS +=  phononengine.h \
            phononplugin.h \
            abstractaudioplugin.h \
            abstractaudiointerface.h

SOURCES +=  phononengine.cpp \
            phononplugin.cpp \ 
            abstractaudioplugin.cpp

