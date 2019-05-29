// src\Inventor\Win\SoWinCommon.cpp.  Generated from SoGuiCommon.cpp.in by configure.

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

/* For the Doxygen tool. */

/*! \defgroup misc Miscellaneous Classes */
/*! \defgroup devices Win Device Classes */
/*! \defgroup components Win Components */
/*! \defgroup viewers Win Viewer Components */


// FIXME: the code example shouldn't be duplicated here -- it's
// already part of the SoQt mainpage-doc. 20020806 mortene.

/*!
  \class SoWin SoWin.h Inventor/Win/SoWin.h
  \brief The SoWin class takes care of Win initialization and event dispatching.
  \ingroup misc

  This is the "application-wide" class with solely static methods
  handling initialization and event processing tasks. You must use
  this class in any application built on top of the SoWin
  library.

  Typical usage is as follows (complete application code):

  \code
  #include <Inventor/Win/SoWin.h>
  #include <Inventor/Win/viewers/SoWinExaminerViewer.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  int
  main(int argc, char ** argv)
  {
    // Initialize SoWin and Inventor API libraries. This returns a main
    // window to use.
    HWND mainwin = SoWin::init(argc, argv, argv[0]);
  
    // Make a dead simple scene graph, only containing a single
    // yellow cone under the scenegraph root.
    SoSeparator * root = new SoSeparator;
    root->ref();

    SoBaseColor * col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
  
    // Use one of the convenient viewer classes.
    SoWinExaminerViewer * eviewer = new SoWinExaminerViewer(mainwin);
    eviewer->setSceneGraph(root);
    eviewer->show();
  
    // Pop up the main window.
    SoWin::show(mainwin);
    // Loop until exit.
    SoWin::mainLoop();

    // Clean up resources.
    delete eviewer;
    root->unref();
    SoWin::done();

    return 0;
  }
  \endcode

  And voila:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/general/sogui-class-example.png">
  </center>

  \sa SoWinComponent
*/

// *************************************************************************

#include <stdlib.h>
#include <string.h>

#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoGuiP.h>
#include <Inventor/Win/SoAny.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>

#include <string>

// *************************************************************************

// Default constructor and the destructor is private (SoWin class
// consists solely of static functions).
SoWin::SoWin(void) { }
SoWin::~SoWin() { }

// *************************************************************************

cc_mutex * SoGuiP::gllockmutex = NULL;

// *************************************************************************

void
SoGuiP::commonInit(void)
{
  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  SoAny::init();

  SoGuiP::gllockmutex = cc_mutex_construct();
}

void
SoGuiP::commonCleanup(void)
{
  cc_mutex_destruct(SoGuiP::gllockmutex);
}

// *************************************************************************

/*!
  This function initializes the SoWin library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  The returned HWND is a toplevel shell widget for the
  application, which can be used as a shell for the main component.

  This method is provided for easier porting / compatibility with the
  original SGI Inventor InventorXt library. It just adds dummy \a argc
  and \a argv arguments and calls the SoWin::init() method below.
*/
HWND
SoWin::init(const char * appname, const char * classname)
{
  // If this is SoQt, Qt will keep a reference to the appname string
  // -- so make it guaranteed permanent.
  char * buf = NULL;
  if (appname != NULL) {
    size_t appname_len = strlen(appname);
    buf = (char *)new char[appname_len+1]; // FIXME: minor memleak, free on app-exit. 20020117 mortene.
    (void)strncpy(buf, appname, appname_len);
    buf[appname_len] = '\0';
  }

  // FIXME: Both SoGui_init_argc/argv need to be static to workaround
  // a compiler optimization memory corruption bug (observed with gcc
  // 4.1.1 using -O2 in combination with qt-4.2.x), which would lead
  // to a segfault. 20070528 tamer.

  // Fake argc and argv setup, forward to real init routine.
  static int SoGui_init_argc = (buf != NULL) ? 1 : 0;
  static char * SoGui_init_argv[2]; // use static array to avoid memory corruption in Qt
  SoGui_init_argv[0] = buf;
  SoGui_init_argv[1] = NULL;

  return SoWin::init(SoGui_init_argc, SoGui_init_argv, appname, classname);
}

/*!
  \fn HWND SoWin::init(int & argc, char ** argv, const char * appname, const char * classname)

  This function initializes the SoWin library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  Creates an application framework and constructs and returns a main
  widget for you.

  The returned HWND is a toplevel shell widget for the
  application, which can be used as a shell for the main component.
*/

/*!
  \fn void SoWin::init(HWND toplevelwidget)

  This function initializes the SoWin library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  Assumes you are creating your own application framework and main
  widget.  \a toplevelwidget should be your application's main widget.
*/

/*!
  \fn void SoWin::done(void)

  Cleans up all static data allocated by the SoWin library on
  initialization.

  This functions calls SoDB::finish() which means that no Coin classes
  should be used after it has been called.

  <!-- (Disabled: see FIXME in SoQt.cpp:855)
  Is called implicitly from the end of SoWin::mainLoop(), so the
  application programmer should not call it explicitly unless she
  has taken control over the native event loop herself, and is
  therefore not using SoWin::mainLoop().

  (Even then it is usually not necessary to call this method, as the
  operating system should take care of cleaning up resource use when
  an application exits. This method is mainly provided as a manner to
  be able to clean up in "unusual" run-time environments, for instance
  if SoWin is used as a component in a browser plug-in.)
  -->

  It should never be invoked more than \e once, and that
  is just before application exit, as it deallocates \e static data
  allocated as "one-off" operations in SoWin::init().
  
  NOTE: done() is not present in the original API for SGI's InventorXt
  library.
*/

/*!
  \fn void SoWin::mainLoop(void)

  This is the event dispatch loop.

  It doesn't return until application exit is somehow forced, either
  programmatically from the relevant API-call of the native toolkit,
  or when the user closes the application's main widget.
  
  After the main loop has finished execution, call SoWin::done() to clean 
  up static data.

  <!-- (Disabled: see FIXME in SoQt.cpp:855)
  An important note: be careful about how you handle
  SoWinComponent-derived objects after the application control returns
  from mainLoop(), as SoWin will then have been "cleaned up" with
  regards to various internal resources. So doing for instance
  something like this:

  \code
  SoWin::mainLoop();
  viewer->hide();
  \endcode

  ..spells "undefined behavior, expect a crash".

  \e Deleting a component after mainLoop() returns is allowed, though,
  and also necessary to avoid getting reports of possible memory leaks
  from memleak checkers.
  -->
*/

/*!
  \fn void SoWin::exitMainLoop(void)

  This function will make the main event loop finish looping.

  NOTE: exitMainLoop() is not present in the original API for SGI's
  InventorXt library.
*/

/*!
  \fn HWND SoWin::getTopLevelWidget(void)

  Returns the HWND which is the main widget for the
  application. When this widget gets closed, SoWin::mainLoop() will
  return (unless the close event is caught by the user).

  \sa getShellWidget()
*/

// *************************************************************************

/*!
  This method will fill in the integers pointed to by the arguments
  with the corresponding part of the version release number of the
  SoWin library.

  A \c NULL pointer will make that part of the version number be ignored.

  This method is not part of the original InventorXt API from SGI.
*/
void
SoWin::getVersionInfo(int * major, int * minor, int * micro)
{
  if (major) { *major = SOWIN_MAJOR_VERSION; }
  if (minor) { *minor = SOWIN_MINOR_VERSION; }
  if (micro) { *micro = SOWIN_MICRO_VERSION; }
}

/*!
  This method returns a string containing the version id of the
  library.

  This method is not part of the original InventorXt API from SGI.
*/
const char *
SoWin::getVersionString(void)
{
  static const char version[] = SOWIN_VERSION;
  return version;
}

/*!
  This method returns a string containing the version id of the
  Win toolkit the SoWin library is linked against.

  This method is not part of the original InventorXt API from SGI.
*/
#ifdef SOWIN_INTERNAL
namespace Win32 {
  std::string GetOSDisplayString();
};
#endif

const char *
SoWin::getVersionToolkitString(void)
{
#ifdef SOWIN_INTERNAL
  static std::string toolkit_version = Win32::GetOSDisplayString();
#else
  static std::string toolkit_version = GUI_TOOLKIT_VERSION;
#endif
  return toolkit_version.c_str();
}

// *************************************************************************

/*!
  This method locks other threads out from a code section until the
  caller thread invokes SoWin::unlockGL().

  It is meant to protect several threads invoking OpenGL calls in
  parallell, in case the underlying OpenGL implementation is not
  multi-thread safe.

  For convenience, the function can be invoked through the
  SOWIN_ENTER_GL_SECTION().

  This method is not part of the original InventorXt API from SGI.
*/
void
SoWin::lockGL(void)
{
  assert(SoGuiP::gllockmutex);

  cc_mutex_lock(SoGuiP::gllockmutex);
}

/*!
  See documentation for SoWin::lockGL().

  For convenience, the function can be invoked through the
  SOWIN_LEAVE_GL_SECTION().

  This method is not part of the original InventorXt API from SGI.
*/
void
SoWin::unlockGL(void)
{
  cc_mutex_unlock(SoGuiP::gllockmutex);
}

// *************************************************************************

/*!
  \typedef void SoWin::FatalErrorCB(const SbString errmsg, SoWin::FatalErrors errcode, void * userdata)

  An application function callback for handling fatal errors should be
  of this type.

  The first argument is an error message in English describing the
  details of the error. The second argument is an error code used so
  the application can identify specific conditions. The third argument
  is the userdata pointer passed in to SoWin::setFatalErrorHandler().
*/

/*!
  \enum SoWin::FatalErrors
  Numerical identifiers for classifying the different kinds of possible
  fatal errors.
*/

/*!
  \var SoWin::FatalErrors SoWin::NO_OPENGL_CANVAS

  Could not construct \e any valid OpenGL canvas. Something is very
  wrong on the client system.
*/
/*!
  \var SoWin::FatalErrors SoWin::INTERNAL_ASSERT

  An internal error condition that should never happen was
  detected. The most likely cause of this is programmering errors
  within the SoWin library itself.
*/
/*!
  \var SoWin::FatalErrors SoWin::UNSPECIFIED_ERROR

  Signifies that we were not able to specify in any greater detail the
  error condition that came up.
*/

/*!
  Set up a callback to invoke in the case of unexpected fatal error
  conditions within the SoWin library.

  Almost any error condition within the library is handled in a robust
  way through return values indicating errors for the offending calls,
  but there are a few cases that are impossible to handle without
  seriously crippling the functionality.

  (One example is if we fail to find \e any way of making a valid
  OpenGL canvas. This is an indication that something is seriously
  wrong on the end-user's system, and the SoWin library will not work
  properly.)

  In the case of a fatal error, it is expected that the given
  application callback function communicates the problem to the
  end-user and then either exits the application or at least refrains
  from using any part of the SoWin library.

  If no callback is explicitly set up by the application, the SoWin
  library will display an error message to the end-user and then exit
  the application.

  When setting a callback, this method returns a pointer to the
  previous callback function, or \c NULL if none.

  (This is an extension versus the original SGI InventorXt library
  API.)


  On a related note, be aware that the end-user will still be notified
  about non-fatal errors and warning messages through a dialog box. If
  you want to handle these yourself, or if you don't want your
  end-user to see any of these non-critical messages, you will also
  need to install an error handler callback function on the Coin
  library itself:

  \code
    SoDebugError::setHandlerCallback(myErrorHandlerCB, myCBUserData);
  \endcode

  (Please also see the documentation of
  SoDebugError::setHandlerCallback().)

 */
SoWin::FatalErrorCB *
SoWin::setFatalErrorHandler(SoWin::FatalErrorCB * cb, void * userdata)
{
  return SoAny::si()->setFatalErrorHandler(cb, userdata);
}

// *************************************************************************

/*!
  Returns \c TRUE if this binary version of the SoWin library was
  compiled with debug settings on.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoWin::isDebugLibrary(void)
{
  return SOWIN_DEBUG != 0 ? TRUE : FALSE;
}

/*!
  Returns \c TRUE if this version of the SoWin library has an
  Application Binary Interface compatible with the given version.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoWin::isCompatible(unsigned int major, unsigned int minor)
{
  if (major != SOWIN_MAJOR_VERSION) { return FALSE; }
  if (minor > SOWIN_MINOR_VERSION) { return FALSE; }
  return TRUE;
}

/*!
  \enum SoWin::ABIType
  Numerical identifiers to identify how the library was built.
*/
/*!
  \var SoWin::ABIType SoWin::DLL
  The SoWin library was built as a dynamic link library (aka "shared
  library").
*/
/*!
  \var SoWin::ABIType SoWin::LIB
  The SoWin library was built as a static library.
*/
/*!
  \var SoWin::ABIType SoWin::UNKNOWN
  The SoWin introspection mechanisms can not decide how the library
  was built.
*/


/*!
  Returns an indication on how the library was compiled: as a dynamic
  library, or as a static library.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SoWin::ABIType
SoWin::getABIType(void)
{
  // FIXME: replace this ugly shit with a configure-check to test if
  // we're on a DLL-capable platform. 20020118 mortene.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef SOWIN_MAKE_DLL
  return SoWin::DLL;
#else
  return SoWin::LIB;
#endif
#endif
  // FIXME: this is rather lame, we have enough information in the
  // configure / build process to always know whether we're building
  // static or dynamic. 20020618 mortene.
  return SoWin::UNKNOWN;
}

#if 0 // FIXME: not in use, see larsa's FIXME below. 20020617 mortene.

// *************************************************************************
// These sanity checks are designed to detect common pitfall errors for
// Microsoft Windows linkage with SoWin and Coin.

// FIXME: use an "is-this-a-DLL-capable-platform" configure check
// and remove the system "#if defined" tests below. 20011203 mortene.
// FIXME: I disabled this because it wasn't inlined in the client app
// but compiled into the library by MSVC++ and with SOWIN_DLL undefined,
// the ABI test always tried the LIB_ABI path.  20020126 larsa
#if 0 && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define SOWIN_SANITY_CHECK(forward_call)                  \
  if (SoWin::isDebugLibrary() &&                       \
       SoDB::isInitialized())                          \
    SoDebugError::postWarning("SoWin::init",            \
      "unable to verify application linkage sanity "    \
      "because Open Inventor has already been "         \
      "initialized before SoWin::init call");           \
  SOWIN_RUNTIME_SANITY_CHECK();                         \
  SOWIN_LINKSTYLE_SANITY_CHECK();                       \
  forward_call;                                         \
  SOWIN_LINKTIME_SANITY_CHECK()

#define SOWIN_RUNTIME_SANITY_CHECK()                    \
  do {                                                  \
    if (!SoWin::isCompatible(SOWIN_MAJOR_VERSION,      \
                            SOWIN_MINOR_VERSION))      \
      SoWin::abort(SOWIN_RUNTIME_MISMATCH);             \
  } while (FALSE)

#ifdef SOWIN_DLL
#define SOWIN_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoWin::getABIType() != SOWIN_DLL_ABI)         \
      SoWin::abort(SOWIN_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#else
#define SOWIN_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoWin::getABIType() != SOWIN_LIB_ABI)         \
      SoWin::abort(SOWIN_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#endif

#define SOWIN_LINKTIME_SANITY_CHECK()                   \
  do {                                                  \
    if (!SoDB::isInitialized())                       \
      SoWin::abort(SOWIN_LINKTIME_MISMATCH);            \
  } while (FALSE)
#else /* ! MS Windows */
#define SOWIN_SANITY_CHECK(forward_call)                \
  forward_call
#endif /* ! MS Windows */

#endif // OBSOLETED

#ifndef DOXYGEN_SKIP_THIS

// Abort the application due to some kind of mismatch in the ABI
// settings / configuration. This should hopefully help application
// programmers avoid shooting themselves in the foot by controlling
// certain run-time parameters in the client application versus what
// is expected by the SoWin library.
//
// If you're an application programmer, it is very likely that you
// only need to consider this an internal library method.
//
// This method was not part of the original SGI InventorXt library,
// but is an extension specific to the Coin project.
void
SoGuiP::abort(SoGuiP::ABIError error)
{
  switch (error) {
  case SoGuiP::LINKTIME_MISMATCH:
    SoWin::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linktime mismatch error.");
    break;

  case SoGuiP::LINKSTYLE_MISMATCH:
    SoWin::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linkstyle mismatch error (DLL vs. LIB).");
    break;

  case SoGuiP::RUNTIME_MISMATCH:
    SoWin::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected runtime mismatch error (versioning and ABI compatibility).");
    break;

  default:
    // FIXME: shouldn't this rather be an assert? 20020118 mortene.
    SoWin::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Unknown error in SoWin :(");
    break;
  }

  // FIXME: call fatal error handler in SoAny. 20020118 mortene.
  exit(-1);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  \fn void SoWin::createSimpleErrorDialog(HWND widget, const char * title, const char * string1, const char * string2)


  This is a convenient way for the application programmer to throw up
  an obtrusive application-global error dialog.

  If \a widget is \c NULL, the dialog will be modal for the whole
  application (all windows will be blocked for input). If not,
  only the window for the given \a widget will be blocked.

  \a title is the title of the dialog box. \a string1 and \a string2
  contains the text which will be shown in the dialog box.

  There will only be a single "Ok" button for the user to press and
  continue with the application.
*/
