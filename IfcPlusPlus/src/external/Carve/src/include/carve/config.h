/* include/carve/config.h.  Generated from config.h.in by configure.  */
#pragma once

#include <math.h>

#if defined(_MSC_VER)
#pragma warning(disable : 4201)

#include <math.h>

#if(_MSC_VER < 1900)
static inline double round(double value) {
  return (value >= 0) ? floor(value + 0.5) : ceil(value - 0.5);
}
#endif
#endif