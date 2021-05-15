cmake_minimum_required (VERSION 2.8...3.7.2)

if(WIN32)
	# for vs2017
	set(VC_LIBS Bcrypt.lib)
else(WIN32)
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release")
	endif()

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
endif(WIN32)

file (GLOB ifc4_source "src/ifcpp/IFC4/lib/*.cpp")

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
endif()

add_definitions(-DIFCQUERY_AS_DYNAMIC_LIBRARY)
add_definitions(-DIFCQUERY_LIB)
add_library(IfcPlusPlus SHARED ${IFCPP_SOURCE_FILES})

link_directories( ${CMAKE_BINARY_DIR}/IfcPlusPlus/${CMAKE_BUILD_TYPE} )
link_directories( ${CMAKE_BINARY_DIR}/external/Carve/${CMAKE_BUILD_TYPE} )
target_link_libraries(IfcPlusPlus
  PUBLIC
    carve
    ${VC_LIBS}
)

target_include_directories(IfcPlusPlus
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:include>")

set_target_properties(IfcPlusPlus PROPERTIES DEBUG_POSTFIX "d")

if(WIN32)
    set_target_properties(IfcPlusPlus PROPERTIES LINK_FLAGS_DEBUG "/debug /INCREMENTAL:NO")
endif()

install(
    DIRECTORY   src/ifcpp
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(
    TARGETS IfcPlusPlus
    EXPORT  IfcPlusPlus
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION bin
)

set(target_output_filename IfcPlusPlus-targets.cmake)
set(target_output "${CMAKE_BINARY_DIR}/cmake/${target_output_filename}")

export(
  EXPORT IfcPlusPlus
  FILE ${target_output}
  NAMESPACE IFCPP::)

install(
  EXPORT IfcPlusPlus
  DESTINATION ${IFCPP_CONFIG_DIR}
  FILE ${target_output_filename}
  NAMESPACE IFCPP::)
