#ifndef SOWINPLANEVIEWERP_H
#define SOWINPLANEVIEWERP_H

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

#include <Inventor/Win/viewers/SoGuiPlaneViewerP.h>

class SoWinPlaneViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoWinPlaneViewer class.

class SoWinPlaneViewerP : public SoGuiPlaneViewerP
{
public:
  SoWinPlaneViewerP(SoWinPlaneViewer * publ);
  ~SoWinPlaneViewerP();

  void constructor(SbBool build);
 
  static void xButtonProc(class SoWinBitmapButton * b, void * userdata);
  static void yButtonProc(class SoWinBitmapButton * b, void * userdata);
  static void zButtonProc(class SoWinBitmapButton * b, void * userdata);
  static void cameraButtonProc(class SoWinBitmapButton * b, void * userdata);

  class SoWinBitmapButton * camerabutton;
};

// ************************************************************************

#endif // ! SOWINPLANEVIEWERP_H
