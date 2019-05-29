#ifndef SOWINGLWIDGETP_H
#define SOWINGLWIDGETP_H

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

#include <Inventor/Win/SoGuiGLWidgetP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoWinGLWidget class.

class SoWinGLWidgetP : public SoGuiGLWidgetP
{
public:
  SoWinGLWidgetP(SoWinGLWidget * publ);
  ~SoWinGLWidgetP();

  static SbBool debugGLContextCreation(void);
  static void dumpPixelFormat(HDC hdc, int format);
  static double weighPixelFormat(const PIXELFORMATDESCRIPTOR * pfd,
                                 SbBool want_rgb = TRUE,
                                 SbBool want_dblbuffer = TRUE,
                                 SbBool want_zbuffer = TRUE,
                                 SbBool want_stereo = FALSE,
                                 SbBool want_accum = FALSE,
                                 SbBool want_stencil = FALSE,
                                 SbBool want_overlay = FALSE,
				 SbBool want_alphachannel = FALSE);

  void buildNormalGLWidget(HWND manager);
  void buildOverlayGLWidget(HWND manager);
  void createGLContext(HWND window);

  static LRESULT eventHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

  LRESULT onCreate(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT onPaint(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  void destroyWindow(HWND window);

  static BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);

  static LRESULT CALLBACK glWidgetProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT CALLBACK mgrWidgetProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

  HWND managerWidget;
  HWND normalWidget;
  HWND overlayWidget;

  HGLRC ctxNormal;
  HGLRC ctxOverlay;

  HDC hdcNormal;
  HDC hdcOverlay;

  SbVec2s glSize;

  PIXELFORMATDESCRIPTOR pfdNormal;
  PIXELFORMATDESCRIPTOR pfdOverlay;

  SbBool drawToFrontBuffer;
  SbBool glRealized;
  SbBool havefocus;

  int glModes;
  SbBool accumulationenabled, stencilenabled, alphachannelenabled;
  int bordersize;
  int lockcounter;
  int overlaylockcounter;

  static ATOM managerWndClassAtom;
  static ATOM glWndClassAtom;
  static ATOM glWidgetTmpAtom;
  static int widgetCounter;

protected:
  virtual SbBool isDirectRendering(void);
};

// ************************************************************************

#endif // !SOWINGLWIDGETP_H
