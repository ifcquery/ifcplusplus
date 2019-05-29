#ifndef SOWINCOMPONENTP_H
#define SOWINCOMPONENTP_H

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

#ifndef SOWIN_INTERNAL
#error this is a private header file
#endif /* !SOWIN_INTERNAL */

#include <Inventor/Win/SoGuiComponentP.h>
#include <Inventor/Win/SoWinCursor.h>

class SbStorage;

// ************************************************************************

// "Hidden" class for storing private data (to avoid cluttering up the
// header file, and to make it easier to keep ABI compatibility upon
// changes.)

class SoWinComponentP : public SoGuiComponentP
{
public:
  SoWinComponentP(SoWinComponent * publ);
  ~SoWinComponentP();


  static HCURSOR getNativeCursor(const SoWinCursor::CustomCursor * cc);

  static void fatalerrorHandler(void * userdata);
  void cleanupWin32References(void);

  static LRESULT CALLBACK frameWindowHandler(HWND window, UINT message,
                                             WPARAM wparam, LPARAM lparam);

  void commonEventHandler(UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT CALLBACK systemEventFilter(int code, WPARAM wparam, LPARAM lparam);

  HWND buildFormWidget(HWND parent);

  static SbStorage * hookhandle; // for (global) system message queue interception
  HWND parent;
  HWND widget;
  HWND focusProxy;
  SbBool embedded;
  SbString classname, widgetname, title;
  SoWinComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilitychangeCBs;
  static SbDict * embeddedparents;

  // This is the atom returned when the component
  // window class is registered.
  static ATOM wndClassAtom;

  struct FullscreenData {
    FullscreenData(void)
      : on(FALSE)
    { }

    SbBool on;
    SbVec2s pos;
    SbVec2s size;
    LONG style;
    LONG exstyle;
  };

  struct FullscreenData fullscreendata;

  SbBool hascursor;
  SoWinCursor currcursor;
  SbBool realized;

private:
  static SbDict * cursordict;
};

// ************************************************************************

#endif // !SOWINCOMPONENTP_H
