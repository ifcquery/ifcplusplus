#ifndef SOWIN_THUMBWHEEL_H
#define SOWIN_THUMBWHEEL_H

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

#include <windows.h>

#include <Inventor/SbBasic.h>
#include <Inventor/Win/SoWinBasic.h>

class SoAnyThumbWheel;
class SbDict;

// *************************************************************************

class SoWinThumbWheel {

public:
  enum Interaction { START, MOVE, END };
  typedef void ThumbWheelCB(Interaction type, float val, void * userdata);

  enum Orientation {
    Horizontal,
    Vertical
  };
  enum boundaryHandling {
    CLAMP,
    MODULATE,
    ACCUMULATE
  };
  enum State {
    Idle,
    Dragging,
    Disabled
  };

  SoWinThumbWheel(HWND parent = 0, long id = -1, int x = 0, int y = 0, const char * name = 0);
  SoWinThumbWheel(Orientation, HWND parent = 0, long id = -1, int x = 0, int y = 0, const char * name = 0);
  ~SoWinThumbWheel(void);

  void setId(long id);
  long id(void) const;
  
  void setOrientation(Orientation);
  Orientation orientation(void) const;

  void setValue(float value);
  float value(void) const;

  void setEnabled(SbBool enable);
  SbBool isEnabled(void) const;

  void setLabelText(const char * text);
  void setLabelOffset(int x, int y);
  SIZE getLabelSize(void);

  void setRangeBoundaryHandling(boundaryHandling handling);
  boundaryHandling getRangeBoundaryHandling(void) const;

  HWND getWidget(void);
  SIZE sizeHint(void) const;
  int width(void);
  int height(void);
  void move(int x, int y);
  void size(int width, int height);
  void move(int x, int y, int width, int height);
  void setCallback(ThumbWheelCB * func, void * userdata);

  void show(void);
  void hide(void);

  static SoWinThumbWheel * getWheelFromHWND(HWND h);

protected:

  LRESULT CALLBACK onCreate(HWND window,
                            UINT message,
                            WPARAM wparam,
                            LPARAM lparam);

  LRESULT CALLBACK onSize(HWND window,
                          UINT message,
                          WPARAM wparam,
                          LPARAM lparam);

  LRESULT CALLBACK onPaint(HWND window,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam);

  LRESULT CALLBACK onLButtonDown(HWND window,
                                 UINT message,
                                 WPARAM wparam,
                                 LPARAM lparam);

  LRESULT CALLBACK onLButtonUp(HWND window,
                               UINT message,
                               WPARAM wparam,
                               LPARAM lparam);

  LRESULT CALLBACK onMouseMove(HWND window,
                               UINT message,
                               WPARAM wparam,
                               LPARAM lparam);

  LRESULT CALLBACK onDestroy(HWND window,
                             UINT message,
                             WPARAM wparam,
                             LPARAM lparam);

  static LRESULT CALLBACK windowProc(HWND window,
                                     UINT message,
                                     WPARAM wparam,
                                     LPARAM lparam);

private:
  void constructor(Orientation, HWND parent, long id, int x, int y,
                   const char * name);

  HWND buildWidget(HWND parent, RECT rect, const char * name);
  void initWheel(int diameter, int width);
  HWND createLabel(HWND parent, int x, int y, const char * text);
  HBITMAP createDIB(int width, int height, int bpp, void ** bits);
  void blitBitmap(HBITMAP bitmap, HDC dc, int x,int y, int width, int height) const;
  SIZE getTextSize(HWND window, const char * text);

  Orientation orient;
  State state;
  float wheelValue, tempWheelValue;
  int mouseDownPos, mouseLastPos;

  SoAnyThumbWheel * wheel;
  HBITMAP * pixmaps;
  int numPixmaps;
  int currentPixmap;

  HWND wheelWindow;
  HWND labelWindow;

  POINT labelOffset;

  ThumbWheelCB * viewerCB;
  void * userdataCB;

  static ATOM wheelWndClassAtom;
  static int wheelWidgetCounter;
  static SbDict * hwnddict;
};

// *************************************************************************

#endif // ! SOWIN_THUMBWHEEL_H
