CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

IF(WIN32)
# for vs2017
SET(VC_LIBS
	Bcrypt.lib
    )
ENDIF(WIN32)

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

FIND_PACKAGE(Boost REQUIRED COMPONENTS date_time)

FILE (GLOB ifc4_source "src/ifcpp/IFC4/lib/*.cpp")

if(UNIX)
    # do not add src/external/XUnzip.cpp on unix plattforms
    set(IFCPP_SOURCE_FILES 
        src/ifcpp/IFC4/EntityFactory.cpp
        src/ifcpp/IFC4/TypeFactory.cpp
		src/ifcpp/model/BuildingGuid.cpp
        src/ifcpp/model/BuildingModel.cpp
        src/ifcpp/model/UnitConverter.cpp
        src/ifcpp/reader/AbstractReader.cpp
        src/ifcpp/reader/ReaderSTEP.cpp
        src/ifcpp/reader/ReaderXML.cpp
        src/ifcpp/reader/ReaderUtil.cpp
        src/ifcpp/writer/WriterSTEP.cpp
        src/ifcpp/writer/WriterUtil.cpp

        ${ifc4_source}
    )
else(UNIX)
    set(IFCPP_SOURCE_FILES 
        src/ifcpp/IFC4/EntityFactory.cpp
        src/ifcpp/IFC4/TypeFactory.cpp
		src/ifcpp/model/BuildingGuid.cpp
        src/ifcpp/model/BuildingModel.cpp
        src/ifcpp/model/UnitConverter.cpp
        src/ifcpp/reader/AbstractReader.cpp
        src/ifcpp/reader/ReaderSTEP.cpp
        src/ifcpp/reader/ReaderXML.cpp
        src/ifcpp/reader/ReaderUtil.cpp
        src/ifcpp/writer/WriterSTEP.cpp
        src/ifcpp/writer/WriterUtil.cpp
        src/external/XUnzip.cpp

        ${ifc4_source}
    )
endif(UNIX)

    
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj ")
    
ENDIF()

IF(BUILD_STATIC_LIBRARY)
    ADD_LIBRARY(IfcPlusPlus STATIC ${IFCPP_SOURCE_FILES})
ELSE(BUILD_STATIC_LIBRARY)
    ADD_DEFINITIONS(-DIFCQUERY_AS_DYNAMIC_LIBRARY)
    ADD_DEFINITIONS(-DIFCQUERY_LIB)
    ADD_LIBRARY(IfcPlusPlus SHARED ${IFCPP_SOURCE_FILES})
ENDIF(BUILD_STATIC_LIBRARY)

TARGET_LINK_LIBRARIES(IfcPlusPlus
  PUBLIC
    carve
    ${Boost_LIBRARIES}
    ${VC_LIBS}
    )

TARGET_INCLUDE_DIRECTORIES(IfcPlusPlus
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:include>")

IF(Boost_FOUND)

    TARGET_INCLUDE_DIRECTORIES(IfcPlusPlus
      PUBLIC
        ${Boost_INCLUDE_DIRS})

    TARGET_COMPILE_DEFINITIONS(IfcPlusPlus
      PUBLIC
        CARVE_SYSTEM_BOOST)

ENDIF()

set_target_properties(IfcPlusPlus PROPERTIES DEBUG_POSTFIX "d")

IF(WIN32)
    set_target_properties(IfcPlusPlus PROPERTIES LINK_FLAGS_DEBUG "/debug /INCREMENTAL:NO")
ENDIF()


INSTALL(
    DIRECTORY   src/ifcpp
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
INSTALL(
    TARGETS IfcPlusPlus
    EXPORT  IfcPlusPlus
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

SET(target_output_filename IfcPlusPlus-targets.cmake)
SET(target_output "${CMAKE_BINARY_DIR}/cmake/${target_output_filename}")

export(
  EXPORT IfcPlusPlus
  FILE ${target_output}
  NAMESPACE IFCPP::)

install(
  EXPORT IfcPlusPlus
  DESTINATION ${IFCPP_CONFIG_DIR}
  FILE ${target_output_filename}
  NAMESPACE IFCPP::)
