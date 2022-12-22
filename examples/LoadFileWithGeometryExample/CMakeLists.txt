CMAKE_MINIMUM_REQUIRED (VERSION 3.7.2)

IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

IF(NOT WIN32)
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
ENDIF(NOT WIN32)

ADD_DEFINITIONS(-DGLM_FORCE_CTOR_INIT)
ADD_DEFINITIONS(-DGLM_FORCE_XYZW_ONLY)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
ADD_DEFINITIONS(-DIFCQUERY_STATIC_LIB)
ADD_DEFINITIONS(-DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_CPP)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED USE_OPENMP)
  set(USE_OPENMP True)
endif()

if(USE_OPENMP)
  find_package(OpenMP)
endif()

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/Debug)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/IfcPlusPlus/${CMAKE_BUILD_TYPE})

ADD_EXECUTABLE(LoadFileWithGeometryExample 
    ${IFCPP_SOURCE_DIR}/examples/LoadFileWithGeometryExample/src/main.cpp
)

set_target_properties(LoadFileWithGeometryExample PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(LoadFileWithGeometryExample PROPERTIES CXX_STANDARD 17)

if(OpenMP_CXX_FOUND)
    TARGET_LINK_LIBRARIES(LoadFileWithGeometryExample 
		optimized OpenMP::OpenMP_CXX
		optimized IfcPlusPlus debug IfcPlusPlusd)
		
else(OpenMP_CXX_FOUND)
	TARGET_LINK_LIBRARIES(LoadFileWithGeometryExample 
		optimized IfcPlusPlus debug IfcPlusPlusd)
endif()

TARGET_INCLUDE_DIRECTORIES(LoadFileWithGeometryExample
    PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/ifcpp/IFC4X3/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/glm
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/Carve/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/Carve/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/Carve/src/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/Carve/src/common
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/Carve/build/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/glm
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/utilities/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/utilities
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/polygon/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/manifold/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/collider/include
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/third_party
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/third_party/thrust
	${IFCPP_SOURCE_DIR}/IfcPlusPlus/src/external/manifold/src/third_party/graphlite/include
)
		
INSTALL(
    TARGETS LoadFileWithGeometryExample
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
    ARCHIVE DESTINATION lib
)
