#pragma once

// Choose one of the following four options.

// Define if using gnu libstdc++.
#define HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS 

// Define if std::unordered_map and std::unordered_set are supported by your compiler.
// #define HAVE_STD_UNORDERED_COLLECTIONS

// Define if using std::tr1 collections.
//#define HAVE_TR1_UNORDERED_COLLECTIONS

// Define if using boost collections.
// #define HAVE_BOOST_UNORDERED_COLLECTIONS
// Define to use system installed boost.
// #define CARVE_SYSTEM_BOOST


// Other options.

// Define to include debugging output in carve.
// #define CARVE_DEBUG

// Write intermediate debugging info in .ply format.
// #define CARVE_DEBUG_WRITE_PLY_DATA

// Define to make carve use exact predicates where available.
// #cmakedefine CARVE_USE_EXACT_PREDICATES
