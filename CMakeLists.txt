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

project(IFCPP)
MESSAGE(STATUS "------------------------------------------------------------------------")

OPTION(BUILD_VIEWER_APPLICATION "Build the viewer example application" ON)
OPTION(BUILD_STATIC_LIBRARY "Build static library" ON)

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
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF(NOT WIN32)

SET(CARVE_SOURCE_DIR ${IFCPP_SOURCE_DIR}/external/Carve)
SET(CARVE_BINARY_DIR ${IFCPP_BINARY_DIR})
SET(IFCPP_CONFIG_DIR "share/IFCPP/cmake")
ADD_SUBDIRECTORY (external/Carve)
ADD_SUBDIRECTORY (IfcPlusPlus)

# Install configuration file
INCLUDE(CMakePackageConfigHelpers)
SET(config_file_input  "${CMAKE_SOURCE_DIR}/cmake/IFCPPConfig.cmake.in")
SET(config_file_output "${CMAKE_BINARY_DIR}/cmake/IFCPPConfig.cmake")

CONFIGURE_PACKAGE_CONFIG_FILE(
  ${config_file_input}
  ${config_file_output}
  INSTALL_DESTINATION ${IFCPP_CONFIG_DIR})

INSTALL(
  FILES ${config_file_output}
  DESTINATION ${IFCPP_CONFIG_DIR})

IF(BUILD_VIEWER_APPLICATION)
  ADD_SUBDIRECTORY (examples/SimpleViewerExampleQt)
ENDIF()
