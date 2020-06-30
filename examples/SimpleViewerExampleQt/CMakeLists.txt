CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

IF(NOT WIN32)
    IF(NOT CMAKE_BUILD_TYPE)
        SET(CMAKE_BUILD_TYPE "Release")
    ENDIF()

    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ENDIF(NOT WIN32)

# Qt library
message(STATUS "Qt5 is used")
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgUtil osgGA osgFX osgSim osgText osgViewer)

SET(IFCPPVIEWER_SOURCE_FILES 
    src/IfcPlusPlusSystem.cpp
    src/main.cpp
    src/cmd/CmdRemoveSelectedObjects.cpp
    src/cmd/CmdWriteIfcFile.cpp
    src/cmd/Command.cpp
    src/cmd/CommandManager.cpp
    src/cmd/LoadIfcFileCommand.cpp
    src/gui/IfcTreeWidget.cpp
    src/gui/MainWindow.cpp
    src/gui/TabReadWrite.cpp
    src/gui/TabView.cpp
    src/viewer/GraphicsWindowQt.cpp
    src/viewer/OrbitCameraManipulator.cpp
    src/viewer/OrbitCameraManipulator.cpp
    src/viewer/ViewerWidget.cpp
)

SET(IFCPPVIEWER_MOC_FILES
    src/IfcPlusPlusSystem.h
    src/gui/IfcTreeWidget.h
    src/gui/MainWindow.h
    src/gui/TabReadWrite.h
    src/gui/TabView.h
    src/viewer/ViewerWidget.h
)

QT5_WRAP_CPP( IFCPPVIEWER_MOC_GENERATED_FILES ${IFCPPVIEWER_MOC_FILES})

ADD_EXECUTABLE(SimpleViewerExample 
    ${IFCPPVIEWER_MOC_GENERATED_FILES}
    ${IFCPPVIEWER_SOURCE_FILES}
    ${SimpleViewerExample_RESOURCES_RCC}
)

set_target_properties(SimpleViewerExample PROPERTIES DEBUG_POSTFIX "d")

If(UNIX)
    # ON Debian Stretch with cmake 3.7.2 OPENTHREADS_LIBRARY_DEBUG is a not known variable
    TARGET_LINK_LIBRARIES(SimpleViewerExample ${OPENTHREADS_LIBRARY})
else(UNIX)
    TARGET_LINK_LIBRARIES(SimpleViewerExample optimized ${OPENTHREADS_LIBRARY} debug ${OPENTHREADS_LIBRARY_DEBUG})
ENDIF()

if(USE_OSG_DEBUG)
    target_link_libraries(SimpleViewerExample
        IfcPlusPlus
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::OpenGL
        optimized ${OSG_LIBRARY} debug ${OSG_LIBRARY_DEBUG}
        optimized ${OSGDB_LIBRARY} debug ${OSGDB_LIBRARY_DEBUG}
        optimized ${OSGDB_LIBRARY} debug ${OSGDB_LIBRARY_DEBUG}
        optimized ${OSGGA_LIBRARY} debug ${OSGGA_LIBRARY_DEBUG}
        optimized ${OSGUTIL_LIBRARY} debug ${OSGUTIL_LIBRARY_DEBUG}
        optimized ${OSGFX_LIBRARY} debug ${OSGFX_LIBRARY_DEBUG}
        optimized ${OSGVIEWER_LIBRARY} debug ${OSGVIEWER_LIBRARY_DEBUG}
        optimized ${OSGTEXT_LIBRARY} debug ${OSGTEXT_LIBRARY_DEBUG}
    )
else(USE_OSG_DEBUG)
    target_link_libraries(SimpleViewerExample
        IfcPlusPlus
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::OpenGL
        optimized ${OSG_LIBRARY}
        optimized ${OSGDB_LIBRARY}
        optimized ${OSGDB_LIBRARY}
        optimized ${OSGGA_LIBRARY}
        optimized ${OSGUTIL_LIBRARY}
        optimized ${OSGFX_LIBRARY}
        optimized ${OSGVIEWER_LIBRARY}
        optimized ${OSGTEXT_LIBRARY}
    )
endif(USE_OSG_DEBUG)

TARGET_INCLUDE_DIRECTORIES(SimpleViewerExample
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OPENTHREADS_INCLUDE_DIR}
    ${OSG_INCLUDE_DIR}
    ${OSGGA_INCLUDE_DIR}
    ${OSGDB_INCLUDE_DIR}
    ${OSGUTIL_INCLUDE_DIR}
    ${OSGANIMATION_INCLUDE_DIR}
    ${OSGTEXT_INCLUDE_DIR}
)

INSTALL(
    TARGETS SimpleViewerExample
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
