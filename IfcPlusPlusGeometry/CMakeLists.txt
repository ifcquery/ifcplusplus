CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

SET(IFCPPGEOMETRY_HEADER_FILES 
  src/ifcppgeometry/ConverterOSG.h
  src/ifcppgeometry/CurveConverter.h
  src/ifcppgeometry/CSG_Adapter.h
  src/ifcppgeometry/FaceConverter.h
  src/ifcppgeometry/GeometryConverter.h
  src/ifcppgeometry/GeometryInputData.h
  src/ifcppgeometry/GeometrySettings.h
  src/ifcppgeometry/GeomUtils.h
  src/ifcppgeometry/GeometryInputData.h
  src/ifcppgeometry/PlacementConverter.h
  src/ifcppgeometry/ProfileCache.h
  src/ifcppgeometry/ProfileConverter.h
  src/ifcppgeometry/RepresentationConverter.h
  src/ifcppgeometry/SolidModelConverter.h
  src/ifcppgeometry/StylesConverter.h
  src/ifcppgeometry/Sweeper.h
)

SET(IFCPPGEOMETRY_SOURCE_FILES 
  src/ifcppgeometry/ConverterOSG.cpp
  src/ifcppgeometry/CSG_Adapter.cpp
  src/ifcppgeometry/CurveConverter.cpp
  src/ifcppgeometry/FaceConverter.cpp
  src/ifcppgeometry/GeometryConverter.cpp
  src/ifcppgeometry/GeometryInputData.cpp
  src/ifcppgeometry/GeometrySettings.cpp
  src/ifcppgeometry/GeomUtils.cpp
  src/ifcppgeometry/PlacementConverter.cpp
  src/ifcppgeometry/PointConverter.cpp
  src/ifcppgeometry/ProfileCache.cpp
  src/ifcppgeometry/ProfileConverter.cpp
  src/ifcppgeometry/RepresentationConverter.cpp
  src/ifcppgeometry/SolidModelConverter.cpp
  src/ifcppgeometry/SplineConverter.cpp
  src/ifcppgeometry/StylesConverter.cpp
  src/ifcppgeometry/Sweeper.cpp
)

FIND_PACKAGE(Boost) 
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 
	ADD_DEFINITIONS( -DCARVE_SYSTEM_BOOST )
ENDIF()

FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgUtil osgGA osgFX osgSim osgText osgViewer)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/common)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) 
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${IFCPP_SOURCE_DIR}/IfcPlusPlusGeometry/src/)
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OPENTHREADS_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSG_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGDB_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGUTIL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSGANIMATION_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRECTORIES} ${OSG_BINARY_DIR}/include ) # For the generated Config file

LINK_DIRECTORIES (${LINK_DIRECTORIES} ${IFCPP_BINARY_DIR}/${CMAKE_BUILD_TYPE}) # To find the IfcPlusPlus library
LINK_DIRECTORIES (${LINK_DIRECTORIES} ${CARVE_BINARY_DIR}/lib) 
LINK_DIRECTORIES (${LINK_DIRECTORIES} ${CARVE_BINARY_DIR}/common) 

IF(CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
	ADD_LIBRARY(IfcPlusPlusGeometry SHARED ${IFCPPGEOMETRY_HEADER_FILES} ${IFCPPGEOMETRY_SOURCE_FILES})
ELSE(CMAKE_COMPILER_IS_GNUCXX)
	ADD_LIBRARY(IfcPlusPlusGeometry STATIC ${IFCPPGEOMETRY_HEADER_FILES} ${IFCPPGEOMETRY_SOURCE_FILES})
ENDIF()

TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry carve)
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry IfcPlusPlus)
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OPENTHREADS_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSG_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGDB_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGUTIL_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGANIMATION_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGFX_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGTEXT_LIBRARY} )
TARGET_LINK_LIBRARIES( IfcPlusPlusGeometry ${OSGVIEWER_LIBRARY} )