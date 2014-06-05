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

configure_file (
  "${CARVE_SOURCE_DIR}/src/include/carve/cmake-config.h.in"
  "${CARVE_BINARY_DIR}/include/carve/config.h"
)
add_definitions(-DCMAKE_BUILD)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file


find_package(Boost 1.53)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Using Boost libraries for Carve")
else(Boost_FOUND)
  message(STATUS "Could not find Boost libraries for Carve")
endif(Boost_FOUND)

set(CARVE_HEADER_FILES 
  src/common/geometry.hpp
  src/common/opts.hpp
  src/common/rgb.hpp
  src/common/stringfuncs.hpp  
 
  src/include/carve/aabb_impl.hpp
  src/include/carve/carve.hpp
  src/include/carve/cbrt.h
  src/include/carve/classification.hpp

  src/include/carve/collection/unordered/boost_impl.hpp
  src/include/carve/collection/unordered/fallback_impl.hpp
  src/include/carve/collection/unordered/libstdcpp_impl.hpp
  src/include/carve/collection/unordered/std_impl.hpp
  src/include/carve/collection/unordered/tr1_impl.hpp
  src/include/carve/collection/unordered/vcpp_impl.hpp

  src/include/carve/collection/unordered.hpp

  src/include/carve/collection.hpp
  src/include/carve/collection_types.hpp
  src/include/carve/colour.hpp
  src/include/carve/convex_hull.hpp
  src/include/carve/csg.hpp
  src/include/carve/csg_triangulator.hpp
  src/include/carve/debug_hooks.hpp
  src/include/carve/djset.hpp
  src/include/carve/edge_decl.hpp
  src/include/carve/edge_impl.hpp
  src/include/carve/exact.hpp
  src/include/carve/face_decl.hpp
  src/include/carve/face_impl.hpp
  src/include/carve/faceloop.hpp
  src/include/carve/geom.hpp
  src/include/carve/geom2d.hpp
  src/include/carve/geom3d.hpp
  src/include/carve/geom_impl.hpp
  src/include/carve/gnu_cxx.h
  src/include/carve/heap.hpp
  src/include/carve/input.hpp
  src/include/carve/interpolator.hpp
  src/include/carve/intersection.hpp
  src/include/carve/iobj.hpp
  src/include/carve/kd_node.hpp
  src/include/carve/math.hpp
  src/include/carve/math_constants.hpp
  src/include/carve/matrix.hpp
  src/include/carve/mesh.hpp
  src/include/carve/mesh_impl.hpp
  src/include/carve/mesh_ops.hpp
  src/include/carve/mesh_simplify.hpp
  src/include/carve/octree_decl.hpp
  src/include/carve/octree_impl.hpp
  src/include/carve/pointset.hpp
  src/include/carve/pointset_decl.hpp
  src/include/carve/pointset_impl.hpp
  src/include/carve/pointset_iter.hpp
  src/include/carve/poly.hpp
  src/include/carve/poly_decl.hpp
  src/include/carve/poly_impl.hpp
  src/include/carve/polyhedron_base.hpp
  src/include/carve/polyhedron_decl.hpp
  src/include/carve/polyhedron_impl.hpp
  src/include/carve/polyline.hpp
  src/include/carve/polyline_decl.hpp
  src/include/carve/polyline_impl.hpp
  src/include/carve/polyline_iter.hpp
  src/include/carve/rescale.hpp
  src/include/carve/rtree.hpp
  src/include/carve/shewchuk_predicates.hpp
  src/include/carve/spacetree.hpp
  src/include/carve/tag.hpp
  src/include/carve/timing.hpp
  src/include/carve/tree.hpp
  src/include/carve/triangle_intersection.hpp
  src/include/carve/triangulator.hpp
  src/include/carve/triangulator_impl.hpp
  src/include/carve/util.hpp
  src/include/carve/vcpp_config.h
  src/include/carve/vector.hpp
  src/include/carve/vertex_decl.hpp
  src/include/carve/vertex_impl.hpp
  src/include/carve/win32.h
  src/include/carve/xcode_config.h
 
  src/lib/intersect_classify_common.hpp
  src/lib/intersect_classify_common_impl.hpp
  src/lib/csg_collector.hpp
  src/lib/csg_data.hpp
  src/lib/csg_detail.hpp
)

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
  src/lib/intersect_common.hpp
  src/lib/intersect_debug.cpp
  src/lib/intersect_debug.hpp
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

ADD_LIBRARY(carve SHARED ${CARVE_HEADER_FILES} ${CARVE_SOURCE_FILES})

add_subdirectory(src/common) 
