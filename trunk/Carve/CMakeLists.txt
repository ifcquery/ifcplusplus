cmake_minimum_required (VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)


message(STATUS "Carve: directories of Carve")
message(STATUS "source: " ${CARVE_SOURCE_DIR})
message(STATUS "binary: " ${CARVE_BINARY_DIR})

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)
set(HAVE_TR1_UNORDERED_COLLECTIONS FALSE)set(HAVE_STD_UNORDERED_COLLECTIONS FALSE)set(HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS FALSE)set(HAVE_BOOST_UNORDERED_COLLECTIONS FALSE)if(CARVE_SYSTEM_BOOST)	find_package(Boost 1.40)	if(Boost_FOUND)		include_directories(${Boost_INCLUDE_DIRS})		message(STATUS "Using system boost")	else(Boost_FOUND)		set(CARVE_SYSTEM_BOOST OFF)	endif(Boost_FOUND)endif(CARVE_SYSTEM_BOOST)if(CARVE_BOOST_COLLECTIONS)	set(HAVE_BOOST_UNORDERED_COLLECTIONS TRUE)else(CARVE_BOOST_COLLECTIONS)	# attempt to work out which unordered collection implementation we can use	try_compile(_HAVE_STD_UNORDERED_COLLECTIONS ${CMAKE_BINARY_DIR} "${carve_SOURCE_DIR}/cmake/test_std_unordered.cpp" OUTPUT_VARIABLE OUTPUT)	try_compile(_HAVE_TR1_UNORDERED_COLLECTIONS ${CMAKE_BINARY_DIR} "${carve_SOURCE_DIR}/cmake/test_tr1_unordered.cpp" OUTPUT_VARIABLE OUTPUT)
	try_compile(_HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS ${CMAKE_BINARY_DIR} "${carve_SOURCE_DIR}/cmake/test_libstdcpp_unordered.cpp" OUTPUT_VARIABLE OUTPUT)
	if(_HAVE_STD_UNORDERED_COLLECTIONS)		set(HAVE_STD_UNORDERED_COLLECTIONS TRUE)
		message(STATUS "Using std::unordered_map")	elseif(_HAVE_TR1_UNORDERED_COLLECTIONS)		set(HAVE_TR1_UNORDERED_COLLECTIONS TRUE)		message(STATUS "Using tr1::unordered_map")	elseif(_HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS)		set(HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS TRUE)		message(STATUS "Using __gnu_cxx::unordered_map ")	endif(_HAVE_STD_UNORDERED_COLLECTIONS)endif(CARVE_BOOST_COLLECTIONS)
set(carve_VERSION_MAJOR 2)set(carve_VERSION_MINOR 0)set(carve_VERSION_PATCH 0a)set(CARVE_VERSION ${carve_VERSION_MAJOR}.${carve_VERSION_MINOR}.${carve_VERSION_PATCH})

configure_file (
  "${CARVE_SOURCE_DIR}/src/include/carve/cmake-config.h.in"
  "${CARVE_BINARY_DIR}/include/carve/config.h"
)
add_definitions(-DCMAKE_BUILD)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file

set(CARVE_SOURCE_FILES 
  src/lib/aabb.cpp
  src/lib/carve.cpp
  src/lib/convex_hull.cpp
  src/lib/csg.cpp
  src/lib/csg_collector.cpp
  src/lib/edge.cpp
  src/lib/face.cpp
  src/lib/geom.cpp
  src/lib/geom2d.cpp
  src/lib/geom3d.cpp
  src/lib/intersect.cpp
  src/lib/intersect_classify_edge.cpp
  src/lib/intersect_classify_group.cpp
  src/lib/intersect_debug.cpp
  src/lib/intersect_face_division.cpp
  src/lib/intersect_group.cpp
  src/lib/intersect_half_classify_group.cpp
  src/lib/intersection.cpp
  src/lib/math.cpp
  src/lib/mesh.cpp
  src/lib/octree.cpp
  src/lib/pointset.cpp
  src/lib/polyhedron.cpp
  src/lib/polyline.cpp
  src/lib/shewchuk_predicates.cpp
  src/lib/tag.cpp
  src/lib/timing.cpp
  src/lib/triangle_intersection.cpp
  src/lib/triangulator.cpp
)

ADD_LIBRARY(carve SHARED ${CARVE_SOURCE_FILES})

add_subdirectory(src/common) 
