cmake_minimum_required (VERSION 2.8...3.7.2)
project(IfcPlusPlus)

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

set(IFCPP_SOURCE_FILES 
    src/ifcpp/IFC4X3/EntityFactory.cpp
    src/ifcpp/IFC4X3/TypeFactory.cpp
	src/ifcpp/model/BuildingGuid.cpp
    src/ifcpp/model/BuildingModel.cpp
    src/ifcpp/model/UnitConverter.cpp
    src/ifcpp/reader/AbstractReader.cpp
    src/ifcpp/reader/ReaderSTEP.cpp
    src/ifcpp/reader/ReaderXML.cpp
    src/ifcpp/reader/ReaderUtil.cpp
    src/ifcpp/writer/WriterSTEP.cpp
    src/ifcpp/writer/WriterUtil.cpp
)
    
if (MSVC)
	set(IFCPP_SOURCE_FILES 
        src/external/XUnzip.cpp  # do not add src/external/XUnzip.cpp on unix platforms
        ${IFCPP_SOURCE_FILES})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj ")
endif()

ADD_DEFINITIONS(-DIFCQUERY_AS_DYNAMIC_LIBRARY)
ADD_DEFINITIONS(-DIFCQUERY_LIB)
ADD_DEFINITIONS(-DGLM_FORCE_CTOR_INIT)
ADD_DEFINITIONS(-DGLM_FORCE_XYZW_ONLY)
ADD_DEFINITIONS(-D_HAS_AUTO_PTR_ETC=1)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(IfcPlusPlus SHARED ${IFCPP_SOURCE_FILES})

if(OpenMP_CXX_FOUND)
	ADD_DEFINITIONS(-DENABLE_OPENMP)
    TARGET_LINK_LIBRARIES(IfcPlusPlus 
		optimized OpenMP::OpenMP_CXX
		${VC_LIBS})
endif()


target_include_directories(IfcPlusPlus
  PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/external>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ifcpp/IFC4/include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ifcpp/IFC4X3/include>"
    "$<INSTALL_INTERFACE:include>")

set_target_properties(IfcPlusPlus PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(IfcPlusPlus PROPERTIES CXX_STANDARD 20)

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
