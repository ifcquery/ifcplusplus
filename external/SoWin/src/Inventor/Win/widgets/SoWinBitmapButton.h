#ifndef SOWIN_BITMAPBUTTON_H
#define SOWIN_BITMAPBUTTON_H

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

#include <windows.h>

#include <Inventor/SbBasic.h>
#include <Inventor/Win/SoWinBasic.h>

// *************************************************************************

class SOWIN_DLL_API SoWinBitmapButton {

public:
  SoWinBitmapButton(HWND parent,
                    int x,
                    int y,
                    int width = 30,
                    int height = 30,
                    int depth = 0,
                    const char * name = NULL,
                    void * bits = NULL);
  SoWinBitmapButton(HWND parent,
                    int depth = 0,
                    const char * name = NULL,
                    void * bits = NULL);
  ~SoWinBitmapButton();

  void setEnabled(SbBool enable);
  SbBool isEnabled(void) const;

  void setPressedState(SbBool enable);
  SbBool getPressedState(void) const;

  void addBitmap(HBITMAP hbmp);
  void addBitmap(int width, int height, int bpp, void * src);
  void addBitmap(const char ** xpm, int bpp = 24);
  HBITMAP getBitmap(int index) const;
  void setBitmap(int index);

  HWND getWidget(void);

  int width(void) const;
  int height(void) const;
  void move(int x, int y);
  void move(int x, int y, int width, int height);
  void resize(int width, int height);

  void show(void);
  void hide(void);

  typedef void ClickedProc(SoWinBitmapButton * button, void * userdata);
  void registerClickedProc(ClickedProc * func, void * userdata);

private:
  class SoWinBitmapButtonP * pimpl;
  friend class SoWinBitmapButtonP;
};

// *************************************************************************

#endif // ! SOWIN_BITMAPBUTTON_H
