#ifndef SOWIN_H
#define SOWIN_H

// src\Inventor\Win\SoWin.h.  Generated from SoGui.h.in by configure.

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

#include <Inventor/Win/SoWinBasic.h>

// FIXME: use configure defines for the header files.
// 20020613 mortene.

#ifdef __COIN_SOQT__

#if QT_VERSION >= 0x040000
#include <QtCore/QObject>
#else
#include <qobject.h>
#endif

#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
// Fetch stdlib.h, so NULL is defined before glib.h is (indirectly)
// included. Otherwise we get a compile error with KCC on some
// systems.
#include <stdlib.h>
#include <gtk/gtk.h>
class SoGtkComponent;
class SbPList;
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
#include <windows.h>
#endif // __COIN_SOWIN__

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbString.h>
#include <Inventor/SoDB.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

class SOWIN_DLL_API SoWin
{

public:
  static HWND init(const char * appname, const char * classname = "SoWin");
  static HWND init(int & argc, char ** argv,
                       const char * appname, const char * classname = "SoWin");
  static void init(HWND toplevelwidget);

  static void mainLoop(void);
  static void exitMainLoop(void);
  static void done(void);

  static HWND getTopLevelWidget(void);
  static HWND getShellWidget(const HWND w);

  static void show(HWND const widget);
  static void hide(HWND const widget);

  static void setWidgetSize(HWND const widget, const SbVec2s size);
  static SbVec2s getWidgetSize(const HWND widget);

  static void createSimpleErrorDialog(HWND widget,
                                      const char * title,
                                      const char * string1,
                                      const char * string2 = NULL);

  static void getVersionInfo(int * major = NULL,
                             int * minor = NULL,
                             int * micro = NULL);
  static const char * getVersionString(void);
  static const char * getVersionToolkitString(void);

  enum FatalErrors {
    UNSPECIFIED_ERROR = 0,
    NO_OPENGL_CANVAS,
    INTERNAL_ASSERT
  };
  typedef void FatalErrorCB(const SbString errmsg, SoWin::FatalErrors errcode,
                            void * userdata);
  static FatalErrorCB * setFatalErrorHandler(SoWin::FatalErrorCB * cb,
                                             void * userdata);

  static SbBool isDebugLibrary(void);
  static SbBool isCompatible(unsigned int major, unsigned int minor);

  enum ABIType { DLL, LIB, UNKNOWN };
  static ABIType getABIType(void);

  static void lockGL(void);
  static void unlockGL(void);

private:
  // Since the class consists solely of static functions, hide the
  // default constructor and the destructor so nobody can instantiate
  // it.
  SoWin(void);
  virtual ~SoWin();

  friend class SoGuiP;
  friend class SoWinP;


  // FIXME!: audit and remove as much as possible of the remaining
  // toolkit specific parts below. 20020117 mortene.

#ifdef __COIN_SOWIN__
public:
  static void doIdleTasks(void);
#endif // __COIN_SOWIN__

#ifdef __COIN_SOXT__
public:
  static void nextEvent(XtAppContext, XEvent *);
  static Boolean dispatchEvent(XEvent * event);
  static XtAppContext getAppContext(void);
  static Display * getDisplay(void);
  static XmString encodeString(const char * const str);
  static char * decodeString(XmString xstring);
  static void getPopupArgs(Display * display, int screen,
                           ArgList args, int * n);

  static void registerColormapLoad(Widget widget, Widget shell);
  static void addColormapToShell(Widget widget, Widget shell);
  static void removeColormapFromShell(Widget widget, Widget shell);

  static void addExtensionEventHandler(Widget widget,
                                       int eventType, XtEventHandler proc,
                                       XtPointer clientData);
  static void removeExtensionEventHandler(Widget widget,
                                          int eventType, XtEventHandler proc,
                                          XtPointer clientData);

protected:
  static void getExtensionEventHandler(XEvent * event, Widget & widget,
                                       XtEventHandler & proc,
                                       XtPointer & clientData);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
public:
  friend class SoGtkComponent;
  enum SoGtkComponentAction { CREATION, DESTRUCTION, CHANGE };
  typedef void SoGtkComponentActionCallback(SoGtkComponent *, SoGtk::SoGtkComponentAction, void *);

  static void addComponentActionCallback(SoGtkComponentActionCallback *, void *);
  static void removeComponentActionCallback(SoGtkComponentActionCallback *, void *);

  static int getComponents(SbPList & components);

protected:
  static void invokeComponentActionCallbacks(SoGtkComponent * component,
                                             SoGtkComponentAction action);

  static gint componentCreation(SoGtkComponent * component);
  static gint componentDestruction(SoGtkComponent * component);
  static gint componentChange(SoGtkComponent * component);

private:
  static gint timerSensorCB(gpointer data);
  static gint idleSensorCB(gpointer data);
  static gint delaySensorCB(gpointer data);

  static GtkWidget * mainWidget;
  static SbPList * components;
  static SbPList * component_callbacks;
#endif // __COIN_SOGTK__
};

// *************************************************************************
// Here's something InventorWin provides for convenience. Belongs more
// in the application code domain, imo, but we provide these macros
// for better compile time compatibility with InventorWin.  -mortene

#define SOWIN_ENTER_GL_SECTION() do { SoWin::lockGL(); } while(FALSE)
#define SOWIN_LEAVE_GL_SECTION() do { SoWin::unlockGL(); } while(FALSE)

// *************************************************************************

#endif // ! SOWIN_H
