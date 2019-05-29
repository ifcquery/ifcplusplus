/* src/Inventor/Win/SoWinBasic.h.  Generated from - by configure.  */
#ifndef SOWIN_BASIC_H
#define SOWIN_BASIC_H

// NB: this is not a pure configure-input file, it's also a config header...

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// *************************************************************************

/* Some useful inline template functions:
 *   SoWinMin(Val1, Val2)       - returns minimum value
 *   SoWinMax(Val1, Val2)       - returns maximum value
 *   SoWinClamp(Val, Min, Max)  - returns clamped value
 *   SoWinSwap(Val1, Val2)      - swaps the two values (no return value)
 */

template <class Type>
inline Type SoWinAbs(Type Val) {
  return (Val < 0) ? -Val : Val;
}

template <class Type>
inline Type SoWinMin(Type a, Type b) {
  return (b < a) ? b : a;
}

template <class Type>
inline Type SoWinMax(Type a, Type b) {
  return (b > a) ? b : a;
}

template <class Type>
inline Type SoWinClamp(Type val, Type min, Type max) {
  return SoWinMax(min, SoWinMin(max, val));
}

template <class Type>
inline void SoWinSwap(Type & a, Type & b) {
  Type t = a; a = b; b = t;
}

// *************************************************************************

#define __COIN_SOWIN__

#if ! defined(SOWIN_MAJOR_VERSION)
#define SOWIN_MAJOR_VERSION 1
#endif /* ! SOWIN_MAJOR_VERSION */
#if ! defined(SOWIN_MINOR_VERSION)
#define SOWIN_MINOR_VERSION 6
#endif /* ! SOWIN_MINOR_VERSION */
#if ! defined(SOWIN_MICRO_VERSION)
#define SOWIN_MICRO_VERSION 0
#endif /* ! SOWIN_MICRO_VERSION */
#if ! defined(SOWIN_BETA_VERSION)
/* #undef SOWIN_BETA_VERSION */
#endif /* ! SOWIN_BETA_VERSION */
#if ! defined(SOWIN_VERSION)
#define SOWIN_VERSION "1.6.0a"
#endif /* ! SOWIN_VERSION */

// *************************************************************************

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef SOWIN_DLL_API
# error Leave the internal SOWIN_DLL_API define alone.
#endif /* SOWIN_DLL_API */

/*
  On MSWindows platforms, one of these defines must always be set when
  building application programs:

   - "SOWIN_DLL", when the application programmer is using the
     library in the form of a dynamic link library (DLL)

   - "SOWIN_NOT_DLL", when the application programmer is using the
     library in the form of a static object library (LIB)

  Note that either SOWIN_DLL or SOWIN_NOT_DLL _must_ be defined by
  the application programmer on MSWindows platforms, or else the
  #error statement will hit. Set up one or the other of these two
  defines in your compiler environment according to how the library
  was built -- as a DLL (use "SOWIN_DLL") or as a LIB (use
  "SOWIN_NOT_DLL").

  (Setting up defines for the compiler is typically done by either
  adding something like "/DSOWIN_DLL" to the compiler's argument
  line (for command-line build processes), or by adding the define to
  the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
  this is done from the "Project"->"Settings" menu, choose the "C/C++"
  tab, then "Preprocessor" from the dropdown box and add the
  appropriate define)).

  It is extremely important that the application programmer uses the
  correct define, as using "SOWIN_NOT_DLL" when "SOWIN_DLL" is
  correct is likely to cause mysterious crashes.
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef SOWIN_INTERNAL
#  ifdef SOWIN_MAKE_DLL
#   define SOWIN_DLL_API __declspec(dllexport)
#  endif /* SOWIN_MAKE_DLL */
# else /* !SOWIN_INTERNAL */
#  ifdef SOWIN_DLL
#   define SOWIN_DLL_API __declspec(dllimport)
#  else /* !SOWIN_DLL */
#   ifndef SOWIN_NOT_DLL
#    error Define either SOWIN_DLL or SOWIN_NOT_DLL as appropriate for your linkage! See Inventor/Win/SoWinBasic.h for further instructions.
#   endif /* SOWIN_NOT_DLL */
#  endif /* !SOWIN_DLL */
# endif /* !SOWIN_MAKE_DLL */
#endif /* Microsoft Windows */

#undef SOWIN_DLL_API

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef SOWIN_DLL_API
# define SOWIN_DLL_API
#endif /* !SOWIN_DLL_API */

#ifndef GUI_TOOLKIT_VERSION
#define GUI_TOOLKIT_VERSION GetOSDisplayString()
#endif /* GUI_TOOLKIT_VERSION */

#endif // ! SOWIN_BASIC_H
