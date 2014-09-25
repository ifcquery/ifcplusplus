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
  message(FATAL_ERROR "Could not find Boost libraries")
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

file (GLOB ifc4_source "src/ifcpp/IFC4/*.cpp")


set(IFCPP_SOURCE_FILES 
  src/ifcpp/IfcPPEntityBroker.cpp
  src/ifcpp/IfcPPTypeBroker.cpp
  src/ifcpp/model/IfcPPAttributeObject.cpp
  src/ifcpp/model/IfcPPException.cpp
  src/ifcpp/model/IfcPPGuid.cpp
  src/ifcpp/model/IfcPPModel.cpp
  src/ifcpp/model/IfcPPObject.cpp
  src/ifcpp/model/StatusObservable.cpp
  src/ifcpp/model/UnitConverter.cpp
  src/ifcpp/reader/IfcPPReader.cpp
  src/ifcpp/reader/IfcPPReaderSTEP.cpp
  src/ifcpp/reader/IfcPPReaderXML.cpp
  src/ifcpp/reader/ReaderUtil.cpp
  src/ifcpp/writer/IfcPPWriterSTEP.cpp
  src/ifcpp/writer/IfcPPWriterXML.cpp
  src/ifcpp/writer/WriterUtil.cpp
  src/ifcpp/xml/XmlTools.cpp

  ${ifc4_source}
)

ADD_LIBRARY(IfcPlusPlus SHARED ${IFCPP_SOURCE_FILES})


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
