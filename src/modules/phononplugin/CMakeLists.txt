#project(phononplugin)

set(phononplugin_SRCS
	phononengine.cpp
	phononplugin.cpp
	../../fooaudio/abstractaudioplugin.cpp
)

set(phononplugin_MOC
	phononengine.h
	phononplugin.h
	../../fooaudio/abstractaudioplugin.h
	../../fooaudio/abstractaudiointerface.h
)

include_directories(
	${QT_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../fooaudio
)

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
#add_definitions(-DQT_MODULE)

qt4_wrap_cpp(phononplugin_MOCS ${phononplugin_MOC})
add_library(phononplugin SHARED ${phononplugin_SRCS} ${phononplugin_MOCS})
add_dependencies(phononplugin ${PHONON_LIBRARIES})

set_target_properties(phononplugin PROPERTIES
       link_flags "${PHONON_LDFLAGS_OTHER}" )

target_link_libraries(phononplugin	${PHONON_LIBRARIES} ${QT_LIBRARIES})

######### install ####################################################
#install(TARGETS xx  ${INSTALL_TARGETS_DEFAULT_ARGS} )
#install( FILES x.x  DESTINATION  ${KCFG_INSTALL_DIR} )
#install( FILES x.y  DESTINATION  ${DATA_INSTALL_DIR}/fooaudio )
install(TARGETS phononplugin DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)
