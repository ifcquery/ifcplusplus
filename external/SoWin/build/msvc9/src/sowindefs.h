#ifndef SOWIN_INTERNALDEFS_H
#define SOWIN_INTERNALDEFS_H

// src/sowindefs.h.  Generated from soguidefs.h.in by configure.

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

/* ********************************************************************** */

#ifndef SOWIN_INTERNAL
#error this is a private header file
#endif /* !SOWIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* ********************************************************************** */

#ifdef __FILE__
#define SOWIN_STUB_FILE __FILE__
#else
/* FIXME: avoid this case by redefining the compilation rules to define the
   current filename in some variable.  [20010919 larsa] */
#define SOWIN_STUB_FILE ((char *)0L)
#endif

#ifdef __LINE__
#define SOWIN_STUB_LINE __LINE__
#else
#define SOWIN_STUB_LINE 0
#endif

#ifdef __cplusplus
#ifdef HAVE_CPP_COMPILER_FUNCTION_NAME_VAR
#define SOWIN_STUB_FUNC HAVE_CPP_COMPILER_FUNCTION_NAME_VAR
#else
#define SOWIN_STUB_FUNC ((const char *)0L)
#endif
#else /* !__cplusplus */
#ifdef HAVE_C_COMPILER_FUNCTION_NAME_VAR
#define SOWIN_STUB_FUNC HAVE_C_COMPILER_FUNCTION_NAME_VAR
#else
#define SOWIN_STUB_FUNC ((const char *)0L)
#endif
#endif /* !__cplusplus */

#if SOWIN_DEBUG

#include <Inventor/errors/SoDebugError.h>

/*
  This stupid thing is here to silence some compilers that complain on
  constant if-expressions.
*/

inline int sowin_not_null(const void * arg) { return (arg != NULL) ? 1 : 0; }


/* Collects common code from SOWIN_STUB(), SOWIN_STUB()_ONCE and
   SOWIN_OBSOLETED(). */

#define SOWIN_FUNCTIONIDENTIFIER(SOWINSTR) \
    SbString SOWINSTR(sowin_not_null(SOWIN_STUB_FILE) ? SOWIN_STUB_FILE : ""); \
    if (SOWINSTR.getLength() > 0) { SOWINSTR += ':'; } \
    SOWINSTR += SbString((int)SOWIN_STUB_LINE); SOWINSTR += ':'; \
    SOWINSTR += sowin_not_null(SOWIN_STUB_FUNC) ? SOWIN_STUB_FUNC : ""

/*
  SOWIN_STUB(): a macro that prints out stub information.
  Used where there is functionality missing.
*/

#define SOWIN_STUB() \
  do { \
    SOWIN_FUNCTIONIDENTIFIER(s); \
    SoDebugError::postWarning(s.getString(), \
                              "STUB: functionality not yet completed"); \
  } while (0)

/*
  SOWIN_STUB_ONCE(): a macro that prints out stub information, but only on
  the first pass.  Used where there is functionality missing that may be
  called hundreds of times.
*/

#define SOWIN_STUB_ONCE() \
  do { \
    static int first = 1; \
    if (first) { \
      SOWIN_FUNCTIONIDENTIFIER(s); \
      SoDebugError::postWarning(s.getString(), \
                                "STUB: functionality not yet completed " \
                                "(first and last warning)"); \
      first = 0; \
    } \
  } while (0)

/*
  SOWIN_OBSOLETED(): a macro that prints out information about an obsoleted
  function still being in use.
*/

#define SOWIN_OBSOLETED() \
  do { \
    SOWIN_FUNCTIONIDENTIFIER(s); \
    SoDebugError::post(s.getString(), \
                       "OBSOLETED: functionality no longer supported"); \
  } while (0)

#else /* !SOWIN_DEBUG */

/* snub the stubs */
#define SOWIN_STUB()      do { } while (0)
#define SOWIN_STUB_ONCE() do { } while (0)
#define SOWIN_OBSOLETED() do { } while (0)

#endif /* !SOWIN_DEBUG */

/*
  GETTEXT support
*/

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
#include <libintl.h>
#define _(String) dgettext (PACKAGE, String)
#define N_(String) (String)
#else
#define _(String) (String)
#define N_(String) (String)
#endif /* HAVE_GETTEXT && ENABLE_NLS */

/* ********************************************************************** */

#endif /* ! SOWIN_INTERNALDEFS_H */
