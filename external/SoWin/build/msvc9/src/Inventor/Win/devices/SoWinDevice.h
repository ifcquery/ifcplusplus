#ifndef SOWIN_DEVICE_H
#define SOWIN_DEVICE_H

// src\Inventor\Win\devices\SoWinDevice.h.  Generated from SoGuiDevice.h.in by configure.

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
//
// Toolkit-specific typedef and include(s). Put these before any Coin
// and/or SoWin includes, in case there are any dependency bugs in
// the underlying native toolkit set of include files versus the
// compiler environment's include files.

#include <Inventor/Win/SoWinBasic.h> // Contains __COIN_SOWIN__ define.

#ifdef __COIN_SOQT__
#include <qevent.h>
typedef void SoWinEventHandler(HWND, void *, MSG *, bool *);
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
#include <X11/Intrinsic.h>
typedef void SoWinEventHandler(HWND, XtPointer, XEvent *, Boolean *);
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
#include <gtk/gtk.h>
typedef gint SoWinEventHandler(HWND, MSG *, gpointer);
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
#include <windows.h>
typedef LRESULT SoWinEventHandler(HWND, UINT, WPARAM, LPARAM);
#endif // __COIN_SOWIN__

// *************************************************************************

#include <Inventor/SbLinear.h>
#include <Inventor/Win/SoWinObject.h>

class SoEvent;

// *************************************************************************

class SOWIN_DLL_API SoWinDevice : public SoWinObject {
  SOWIN_OBJECT_ABSTRACT_HEADER(SoWinDevice, SoWinObject);

public:
  virtual ~SoWinDevice();

  virtual void enable(HWND w, SoWinEventHandler * handler, void * closure) = 0;
  virtual void disable(HWND w, SoWinEventHandler * handler, void * closure) = 0;

  virtual const SoEvent * translateEvent(MSG * event) = 0;

  void setWindowSize(const SbVec2s size);
  SbVec2s getWindowSize(void) const;

  static void initClasses(void);

protected:
  SoWinDevice(void);

  void setEventPosition(SoEvent * event, int x, int y) const;
  static SbVec2s getLastEventPosition(void);

  void addEventHandler(HWND, SoWinEventHandler *, void *);
  void removeEventHandler(HWND, SoWinEventHandler *, void *);
  void invokeHandlers(MSG * event);

private:
  class SoWinDeviceP * pimpl;
  friend class SoWinDeviceP;
};

// *************************************************************************

#endif // !SOWIN_DEVICE_H
