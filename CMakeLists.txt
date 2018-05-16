CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

# Set a default build type if none was specified https://blog.kitware.com/cmake-and-the-default-build-type/
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

PROJECT (IFCPP)
MESSAGE(STATUS "------------------------------------------------------------------------")

OPTION(BUILD_VIEWER_APPLICATION "Build the viewer example application" ON)
OPTION(BUILD_STATIC_LIBRARY "Build static library" ON)

IF(NOT WIN32)
IF("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET_PROPERTY(DIRECTORY APPEND PROPERTY  
      COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>
    )
  ENDIF()
ENDIF()
ELSE(NOT WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

SET(CARVE_SOURCE_DIR ${IFCPP_SOURCE_DIR}/external/Carve)
SET(CARVE_BINARY_DIR ${IFCPP_BINARY_DIR})
ADD_SUBDIRECTORY (external/Carve)
ADD_SUBDIRECTORY (IfcPlusPlus)

IF(BUILD_VIEWER_APPLICATION)
  ADD_SUBDIRECTORY (SimpleViewerExample)
ENDIF()
