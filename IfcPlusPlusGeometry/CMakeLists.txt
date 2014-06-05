cmake_minimum_required (VERSION 2.8)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

set(IFCPPGEOMETRY_HEADER_FILES 
  src/ifcppgeometry/ConverterOSG.h
  src/ifcppgeometry/CurveConverter.h
  src/ifcppgeometry/CSG_Adapter.h
  src/ifcppgeometry/FaceConverter.h
  src/ifcppgeometry/GeometryInputData.h
  src/ifcppgeometry/GeometrySettings.h
  src/ifcppgeometry/GeomUtils.h
  src/ifcppgeometry/GeometryInputData.h
  src/ifcppgeometry/PlacementConverter.h
  src/ifcppgeometry/ProfileCache.h
  src/ifcppgeometry/ProfileConverter.h
  src/ifcppgeometry/ReaderWriterIFC.h
  src/ifcppgeometry/RepresentationConverter.h
  src/ifcppgeometry/SolidModelConverter.h
  src/ifcppgeometry/StylesConverter.h
)

set(IFCPPGEOMETRY_SOURCE_FILES 
  src/ifcppgeometry/ConverterOSG.cpp
  src/ifcppgeometry/CurveConverter.cpp
  src/ifcppgeometry/CSG_Adapter.cpp
  src/ifcppgeometry/FaceConverter.cpp
  src/ifcppgeometry/GeometrySettings.cpp
  src/ifcppgeometry/GeomUtils.cpp
  src/ifcppgeometry/GeometryInputData.cpp
  src/ifcppgeometry/PlacementConverter.cpp
  src/ifcppgeometry/ProfileCache.cpp
  src/ifcppgeometry/ProfileConverter.cpp
  src/ifcppgeometry/ReaderWriterIFC.cpp
  src/ifcppgeometry/RepresentationConverter.cpp
  src/ifcppgeometry/SectionedSpine.cpp
  src/ifcppgeometry/SolidModelConverter.cpp
  src/ifcppgeometry/StylesConverter.cpp
)

find_package(Boost 1.53)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Using Boost libraries for Carve")
else(Boost_FOUND)
  message(STATUS "Could not find Boost libraries for Carve")
endif(Boost_FOUND)

# Carve library
message(STATUS "IfcPlusPlusGeometry: directories of Carve")
message(STATUS "source: " ${CARVE_SOURCE_DIR})
message(STATUS "binary: " ${CARVE_BINARY_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/common)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${IFCPP_SOURCE_DIR}/IfcPlusPlusGeometry/src/)

FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgUtil osgGA osgFX osgSim osgText osgViewer)

INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OPENTHREADS_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSG_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGDB_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGUTIL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGANIMATION_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSG_BINARY_DIR}/include ) # For the generated Config file

LINK_DIRECTORIES (${LINK_DIRECTORIES} ${IFCPP_BINARY_DIR}/${CMAKE_BUILD_TYPE}) # To find the IfcPlusPlus library

# Linking to the Carve library
LINK_DIRECTORIES (${LINK_DIRECTORIES} ${CARVE_BINARY_DIR}/lib) 
LINK_DIRECTORIES (${LINK_DIRECTORIES} ${CARVE_BINARY_DIR}/common) 

ADD_LIBRARY(IfcPlusPlusGeometry SHARED ${IFCPPGEOMETRY_HEADER_FILES} ${IFCPPGEOMETRY_SOURCE_FILES})

TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry carve carve_misc)
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry IfcPlusPlus)
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OPENTHREADS_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSG_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGDB_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGUTIL_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGANIMATION_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGFX_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGTEXT_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGVIEWER_LIBRARY} )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
