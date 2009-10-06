cmake_minimum_required(VERSION 2.6)

project(fooaudio)

find_package(Qt4 REQUIRED)
find_package(PkgConfig REQUIRED)

include(${QT_USE_FILE})

pkg_check_modules(PHONON REQUIRED phonon)

include_directories( ${PHONON_INCLUDE_DIRS} )
link_directories( ${PHONON_LIBRARY_DIRS} )
add_definitions( ${QT_DEFINITIONS} ${PHONON_CFLAGS_OTHER} )

#package includedir libdir linkflags cflags
#IF(NOT QT_PHONON_FOUND)
#    MESSAGE( FATAL_ERROR "Install phonon devel libs.")
#ENDIF(NOT QT_PHONON_FOUND)

add_subdirectory( src )
