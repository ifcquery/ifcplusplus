CMAKE_MINIMUM_REQUIRED (VERSION 3.6)
project(IfcPlusPlus)

# Set a default build type if none was specified https://blog.kitware.com/cmake-and-the-default-build-type/
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

project(IFCPP)
message(STATUS "------------------------------------------------------------------------")

option(BUILD_CONSOLE_APPLICATION "Build an example CLI application" ON)
if (WIN32)
	option(BUILD_VIEWER_APPLICATION "Build the viewer example application" ON)
else(NOT WIN32)
	option(BUILD_VIEWER_APPLICATION "Build the viewer example application" OFF)
endif()
option(USE_OSG_DEBUG "Use openscenegraph debug library" OFF)

IF(NOT WIN32)
    IF("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            SET_PROPERTY(DIRECTORY APPEND PROPERTY
                COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>
            )
        ENDIF()
    ENDIF()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ELSE(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

set(IFCPP_CONFIG_DIR "share/IFCPP/cmake")
ADD_SUBDIRECTORY (IfcPlusPlus)

# Install configuration file
#INCLUDE(CMakePackageConfigHelpers)
#set(config_file_input  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/IFCPPConfig.cmake.in")
#set(config_file_output "${CMAKE_CURRENT_BINARY_DIR}/cmake/IFCPPConfig.cmake")

#CONFIGURE_PACKAGE_CONFIG_FILE(
#  ${config_file_input}
#  ${config_file_output}
# INSTALL_DESTINATION ${IFCPP_CONFIG_DIR})

#INSTALL(
#  FILES ${config_file_output}
# DESTINATION ${IFCPP_CONFIG_DIR})

#install(
#    DIRECTORY   src/ifcpp
#    DESTINATION include
#    FILES_MATCHING PATTERN "*.h"
#)

#install(
#    TARGETS IfcPlusPlus
    #EXPORT  IfcPlusPlus
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION bin
#    ARCHIVE DESTINATION bin
#)
  
IF(BUILD_CONSOLE_APPLICATION)
  ADD_SUBDIRECTORY (examples/CreateIfcWallAndWriteFile)
  ADD_SUBDIRECTORY (examples/LoadFileExample)
ENDIF()
IF(BUILD_VIEWER_APPLICATION)
  ADD_SUBDIRECTORY (examples/SimpleViewerExampleQt)
ENDIF()
