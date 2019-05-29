#ifndef SOWIN_CURSOR_H
#define SOWIN_CURSOR_H

// src\Inventor\Win\SoWinCursor.h.  Generated from SoGuiCursor.h.in by configure.

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
#include <Inventor/Win/SoWinBasic.h>

class SOWIN_DLL_API SoWinCursor {
public:
  static void initClass(void);

  struct CustomCursor {
    SbVec2s dim;
    SbVec2s hotspot;
    unsigned char * bitmap;
    unsigned char * mask;
  };


  // FIXME: add more default shapes. 20011119 pederb.
  enum Shape {
    CUSTOM_BITMAP = -1,
    DEFAULT = 0,
    BUSY,
    CROSSHAIR,
    UPARROW
  };
  
  SoWinCursor(void);
  SoWinCursor(const Shape shape);
  SoWinCursor(const CustomCursor * cc);
  SoWinCursor(const SoWinCursor & cursor);
  ~SoWinCursor();

  SoWinCursor & operator=(const SoWinCursor & c);

  Shape getShape(void) const;
  void setShape(const Shape shape);

  const CustomCursor & getCustomCursor(void) const;

  static const SoWinCursor & getZoomCursor(void);
  static const SoWinCursor & getPanCursor(void);
  static const SoWinCursor & getRotateCursor(void);
  static const SoWinCursor & getBlankCursor(void);
  
private:
  void commonConstructor(const Shape shape, const CustomCursor * cc);

  Shape shape;
  CustomCursor * cc;
};

#endif // ! SOWIN_CURSOR_H
