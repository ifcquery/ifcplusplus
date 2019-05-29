#ifndef SOWIN_COMPONENT_H
#define SOWIN_COMPONENT_H

// src\Inventor\Win\SoWinComponent.h.  Generated from SoGuiComponent.h.in by configure.

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

#include <Inventor/SbLinear.h>
#include <Inventor/Win/SoWinObject.h>

#ifdef __COIN_SOQT__
class QWidget;
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
#include <X11/Intrinsic.h>
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
#include <gtk/gtk.h>
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
#include <windows.h>
#endif // __COIN_SOWIN__


class SoWinComponent;
class SoWinCursor;

typedef void SoWinComponentCB(void * user, SoWinComponent * component);
typedef void SoWinComponentVisibilityCB(void * user, SbBool visible);

// *************************************************************************

class SOWIN_DLL_API SoWinComponent : public SoWinObject {
  SOWIN_OBJECT_ABSTRACT_HEADER(SoWinComponent, SoWinObject);

public:
  virtual ~SoWinComponent();

  virtual void show(void);
  virtual void hide(void);

  virtual void setComponentCursor(const SoWinCursor & cursor);
  static void setWidgetCursor(HWND w, const SoWinCursor & cursor);

  SbBool isFullScreen(void) const;
  SbBool setFullScreen(const SbBool onoff);

  SbBool isVisible(void);
  SbBool isTopLevelShell(void) const;

  HWND getWidget(void) const;
  HWND getBaseWidget(void) const;
  HWND getShellWidget(void) const;
  HWND getParentWidget(void) const;

  void setSize(const SbVec2s size);
  SbVec2s getSize(void) const;

  void setTitle(const char * const title);
  const char * getTitle(void) const;
  void setIconTitle(const char * const title);
  const char * getIconTitle(void) const;

  const char * getWidgetName(void) const;
  const char * getClassName(void) const;

  void setWindowCloseCallback(SoWinComponentCB * const func,
                              void * const user = NULL);
  static SoWinComponent * getComponent(HWND widget);

  static void initClasses(void);

protected:
  SoWinComponent(HWND const parent = NULL,
                   const char * const name = NULL,
                   const SbBool embed = TRUE);

  virtual void afterRealizeHook(void);

  // About the wrapping below: this variable was added after SoQt 1.0,
  // and before SoXt 1.1. To be able to release SoQt 1.1 from this
  // same branch, sizeof(SoQtComponent) needs to be the same as for
  // SoQt 1.0, which means we can't add this variable for SoQt.
#ifndef __COIN_SOQT__
  SbBool firstRealize;
#endif // __COIN_SOQT__

  void setClassName(const char * const name);
  void setBaseWidget(HWND widget);

  void registerWidget(HWND widget);
  void unregisterWidget(HWND widget);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void sizeChanged(const SbVec2s & size);

  void addVisibilityChangeCallback(SoWinComponentVisibilityCB * const func,
                                   void * const user = NULL);
  void removeVisibilityChangeCallback(SoWinComponentVisibilityCB * const func,
                                      void * const user = NULL);

private:
  class SoWinComponentP * pimpl;
  friend class SoGuiComponentP;
  friend class SoWinComponentP;

  // FIXME!: audit and remove as much as possible of the remaining
  // toolkit specific parts below. 20020117 mortene.

#ifdef __COIN_SOXT__
public:
  Display * getDisplay(void);
  void fitSize(const SbVec2s size);
  // FIXME: I guess these should really be part of the common
  // API. 20011012 mortene.
  void addWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);
  void removeWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);

protected:
  // FIXME: I guess this should perhaps be part of the common API?
  // 20011012 mortene.
  void invokeVisibilityChangeCallbacks(const SbBool enable) const;
  void invokeWindowCloseCallbacks(void) const;
  virtual void windowCloseAction(void);

private:
  // FIXME: get rid of this? 20011012 mortene.
  static void event_handler(Widget, XtPointer, XEvent *, Boolean *);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
protected:
  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
private:
  static gint eventHandler(GtkWidget * object, GdkEvent * event, gpointer closure);
#endif // __COIN_SOGTK__
};

// *************************************************************************

#endif // ! SOWIN_COMPONENT_H
