set( fooaudio_SRCS
    abstractaudioplugin.cpp
    fooabout.cpp
    fooaudio.cpp
    foochangename.cpp
    foomainwindow.cpp
    fooplaylistwidget.cpp
    footabbar.cpp
    footabwidget.cpp
    foometadatamanager.cpp
    footracklist.cpp
    fooplaylistmanager.cpp
    footrack.cpp
    fooapplication.cpp
    tracklistmodel.cpp
    logic/applicationlogic.cpp
)

set( fooaudio_MOC
    abstractaudiointerface.h
    abstractaudioplugin.h
    fooabout.hpp
    foochangename.hpp
    foomainwindow.hpp
    fooplaylistmanager.hpp
    foometadatamanager.hpp
    #fooplaylistwidget.hpp
    footabbar.hpp
    footabwidget.hpp
    footrack.hpp
    footracklist.hpp
    fooapplication.h
    tracklistmodel.hpp
    logic/applicationlogic.h
)

qt4_wrap_cpp(fooaudio_MOCS ${fooaudio_MOC})
qt4_add_resources( fooaudio_SRCS resource.qrc )

add_executable( fooaudio ${fooaudio_SRCS} ${fooaudio_MOCS})

target_link_libraries(fooaudio ${QT_LIBRARIES})

######### install ####################################################

#install(TARGETS xx  ${INSTALL_TARGETS_DEFAULT_ARGS} )
#install( FILES x.x  DESTINATION  ${KCFG_INSTALL_DIR} )
#install( FILES x.y  DESTINATION  ${DATA_INSTALL_DIR}/fooaudio )
install(TARGETS fooaudio DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../bin )
