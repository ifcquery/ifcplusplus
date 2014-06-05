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

find_package(Boost 1.53)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Using Boost libraries")
else(Boost_FOUND)
  message(STATUS "Could not find Boost libraries")
endif(Boost_FOUND)

# Carve library
message(STATUS "IfcPlusPlus: directories of Carve")
message(STATUS "source: " ${CARVE_SOURCE_DIR})
message(STATUS "binary: " ${CARVE_BINARY_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/common)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} src/)

LINK_DIRECTORIES (${LINK_DIRECTORIES} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}) 

file (GLOB ifc4_headers "src/ifcpp/IFC4/include/*.h")

file (GLOB ifc4_source "src/ifcpp/IFC4/*.cpp")

set(IFCPP_HEADER_FILES 
  src/ifcpp/IfcPPEntities.h 
  src/ifcpp/IfcPPEntitiesMap.h
  src/ifcpp/IfcPPEntityEnums.h
  src/ifcpp/IfcPPTypeEnums.h
  src/ifcpp/IfcPPTypeMap.h
  src/ifcpp/IfcPPTypes.h
  src/ifcpp/guid/CreateGuid_64.h
  src/ifcpp/model/IfcPPAttributeObject.h
  src/ifcpp/model/IfcPPException.h
  src/ifcpp/model/IfcPPModel.h
  src/ifcpp/model/IfcPPObject.h
  src/ifcpp/model/IfcPPOpenMP.h
  src/ifcpp/model/IfcPPUtil.h
  src/ifcpp/model/StatusObservable.h
  src/ifcpp/model/UnitConverter.h
  src/ifcpp/model/UnknownEntityException.h
  src/ifcpp/model/shared_ptr.h
  src/ifcpp/reader/IfcPlusPlusReader.h
  src/ifcpp/reader/IfcStepReader.h
  src/ifcpp/reader/IfcXmlReader.h
  src/ifcpp/reader/ReaderUtil.h
  src/ifcpp/writer/IfcPlusPlusWriter.h
  src/ifcpp/writer/IfcStepWriter.h
  src/ifcpp/writer/IfcXmlWriter.h
  src/ifcpp/writer/WriterUtil.h
  src/ifcpp/xml/XmlTools.h

  ${ifc4_headers}

)

set(IFCPP_SOURCE_FILES 
  src/ifcpp/IfcPPEntityBroker.cpp
  src/ifcpp/IfcPPTypeBroker.cpp
  src/ifcpp/guid/CreateGuid_64.cpp
  src/ifcpp/model/IfcPPAttributeObject.cpp
  src/ifcpp/model/IfcPPException.cpp
  src/ifcpp/model/IfcPPModel.cpp
  src/ifcpp/model/IfcPPObject.cpp
  src/ifcpp/model/StatusObservable.cpp
  src/ifcpp/model/UnitConverter.cpp
  src/ifcpp/reader/IfcPlusPlusReader.cpp
  src/ifcpp/reader/IfcStepReader.cpp
  src/ifcpp/reader/IfcXmlReader.cpp
  src/ifcpp/reader/ReaderUtil.cpp
  src/ifcpp/writer/IfcStepWriter.cpp
  src/ifcpp/writer/IfcXmlWriter.cpp
  src/ifcpp/writer/WriterUtil.cpp
  src/ifcpp/xml/XmlTools.cpp

  ${ifc4_source}
)

ADD_LIBRARY(IfcPlusPlus SHARED ${IFCPP_HEADER_FILES} ${IFCPP_SOURCE_FILES})


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
