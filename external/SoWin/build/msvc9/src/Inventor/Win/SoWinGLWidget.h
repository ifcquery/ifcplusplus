#ifndef SOWIN_GLWIDGET_H
#define SOWIN_GLWIDGET_H

// src\Inventor\Win\SoWinGLWidget.h.  Generated from SoGuiGLWidget.h.in by configure.

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

#include <Inventor/SbBasic.h>
#include <Inventor/Win/SoWinBasic.h>
#include <Inventor/Win/SoWinComponent.h>

#ifdef __COIN_SOQT__
class QEvent;
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
/*
 * GL/glx.h includes X11/Xmd.h which contains typedefs for BOOL and
 * INT32 that conflict with the definitions in windef.h (which is
 * included from windows.h, which may be included from
 * Inventor/system/gl.h).  To avoid this conflict, we rename the
 * typedefs done in X11/Xmd.h to use other names (tempbool and
 * tempint32), and try to clean up the hack after the header has been
 * parsed.  2003-06-25 larsa
 */
#ifndef BOOL
#define BOOL tempbool
#define COIN_DEFINED_BOOL
#endif /* !BOOL */
#ifndef INT32
#define INT32 tempint32
#define COIN_DEFINED_INT32
#endif /* !INT32 */
#include <GL/glx.h>
/*
 * This is the cleanup part of the X11/Xmd.h conflict fix hack set up
 * above.  2003-06-25 larsa
 */
#ifdef COIN_DEFINED_BOOL
#undef BOOL
#undef COIN_DEFINED_BOOL
#endif /* COIN_DEFINED_BOOL */
#ifdef COIN_DEFINED_INT32
#undef INT32
#undef COIN_DEFINED_INT32
#endif /* COIN_DEFINED_INT32 */
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
#include <gtk/gtk.h>
#endif // __COIN_SOGTK__

// *************************************************************************

enum GLModes {
  SO_GL_RGB      = 0x01, SO_GLX_RGB      = SO_GL_RGB,
  SO_GL_DOUBLE   = 0x02, SO_GLX_DOUBLE   = SO_GL_DOUBLE,
  SO_GL_ZBUFFER  = 0x04, SO_GLX_ZBUFFER  = SO_GL_ZBUFFER,
  SO_GL_OVERLAY  = 0x08, SO_GLX_OVERLAY  = SO_GL_OVERLAY,
  SO_GL_STEREO   = 0x10, SO_GLX_STEREO   = SO_GL_STEREO
};

// *************************************************************************

class SOWIN_DLL_API SoWinGLWidget : public SoWinComponent {
  SOWIN_OBJECT_ABSTRACT_HEADER(SoWinGLWidget, SoWinComponent);

public:

  void setBorder(const SbBool enable);
  SbBool isBorder(void) const;

  virtual void setDoubleBuffer(const SbBool enable);
  SbBool isDoubleBuffer(void) const;

  void setDrawToFrontBufferEnable(const SbBool enable);
  SbBool isDrawToFrontBufferEnable(void) const;
  
  void setQuadBufferStereo(const SbBool enable);
  SbBool isQuadBufferStereo(void) const;

  void setAccumulationBuffer(const SbBool enable);
  SbBool getAccumulationBuffer(void) const;

  void setStencilBuffer(const SbBool enable);
  SbBool getStencilBuffer(void) const;

  void setAlphaChannel(const SbBool enable);
  SbBool getAlphaChannel(void) const;

  void setOverlayRender(const SbBool onoff);
  SbBool isOverlayRender(void) const;

  void setSampleBuffers(const int numsamples);
  int getSampleBuffers(void) const;

  void setStealFocus(SbBool enable);
  SbBool isStealFocus(void) const;

  HWND getGLWidget(void) const;
  HWND getNormalWidget(void) const;
  HWND getOverlayWidget(void) const;

  SbBool hasOverlayGLArea(void) const;
  SbBool hasNormalGLArea(void) const;

  unsigned long getOverlayTransparentPixel(void);

  // OpenGL query functions.
  void getPointSizeLimits(SbVec2f & range, float & granularity);
  void getLineWidthLimits(SbVec2f & range, float & granularity);

protected:
  SoWinGLWidget(HWND const parent = NULL,
                  const char * const name = NULL,
                  const SbBool embed = TRUE,
                  const int glmodes = SO_GL_RGB,
                  const SbBool build = TRUE);
  ~SoWinGLWidget();

  virtual void processEvent(MSG * event);

  HWND buildWidget(HWND parent);

  virtual void redraw(void) = 0;
  virtual void redrawOverlay(void);

  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);

  virtual void sizeChanged(const SbVec2s & size);
  virtual void widgetChanged(HWND w);

  void setGLSize(const SbVec2s size);
  SbVec2s getGLSize(void) const;
  float getGLAspectRatio(void) const;

  // old aliases
  void setGlxSize(const SbVec2s size) { this->setGLSize(size); }
  SbVec2s getGlxSize(void) const { return this->getGLSize(); }
  float getGlxAspectRatio(void) const { return this->getGLAspectRatio(); }

  void setStereoBuffer(SbBool flag);
  SbBool isStereoBuffer(void) const;

  SbBool isRGBMode(void);

  SbBool waitForExpose;
  SbBool drawToFrontBuffer;

  void glLockNormal(void);
  void glUnlockNormal(void);

  void glLockOverlay(void);
  void glUnlockOverlay(void);

  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual SbBool glScheduleRedraw(void);

private:
  class SoWinGLWidgetP * pimpl;
  friend class SoGuiGLWidgetP;
  friend class SoWinGLWidgetP;

  // FIXME: get rid of toolkit-specific stuff below. 20020613 mortene.

#ifdef __COIN_SOXT__
public:
  GLXContext getNormalContext(void);
  GLXContext getOverlayContext(void);

  Window getNormalWindow(void);
  Window getOverlayWindow(void);

  int getOverlayColorMapSize(void);
  int getColorMapSize(void);

  virtual void setNormalVisual(XVisualInfo * visual);
  XVisualInfo * getNormalVisual(void);
  virtual void setOverlayVisual(XVisualInfo * visual);
  XVisualInfo * getOverlayVisual(void);

protected:
  static void eventHandler(Widget, SoXtGLWidget *, XAnyEvent *, Boolean *);

  int getDisplayListShareGroup(GLXContext context);
  Widget getGlxMgrWidget(void);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
protected:
  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
#endif // __COIN_SOGTK__

};

// *************************************************************************

#endif // ! SOWIN_GLWIDGET_H
