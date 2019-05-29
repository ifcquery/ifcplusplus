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

// Class documentation in common/SoGuiGLWidgetCommon.cpp.in.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Win/common/gl.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinBasic.h>
#include <Inventor/Win/SoWinGLWidget.h>
#include <Inventor/Win/SoWinGLWidgetP.h>
#include <Inventor/Win/SoAny.h>
#include <sowindefs.h>
#include <Inventor/Win/Win32API.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/Win/common/SbGuiList.h>
#include <float.h> // FLT_MAX
#include <stdlib.h> // qsort

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

static const int SO_BORDER_THICKNESS = 2;

SOWIN_OBJECT_ABSTRACT_SOURCE(SoWinGLWidget);


// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoWinGLWidget::SoWinGLWidget(HWND const parent,
                             const char * const name,
                             const SbBool embed,
                             const int glmodes,
                             const SbBool build)
  : SoWinComponent(parent, name, embed)
{
  this->pimpl = new SoWinGLWidgetP(this);
  this->waitForExpose = TRUE;

  PRIVATE(this)->managerWidget = NULL;
  PRIVATE(this)->normalWidget = NULL;
  PRIVATE(this)->overlayWidget = NULL;

  PRIVATE(this)->drawToFrontBuffer = FALSE;

  PRIVATE(this)->ctxNormal = NULL;
  PRIVATE(this)->ctxOverlay = NULL;

  PRIVATE(this)->hdcNormal = NULL;
  PRIVATE(this)->hdcOverlay = NULL;

  PRIVATE(this)->glModes = glmodes;
  PRIVATE(this)->glRealized = FALSE;
  PRIVATE(this)->glSize = SbVec2s(0,0);

  this->setClassName("SoWinGLWidget");
	
  if (! build) return;
	
  PRIVATE(this)->managerWidget = this->buildWidget(parent);
  this->setBaseWidget(PRIVATE(this)->managerWidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoWinGLWidget::~SoWinGLWidget()
{
  // wglDeleteContext(this->ctxNormal), and other window clean-up
  // tasks, will be called implicitly: DestroyWindow() triggers the
  // WM_DESTROY event, which triggers an invocation of
  // SoWinGLWidget::destroyWindow().

  if (IsWindow(PRIVATE(this)->managerWidget))
    Win32::DestroyWindow(PRIVATE(this)->managerWidget);
  if (IsWindow(PRIVATE(this)->normalWidget))
    Win32::DestroyWindow(PRIVATE(this)->normalWidget);
  if (IsWindow(PRIVATE(this)->overlayWidget))
    Win32::DestroyWindow(PRIVATE(this)->overlayWidget);

  SoWinGLWidgetP::widgetCounter--;
  if (SoWinGLWidgetP::widgetCounter <= 0) {
    Win32::UnregisterClass("Manager Widget", NULL);
    SoWinGLWidgetP::managerWndClassAtom = 0;
    Win32::UnregisterClass("GL Widget", NULL);
    SoWinGLWidgetP::glWndClassAtom = 0;
    Win32::UnregisterClass("sowin_glwidget_temp", NULL);
    SoWinGLWidgetP::glWidgetTmpAtom = 0;
  }

  delete PRIVATE(this);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
HWND
SoWinGLWidget::getNormalWidget(void) const
{
  return PRIVATE(this)->normalWidget;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
HWND
SoWinGLWidget::getOverlayWidget(void) const
{
  return PRIVATE(this)->overlayWidget;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setOverlayRender(const SbBool onoff)
{
  PRIVATE(this)->glModes |= SO_GL_OVERLAY;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isOverlayRender(void) const
{
  return (PRIVATE(this)->glModes & SO_GL_OVERLAY) != 0;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setDoubleBuffer(SbBool flag)
{
  int old = PRIVATE(this)->glModes;
  if (flag) { PRIVATE(this)->glModes |= SO_GL_DOUBLE; }
  else { PRIVATE(this)->glModes &= ~SO_GL_DOUBLE; }

  if (PRIVATE(this)->glModes == old) { return; }

  Win32::DestroyWindow(this->getNormalWidget());
  PRIVATE(this)->buildNormalGLWidget(PRIVATE(this)->managerWidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isDoubleBuffer(void) const
{
  return (PRIVATE(this)->glModes & SO_GL_DOUBLE ? TRUE : FALSE);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setBorder(SbBool f)
{
  PRIVATE(this)->bordersize = f ? SO_BORDER_THICKNESS : 0;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isBorder(void) const
{
  return PRIVATE(this)->bordersize == SO_BORDER_THICKNESS;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setDrawToFrontBufferEnable(SbBool enable)
{
  PRIVATE(this)->drawToFrontBuffer = enable;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isDrawToFrontBufferEnable(void) const
{
  return PRIVATE(this)->drawToFrontBuffer;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setQuadBufferStereo(const SbBool flag)
{
  int old = PRIVATE(this)->glModes;
  if (flag) { PRIVATE(this)->glModes |= SO_GL_STEREO; }
  else { PRIVATE(this)->glModes &= ~SO_GL_STEREO; }

  if (PRIVATE(this)->glModes == old) { return; }

  Win32::DestroyWindow(this->getNormalWidget());
  PRIVATE(this)->buildNormalGLWidget(PRIVATE(this)->managerWidget);
}

void 
SoWinGLWidget::setSampleBuffers(const int numsamples)
{
  // FIXME: implement
}

int 
SoWinGLWidget::getSampleBuffers() const
{
  // FIXME: implement
  return 1;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setAccumulationBuffer(const SbBool enable)
{
  if (!enable == !PRIVATE(this)->accumulationenabled) { return; }

  PRIVATE(this)->accumulationenabled = enable;

  Win32::DestroyWindow(this->getNormalWidget());
  PRIVATE(this)->buildNormalGLWidget(PRIVATE(this)->managerWidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::getAccumulationBuffer(void) const
{
  return PRIVATE(this)->accumulationenabled;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setStencilBuffer(const SbBool enable)
{
  if (!enable == !PRIVATE(this)->stencilenabled) { return; }

  PRIVATE(this)->stencilenabled = enable;

  Win32::DestroyWindow(this->getNormalWidget());
  PRIVATE(this)->buildNormalGLWidget(PRIVATE(this)->managerWidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::getStencilBuffer(void) const
{
  return PRIVATE(this)->stencilenabled;
}


// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setAlphaChannel(const SbBool enable)
{
  if (!enable == !PRIVATE(this)->alphachannelenabled) { return; }

  PRIVATE(this)->alphachannelenabled = enable;

  Win32::DestroyWindow(this->getNormalWidget());
  PRIVATE(this)->buildNormalGLWidget(PRIVATE(this)->managerWidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::getAlphaChannel(void) const
{
  return PRIVATE(this)->alphachannelenabled;
}


// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isQuadBufferStereo(void) const
{
  return (PRIVATE(this)->glModes & SO_GL_STEREO ? TRUE : FALSE);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::hasOverlayGLArea(void) const
{
  return (IsWindow(this->getOverlayWidget()) ? TRUE : FALSE);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::hasNormalGLArea(void) const
{
  return (IsWindow(this->getNormalWidget()) ? TRUE : FALSE);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::glScheduleRedraw(void)
{
  return FALSE;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::redraw(void)
{
  // virtual - does nothing
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::redrawOverlay(void)
{
  // virtual - does nothing
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::processEvent(MSG * msg)
{
  // Nothing is done here for the SoWinGLWidget, as the events we need
  // to handle for this superclass are caught by the method
  // SoWinGLWidgetP::glWidgetProc() and forwarded directly to
  // SoWinGLWidget::onPaint() etc.  The events we don't care about
  // (like mouse- and keyboard-interaction) are forwarded from
  // glWidgetProc() through this virtual method down to the
  // subclass(es).
  //                          mortene.
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::initGraphic(void)
{
  this->glLockNormal();
  glEnable(GL_DEPTH_TEST);
  this->glUnlockNormal();
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::initOverlayGraphic(void)
{
  // virtual - does nothing
}

// doc in super
void
SoWinGLWidget::sizeChanged(const SbVec2s & newSize)
{
  // virtual - does nothing
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::widgetChanged(HWND newWidget)
{
  // virtual
  // called whenever the widget is changed (i.e. at initialization
  // or after switching from single->double buffer)
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setGLSize(SbVec2s newSize)
{
  if (newSize == PRIVATE(this)->glSize) return;
  PRIVATE(this)->glSize = newSize;

  UINT flags = SWP_NOMOVE | SWP_NOZORDER;
  Win32::SetWindowPos(PRIVATE(this)->managerWidget, NULL, 0, 0,
                         newSize[0], newSize[1], flags);

  flags = SWP_NOMOVE | SWP_NOZORDER;
  Win32::SetWindowPos(PRIVATE(this)->normalWidget,
                      NULL,
                      PRIVATE(this)->bordersize,
                      PRIVATE(this)->bordersize,
                      newSize[0] - 2 * PRIVATE(this)->bordersize,
                      newSize[1] - 2 * PRIVATE(this)->bordersize,
                      flags);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbVec2s
SoWinGLWidget::getGLSize(void) const
{
  return PRIVATE(this)->glSize;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
float
SoWinGLWidget::getGLAspectRatio(void) const
{
  return (float) PRIVATE(this)->glSize[0] /
    (float) PRIVATE(this)->glSize[1];
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
unsigned long
SoWinGLWidget::getOverlayTransparentPixel(void)
{
  SOWIN_STUB();
  // FIXME: investigate if this function is really used for the other
  // toolkits. 20011012 mortene.
  return 0;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isRGBMode(void)
{
  return (PRIVATE(this)->glModes & SO_GL_RGB);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
HWND
SoWinGLWidget::buildWidget(HWND parent)
{
  SoWinGLWidgetP::widgetCounter++;

  HMENU menu = NULL;

  // Build managerWidget.  Used only to draw borders.

  if (! SoWinGLWidgetP::managerWndClassAtom) {
    WNDCLASS windowclass;
    windowclass.lpszClassName = "Manager Widget";
    windowclass.hInstance = NULL;
    windowclass.lpfnWndProc = SoWinGLWidgetP::mgrWidgetProc;
    windowclass.style = 0;
    windowclass.lpszMenuName = NULL;
    windowclass.hIcon = NULL;
    windowclass.hCursor = Win32::LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = GetSysColorBrush(COLOR_3DSHADOW);
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = sizeof(LONG_PTR);

    SoWinGLWidgetP::managerWndClassAtom = Win32::RegisterClass(&windowclass);
  }

  RECT rect;
  assert(IsWindow(parent) && "buildWidget() argument erroneous");
  Win32::GetClientRect(parent, & rect);

  HWND managerwidget = Win32::CreateWindow_("Manager Widget",
                                            this->getTitle(),
                                            WS_VISIBLE |
                                            WS_CLIPSIBLINGS |
                                            WS_CLIPCHILDREN |
                                            WS_CHILD,
                                            rect.left,
                                            rect.top,
                                            rect.right,
                                            rect.bottom,
                                            parent,
                                            menu,
                                            NULL,
                                            this);

  PRIVATE(this)->managerWidget = managerwidget;

  // FIXME: looks shaky to keep this here as long as overlay support
  // has not been implemented.  20010920 mortene.
  if (PRIVATE(this)->glModes & SO_GL_OVERLAY) {
    PRIVATE(this)->buildOverlayGLWidget(managerwidget);
  }

  PRIVATE(this)->buildNormalGLWidget(managerwidget);
  this->waitForExpose = TRUE;

  return managerwidget;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
HWND
SoWinGLWidget::getGLWidget(void) const
{
  return PRIVATE(this)->normalWidget;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glLockNormal(void)
{
  assert(PRIVATE(this)->hdcNormal != NULL);
  assert(PRIVATE(this)->lockcounter >= 0);

#if defined(SOWIN_DEBUG) && 0 // debug
  SoDebugError::postInfo("SoWinGLWidget::glLockNormal",
                         "lockcounter: %d => %d",
                         PRIVATE(this)->lockcounter,
                         PRIVATE(this)->lockcounter + 1);
#endif // debug


  PRIVATE(this)->lockcounter++;

  // Make context current no matter what the value of the lock-counter
  // is, or we could get crash bugs if the context has been released
  // from somewhere else (for instance from application code).
  (void)SoWinGLWidgetP::wglMakeCurrent(PRIVATE(this)->hdcNormal,
                                       PRIVATE(this)->ctxNormal);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glUnlockNormal(void)
{
#if defined(SOWIN_DEBUG) && 0 // debug
  SoDebugError::postInfo("SoWinGLWidget::glUnlockNormal",
                         "lockcounter: %d => %d",
                         PRIVATE(this)->lockcounter,
                         PRIVATE(this)->lockcounter - 1);
#endif // debug

  if (PRIVATE(this)->lockcounter == 0) {
#if defined(SOWIN_DEBUG)
    SoDebugError::post("SoWinGLWidget::glUnlockNormal",
                       "GL-context lock counter too low (internal error)");
#endif // SOWIN_DEBUG
    return;
  }

  PRIVATE(this)->lockcounter--;

  // Must wait until lockcounter goes to 0 before resetting, or we
  // could get problems from any internal SoWin code snippet that goes
  // like this:
  //
  // glLockNormal();
  // [OpenGL commands here];
  // someOtherSoWinFunctionWithLockUnlockPairing();
  // [OpenGL commands here]; /* <-- crash here because no current context */
  // glUnlockNormal();

  if (PRIVATE(this)->lockcounter == 0) {
    // FIXME: shouldn't we rather reset to a stored previous context?
    // 20020718 mortene.
    (void)SoWinGLWidgetP::wglMakeCurrent(NULL, NULL);
    // FIXME: should also investigate if we really have to unlock an
    // OpenGL context under Win32 WGL. Will save us some trouble if
    // it's unnecessary (like under GLX). 20020930 mortene.
  }
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glLockOverlay(void)
{
  // FIXME: there should be a counter for the lock level here.
  // 20010924 mortene.

  (void)SoWinGLWidgetP::wglMakeCurrent(PRIVATE(this)->hdcOverlay,
                                       PRIVATE(this)->ctxOverlay);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glUnlockOverlay(void)
{
  // FIXME: there should be a counter for the lock level here.
  // 20010924 mortene.

  (void)SoWinGLWidgetP::wglMakeCurrent(NULL, NULL);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glSwapBuffers(void)
{
  assert(PRIVATE(this)->hdcNormal != NULL);
  BOOL ok = wglSwapLayerBuffers(PRIVATE(this)->hdcNormal, WGL_SWAP_MAIN_PLANE);
  // FIXME: should have better error reporting and handling. 20020528 mortene.
  assert(ok == TRUE);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::glFlushBuffer(void)
{
  // FIXME: make OpenGL context(s) current first? 20010924 mortene.
  glFlush();
}

///////////////////////////////////////////////////////////////////
//
//  Pimpl class follows. The private data and code for the
//  SoWinGLWidget.

#ifndef DOXYGEN_SKIP_THIS

ATOM SoWinGLWidgetP::managerWndClassAtom = 0;
ATOM SoWinGLWidgetP::glWndClassAtom = 0;
ATOM SoWinGLWidgetP::glWidgetTmpAtom = 0;
int SoWinGLWidgetP::widgetCounter = 0;

SoWinGLWidgetP::SoWinGLWidgetP(SoWinGLWidget * o)
  : SoGuiGLWidgetP(o)
{
  this->bordersize = 0;
  this->lockcounter = 0;
  this->overlaylockcounter = 0;
  this->havefocus = FALSE;
  this->accumulationenabled = FALSE;
  this->stencilenabled = FALSE;
  this->alphachannelenabled = FALSE;
}

// Destructor.
SoWinGLWidgetP::~SoWinGLWidgetP()
{
}

LRESULT CALLBACK
SoWinGLWidgetP::mgrWidgetProc(HWND window, UINT message,
                              WPARAM wparam, LPARAM lparam)
{
  // does nothing
  return DefWindowProc(window, message, wparam, lparam);
}

LRESULT CALLBACK
SoWinGLWidgetP::glWidgetProc(HWND window, UINT message,
                             WPARAM wparam, LPARAM lparam)
{
  if (message == WM_CREATE) {
    CREATESTRUCT * createstruct = (CREATESTRUCT *) lparam;
    SoWinGLWidget * object = (SoWinGLWidget *)(createstruct->lpCreateParams);
    (void)Win32::SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)object);
    return PRIVATE(object)->onCreate(window, message, wparam, lparam);
  }

  SoWinGLWidget * object = (SoWinGLWidget *)
    Win32::GetWindowLongPtr(window, GWLP_USERDATA);

  if (message == WM_INPUT) { // Spacemouse/navigator/3DConnexion input?
    MSG msg;
    POINT pt = { LOWORD(lparam), HIWORD(lparam) };
    msg.hwnd = window;
    msg.lParam = lparam;
    msg.message = message;
    msg.pt = pt;
    msg.time = GetTickCount();
    msg.wParam = wparam;
    object->processEvent(&msg);
    return 0; // Return now to prevent duplicated events.
  }
  
  if (object && window == object->getNormalWidget()) {
    MSG msg;
    POINT pt = { LOWORD(lparam), HIWORD(lparam) };
    msg.hwnd = window;
    msg.lParam = lparam;
    msg.message = message;
    msg.pt = pt;
    msg.time = GetTickCount();
    msg.wParam = wparam;

    // Grab focus so keystrokes are piped in our direction.
    if (!PRIVATE(object)->havefocus) {
      switch (message) {

      // FIXME: the canvas/widget/window should also grab focus on
      // mouse enter events -- shouldn't it? See doc on
      // setStealFocus(). Or will this cause trouble with the typical
      // click-to-focus policy under Windows? 20050412 mortene.

      case WM_LBUTTONDOWN:
      case WM_MBUTTONDOWN:
      case WM_RBUTTONDOWN:
        (void)Win32::SetFocus(window);
        PRIVATE(object)->havefocus = TRUE;
        break;
      default: break;
      }
    }

    object->processEvent(&msg);

    switch (message) {

    case WM_PAINT:
      object->waitForExpose = FALSE; // flip flag on first expose
      return PRIVATE(object)->onPaint(window, message, wparam, lparam);

    case WM_DESTROY:
      PRIVATE(object)->destroyWindow(window);
      return 0;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
      (void)SetCapture(window);
      return 0;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
      (void)ReleaseCapture();
      return 0;

    case WM_KILLFOCUS:
      PRIVATE(object)->havefocus = FALSE;
      return 0;

    case WM_SETFOCUS:
      PRIVATE(object)->havefocus = TRUE;
      return 0;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
      if (wparam == VK_MENU)
        return 0;
      break;
    }
  }
  return DefWindowProc(window, message, wparam, lparam);
}

LRESULT // Used by SoWinRenderArea
SoWinGLWidgetP::eventHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // FIXME: the function gets registered and all in SoWinRenderArea,
  // so why does event handling differ in SoWin versus SoXt and SoQt?
  // 20030411 mortene.
  assert(FALSE && "should not be in use");
  return 0;
}

void
SoWinGLWidgetP::buildNormalGLWidget(HWND manager)
{
  assert(IsWindow(manager) && "buildNormalGLWidget() argument is erroneous");
  const LPSTR wndclassname = "GL Widget";

  if (!SoWinGLWidgetP::glWndClassAtom) {
    WNDCLASS windowclass;
    windowclass.lpszClassName = wndclassname;
    windowclass.hInstance = NULL;
    windowclass.lpfnWndProc = SoWinGLWidgetP::glWidgetProc;
    windowclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowclass.lpszMenuName = NULL;
    windowclass.hIcon = NULL;
    windowclass.hCursor = Win32::LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = NULL;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = sizeof(LONG_PTR);

    SoWinGLWidgetP::glWndClassAtom = Win32::RegisterClass(&windowclass);
  }

  RECT rect;
  Win32::GetClientRect(manager, & rect);

  rect.left += this->bordersize;
  rect.top += this->bordersize;
  rect.right -= 2 * this->bordersize;
  rect.bottom -= 2 * this->bordersize;

  HWND normalwidget = Win32::CreateWindowEx_(0,
                                             wndclassname,
                                             wndclassname,
                                             WS_VISIBLE |
                                             WS_CLIPSIBLINGS |
                                             WS_CLIPCHILDREN |
                                             WS_CHILD,
                                             rect.left, rect.top,
                                             rect.right, rect.bottom,
                                             manager,
                                             NULL,
                                             NULL,
                                             PUBLIC(this));

  this->normalWidget = normalwidget;
  PUBLIC(this)->setGLSize(SbVec2s((short)(rect.right - rect.left),
                                  (short)(rect.bottom - rect.top)));
}

void
SoWinGLWidgetP::buildOverlayGLWidget(HWND manager)
{
  // FIXME: function not implemented
  // FIXME: overlay not supported. mariusbu 20010719.
  SOWIN_STUB();
}

// Return value of SOWIN_DEBUG_GL_SETUP environment variable.
SbBool
SoWinGLWidgetP::debugGLContextCreation(void)
{
  static const char * debuggl = SoAny::si()->getenv("SOWIN_DEBUG_GL_SETUP");
  int val = debuggl ? atoi(debuggl) : 0;
  return val > 0;
}

// This is our replacement for the Win32 / OpenGL ChoosePixelFormat()
// function. Returns -FLT_MAX for pixelformats incompatible with
// OpenGL rendering (in onscreen windows), otherwise a value that
// indicates it's "goodness", where larger value means it seems better
// suited for what is requested.
double
SoWinGLWidgetP::weighPixelFormat(const PIXELFORMATDESCRIPTOR * pfd,
                                 SbBool want_rgb, /* default TRUE */
                                 SbBool want_dblbuffer, /* default TRUE */
                                 SbBool want_zbuffer, /* default TRUE */
                                 SbBool want_stereo, /* default FALSE */
                                 SbBool want_accum, /* default FALSE */
                                 SbBool want_stencil, /* default FALSE */
                                 SbBool want_overlay, /* default FALSE */
                                 SbBool want_alphachannel) /* default FALSE */
{
  // FIXME: I have thought of one improvement we should make: fix
  // interface to make it possible to ask for a particular amount of
  // color and/or depth and/or stencil and/or accumulation buffer
  // bits, and then punish *extra* bits aswell as too few (but punish
  // fewer bits *more* than extra bits).
  //
  // The rationale behind this suggestion is that excessive bits takes
  // up more memory, which increases the chances that
  // e.g. wglMakeCurrent() will later fail due to resource
  // problems.
  //
  // 20031203 mortene.

  if ((pfd->dwFlags & PFD_SUPPORT_OPENGL) == 0) { return -FLT_MAX; }
  // We only care for on-screen windows for now. If we ever want to
  // use this weighting method for offscreen buffers aswell, we should
  // extend with another argument for the function signature to check
  // specifically for this.
  if ((pfd->dwFlags & PFD_DRAW_TO_WINDOW) == 0) { return -FLT_MAX; }

  // FIXME: I believe these pixelformats to be unsupported, as we
  // don't set up any palettes yet:
  if ((pfd->dwFlags & (PFD_NEED_PALETTE|PFD_NEED_SYSTEM_PALETTE)) != 0) { return -FLT_MAX; }


  // *** set up parameter constants for weighing *****************

  // a baseline value
  const double MAX_IMPORTANCE = 1000.0;

  // very important to get right, as it will have a _major_ impact on
  // the rendering
  const double RGBA_VS_COLORINDEX = MAX_IMPORTANCE;

  // equally important -- no zbuffer means rendering will be
  // incomprehensible
  const double ZBUFFER_PRESENT = MAX_IMPORTANCE;

  // veeery close to being as important as the two first "musts" (as
  // we generally _will_ have non-accelerated pixelformats available
  // even on drivers with vendor-specific OpenGL ICDs for gfx
  // hardware)
  const double HARDWARE_ACCELERATED = MAX_IMPORTANCE - 1;

  // singlebuffer sucks when you want doublebuffer
  const double DOUBLEBUFFER_PRESENT = MAX_IMPORTANCE / 2;

  // the next three are often hard to find
  const double STEREO_PRESENT = MAX_IMPORTANCE / 10;
  const double OVERLAYS_AVAILABLE = MAX_IMPORTANCE / 30;
  const double PER_OVERLAY_PLANE = MAX_IMPORTANCE / 90;

  // Depth buffer resolution is a lot more important than color
  // resolution, actually, as really low z-buffer resolution (8 or
  // even 16 bits) causes unexpected nasty visual artifacts for many
  // types of scenes.
  //
  // Setting depth resolution to be this much more important than
  // color resolution also fixes a problem with Win32
  // ChoosePixelFormat() we've experienced with some NVidia drivers
  // (with GeForce2MX and GeForce2Go cards): if we requested a 32 bit
  // depth buffer, ChoosePixelFormat() returned an 8 bit (or
  // something) depth buffer, since 32 bits resolution is not
  // supported on those cards (only 16 and / or 24 bits resolution).
  const double PER_COLOR_BIT = 1;
  const double PER_ALPHA_BIT = 0.75;
  const double PER_DEPTH_BIT = 2.5;

  const double PER_ACCUMULATION_BIT = 0.5;
  const double PER_STENCIL_BIT = 0.5;


  // *** calculate weight for given pixelformat ******************

  double weight = 0.0;
  double alpha_cont = 0.0;
  double stencil_cont = 0.0;
  double accum_cont = 0.0;
  double depth_cont = 0.0;
  double color_cont = 0.0;
  double rgba_cont = 0.0;
  double zbuffer_cont = 0.0;
  double dblbuffer_cont = 0.0;
  double stereo_cont = 0.0;
  double overlay_cont = 0.0;
  double hw_accel_cont = 0.0;

  // want_rgb && PFD_TYPE_RGBA         =>  +1
  // want_rgb && PFD_TYPE_COLORINDEX   =>  -1
  // !want_rgb && PFD_TYPE_RGBA        =>  -1
  // !want_rgb && PFD_TYPE_COLORINDEX  =>  +1

  int sign = (want_rgb ? 1 : -1) * ((pfd->iPixelType == PFD_TYPE_RGBA) ? 1 : -1);
  rgba_cont = RGBA_VS_COLORINDEX * sign;

  const SbBool has_zbuffer = (pfd->cDepthBits > 0);
  // want_zbuffer && has_zbuffer    =>  +1
  // want_zbuffer && !has_zbuffer   =>  -1
  // !want_zbuffer && has_zbuffer   =>  -1
  // !want_zbuffer && !has_zbuffer  =>  +1
  sign = (want_zbuffer ? 1 : -1) * (has_zbuffer ? 1 : -1);
  zbuffer_cont = ZBUFFER_PRESENT * sign;

  const SbBool has_dblbuffer = ((pfd->dwFlags & PFD_DOUBLEBUFFER) != 0);
  if (want_dblbuffer && has_dblbuffer) { dblbuffer_cont = DOUBLEBUFFER_PRESENT; }
  // otherwise ignore -- no need to punish doublebuffer mode if we
  // requested singlebuffer mode, as singlebuffer can flawlessly be
  // "faked" in doublebuffer

  const SbBool has_stereo = ((pfd->dwFlags & PFD_STEREO) != 0);
  if (want_stereo) { 
    if (has_stereo) stereo_cont = STEREO_PRESENT; 
  }
  else if (has_stereo) {
    // punish stereo mode if we requested mono. Stereo mode uses lots
    // of gfx memory
    stereo_cont = -STEREO_PRESENT; 
  }
  
  // We reward extra weight points to formats with accumulation bits,
  // stencil bits or alpha bits if they are actually requested. We *punish*
  // formats with them if they are *not* requested.
  if (want_accum) {
    // FIXME: What is the proper punishment for a missing accumulation buffer? 20031215 handegar
    if (pfd->cAccumBits == 0) accum_cont = -MAX_IMPORTANCE / 3;
    else accum_cont = PER_ACCUMULATION_BIT * pfd->cAccumBits;      
  }
  else accum_cont = -PER_ACCUMULATION_BIT * pfd->cAccumBits;      

  if (want_stencil) {
    // FIXME: What is the proper punishment for a missing stencil? 20031215 handegar
    if (pfd->cStencilBits == 0) stencil_cont = -MAX_IMPORTANCE / 3;
    else stencil_cont = PER_STENCIL_BIT * pfd->cStencilBits;      
  }
  else stencil_cont = -PER_STENCIL_BIT * pfd->cStencilBits;      

  if (want_alphachannel) {
    // FIXME: What is the proper punishment for a missing alpha channel? 20031215 handegar
    if (pfd->cAlphaBits == 0) alpha_cont = -MAX_IMPORTANCE / 4;
    else alpha_cont = PER_ALPHA_BIT * pfd->cAlphaBits;      
  }
  else alpha_cont = -PER_ALPHA_BIT * pfd->cAlphaBits;      

  // The rationale behind punishing unwanted accum and stencil buffers
  // is that extra accumulation and stencil planes can potentially
  // take up a lot of memory, so we don't want just random selection
  // if two or more otherwise equal formats will demand different
  // amounts of graphics memory. (We have had reports from systems
  // where the user ran out of graphics memory due to this problem,
  // which is why we introduced the current strategy.)


  if (want_overlay) {
    const int nr_planes = pfd->bReserved & 0x07;
    overlay_cont = ((nr_planes > 0) ? 1 : 0) * OVERLAYS_AVAILABLE;
    overlay_cont += PER_OVERLAY_PLANE * nr_planes;
    overlay_cont += OVERLAYS_AVAILABLE / 10 *
      ((pfd->dwFlags & PFD_SWAP_LAYER_BUFFERS) ? 1 : 0);
  }
  
  // first check for OpenGL installable client driver
  SbBool hw_accel = (pfd->dwFlags & (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED)) == 0;
  if (!hw_accel) {
    // then test for mini client driver
    hw_accel = (pfd->dwFlags & (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED)) == 
      (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED);
  }
    
  // Note: there are two types of possible "non-generic"
  // configurations. From a 1996 Usenet posting by a Microsoft
  // engineer:
  //
  // ---8<--- [snip] --------8<--- [snip] --------8<--- [snip] -----
  //
  // [...] In the upcoming OpenGL 1.1 for Windows NT/95, we have added
  // a new flag PFD_GENERIC_ACCELERATED (0x00001000) to identify an
  // OpenGL Mini Client Driver (MCD).  [...]
  //
  // So there are now 3 different classes of hardware identified by pixel 
  // formats:
  //
  // 1. (pfd->flags & (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED)) == 0
  //     => OpenGL Installable Client Driver (e.g. Intergraph, AccelGraphics...)
  //
  // 2. (pfd->flags & (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED)) == (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATE)
  //     => OpenGL Mini Client Driver (e.g. Matrox Millennium...)
  //
  // 3. (pfd->flags & (PFD_GENERIC_FORMAT | PFD_GENERIC_ACCELERATED)) == PFD_GENERIC_FORMAT
  //     => OpenGL Generic (software) Implementation
  //
  // Hock San Lee
  // Microsoft
  //
  // ---8<--- [snip] --------8<--- [snip] --------8<--- [snip] -----
  //
  // I couldn't figure out whether there are any quality differences
  // between a "Mini Client Driver" and an "Installable Client
  // Driver", or even if they can be present at the same time, but it
  // seems safe to assume that just not having PFD_GENERIC_FORMAT
  // present means we have hardware-accelerated rendering.
  //
  // Anyone reading this who could educate me further on MS OpenGL
  // acceleration issues and hardware drivers -- please feel free to
  // do so.
  //
  // mortene.
  hw_accel_cont = (hw_accel ? 1 : 0) * HARDWARE_ACCELERATED;
  
  depth_cont = pfd->cDepthBits * PER_DEPTH_BIT;
  color_cont = pfd->cColorBits * PER_COLOR_BIT;

  weight = 
    alpha_cont +
    stencil_cont +
    accum_cont +
    depth_cont +
    color_cont +
    rgba_cont +
    zbuffer_cont +
    dblbuffer_cont +
    stereo_cont +
    overlay_cont +
    hw_accel_cont;

  if (SoWinGLWidgetP::debugGLContextCreation()) {
    SoDebugError::postInfo("SoWinGLWidgetP::weighPixelFormat",
			   "weight: %g, alpha: %g, stencil: %g, accum: %g, depth: %g, color: %g "
			   "rgb: %g, zbuffer: %g, dblbuffer: %g, stereo: %g, overlay: %g, hardware: %g",
			   weight,
			   alpha_cont,
			   stencil_cont,
			   accum_cont,
			   depth_cont,
			   color_cont,
			   rgba_cont,
			   zbuffer_cont,
			   dblbuffer_cont,
			   stereo_cont,
			   overlay_cont,
			   hw_accel_cont);
  }
			 

  // The following are "don't care" properties for now (which is
  // likely to change when we implement missing pieces of
  // SoWinGLWidget):
  //
  // dwFlags:
  //   - PFD_DRAW_TO_BITMAP
  //   - PFD_SUPPORT_GDI
  //   - PFD_SWAP_COPY
  //   - PFD_SWAP_EXCHANGE
  //
  // - individual color bit settings (cRedBits, cGreenBits, cBlueBits)
  // - cAccumBits (with individual R, G, B, A)
  // - cAuxBuffers

  return weight;
}

// List all available pixel formats in an SoDebugError window for the
// given device context.
void
SoWinGLWidgetP::dumpPixelFormat(HDC hdc, int format)
{
  PIXELFORMATDESCRIPTOR desc;
  int maxformat = DescribePixelFormat(hdc, format,
                                      sizeof(PIXELFORMATDESCRIPTOR), &desc);
  if (maxformat == 0) {
    DWORD dummy;
    SbString err = Win32::getWin32Err(dummy);
    SbString s = "DescribePixelFormat() failed with error message: ";
    s += err;
    SoDebugError::post("SoWinGLWidgetP::dumpPixelFormat", s.getString());
    return;
  }

  struct dwFlagsPair { int flag; const char * name; };
  struct dwFlagsPair dwFlags[] = {
    { PFD_DRAW_TO_WINDOW, "DRAW_TO_WINDOW" },
    { PFD_DRAW_TO_BITMAP, "DRAW_TO_BITMAP" },
    { PFD_SUPPORT_GDI, "SUPPORT_GDI" },
    { PFD_SUPPORT_OPENGL, "SUPPORT_OPENGL" },
    { PFD_GENERIC_ACCELERATED, "GENERIC_ACCELERATED" },
    { PFD_GENERIC_FORMAT, "GENERIC_FORMAT" },
    { PFD_NEED_PALETTE, "NEED_PALETTE" },
    { PFD_NEED_SYSTEM_PALETTE, "NEED_SYSTEM_PALETTE" },
    { PFD_DOUBLEBUFFER, "DOUBLEBUFFER" },
    { PFD_STEREO, "STEREO" },
    { PFD_SWAP_LAYER_BUFFERS, "SWAP_LAYER_BUFFERS" },
    { PFD_SWAP_COPY, "SWAP_COPY" },
    { PFD_SWAP_EXCHANGE, "SWAP_EXCHANGE" }
  };

  SbString dwFlagsStr("");
  for (int i=0; i < (int)(sizeof(dwFlags) / sizeof(dwFlags[0])); i++) {
    if (dwFlags[i].flag & desc.dwFlags) {
      if (dwFlagsStr.getLength() > 0) { dwFlagsStr += '|'; }
      dwFlagsStr += dwFlags[i].name;
    }
  }

  SbString iPixelType("unknown");
  if (desc.iPixelType == PFD_TYPE_RGBA) { iPixelType = "RGBA"; }
  if (desc.iPixelType == PFD_TYPE_COLORINDEX) { iPixelType = "COLORINDEX"; }

  SoDebugError::postInfo("SoWinGLWidgetP::dumpPixelFormat",
                         "\npixelformat %d:\n"
                         "  dwFlags==%s (0x%x)\n"
                         "  iPixelType==%s\n"
                         "  cColorBits==%d\n"
                         "  [cRedBits, cGreenBits, cBlueBits, cAlphaBits]==[%d, %d, %d, %d]\n"
                         "  [cRedShift, cGreenShift, cBlueShift, cAlphaShift]==[%d, %d, %d, %d]\n"
                         "  cAccumBits==%d\n"
                         "  [cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits]==[%d, %d, %d, %d]\n"
                         "  cDepthBits==%d\n"
                         "  cStencilBits==%d\n"
                         "  cAuxBuffers==%d\n"
                         "  overlayplanes==%d, underlayplanes==%d  (0x%x)\n"
                         "  transparent color or index == 0x%x"
                         , format
                         , dwFlagsStr.getString(), desc.dwFlags
                         , iPixelType.getString()
                         , desc.cColorBits
                         , desc.cRedBits, desc.cGreenBits, desc.cBlueBits, desc.cAlphaBits
                         , desc.cRedShift, desc.cGreenShift, desc.cBlueShift, desc.cAlphaShift
                         , desc.cAccumBits, desc.cAccumRedBits, desc.cAccumGreenBits, desc.cAccumBlueBits, desc.cAccumAlphaBits
                         , desc.cDepthBits, desc.cStencilBits, desc.cAuxBuffers
                         , desc.bReserved & 0x7, (desc.bReserved & (0xff - 0x7)) >> 3, desc.bReserved
                         , desc.dwVisibleMask
                         );
}


// struct used to sort pixel format descriptors based on weight
typedef struct {
  int format;
  int override;
  double weight;
  PIXELFORMATDESCRIPTOR pfd; 
} so_pixel_format;

extern "C" {
static int
compare_pixel_formats(const void * v0, const void * v1)
{
  so_pixel_format * p0 = *((so_pixel_format**)v0);
  so_pixel_format * p1 = *((so_pixel_format**)v1);
 
  if (p0->override) return -1;
  else if (p1->override) return 1;
 
  if (p0->weight > p1->weight) return -1;
  else if (p0->weight < p1->weight) return 1;
  return 0;
}
}

void
SoWinGLWidgetP::createGLContext(HWND window)
{
  // FIXME: if the pixelformat set up is _not_ an RGB (truecolor)
  // format, we should set up a DC palette here.
  //
  // Something like this:
  //
  // ---8<--- [snip] ------8<--- [snip] ------8<--- [snip] ---
  //    if (pfd.dwFlags & PFD_NEED_PALETTE ||
  //        pfd.iPixelType == PFD_TYPE_COLORINDEX) {
  //
  //      n = 1 << pfd.cColorBits;
  //      if (n > 256) n = 256;
  //
  //      lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
  //                                  sizeof(PALETTEENTRY) * n);
  //      memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
  //      lpPal->palVersion = 0x300;
  //      lpPal->palNumEntries = n;
  //
  //      GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
  //
  //      /* if the pixel type is RGBA, then we want to make an RGB ramp,
  //      otherwise (color index) set individual colors. */
  //      if (pfd.iPixelType == PFD_TYPE_RGBA) {
  //        int redMask = (1 << pfd.cRedBits) - 1;
  //        int greenMask = (1 << pfd.cGreenBits) - 1;
  //        int blueMask = (1 << pfd.cBlueBits) - 1;
  //        int i;
  //
  //        /* fill in the entries with an RGB color ramp. */
  //        for (i = 0; i < n; ++i) {
  //          lpPal->palPalEntry[i].peRed =
  //            (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
  //          lpPal->palPalEntry[i].peGreen =
  //            (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
  //          lpPal->palPalEntry[i].peBlue =
  //            (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
  //          lpPal->palPalEntry[i].peFlags = 0;
  //        }
  //      } else {
  //        lpPal->palPalEntry[0].peRed = 0;
  //        lpPal->palPalEntry[0].peGreen = 0;
  //        lpPal->palPalEntry[0].peBlue = 0;
  //        lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
  //        lpPal->palPalEntry[1].peRed = 255;
  //        lpPal->palPalEntry[1].peGreen = 0;
  //        lpPal->palPalEntry[1].peBlue = 0;
  //        lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
  //        lpPal->palPalEntry[2].peRed = 0;
  //        lpPal->palPalEntry[2].peGreen = 255;
  //        lpPal->palPalEntry[2].peBlue = 0;
  //        lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
  //        lpPal->palPalEntry[3].peRed = 0;
  //        lpPal->palPalEntry[3].peGreen = 0;
  //        lpPal->palPalEntry[3].peBlue = 255;
  //        lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
  //      }
  //
  //      hPalette = CreatePalette(lpPal);
  //      if (hPalette) {
  //        SelectPalette(hDC, hPalette, FALSE);
  //        RealizePalette(hDC);
  //      }
  //
  //      free(lpPal);
  //    }
  // ---8<--- [snip] ------8<--- [snip] ------8<--- [snip] ---
  // 20020719 mortene.

  assert(IsWindow(window));

  SoWinGLWidget * share = NULL;

  // All contexts were destroyed or released in destroyWindow().

  this->hdcNormal = GetDC(window);
  assert(this->hdcNormal && "GetDC failed -- investigate");
  this->hdcOverlay = this->hdcNormal;
  this->glRealized = FALSE; // so that initGraphic() is executed for the new context

  SbBool didtry = FALSE;

  int format = 1, maxformat = -1;  
  SbGuiList <so_pixel_format*> pflist;
  so_pixel_format * pf;
  SbBool foundone = FALSE;
  int i = 0;
  SbBool mainwinerror = FALSE;

  // Make it possible to override the selected pixel format by setting
  // an environment variable. Useful for debugging and for assisting
  // on remote client systems where the pixelformat weighting selects
  // a sub-optimal format. If the latter, this can be used as a
  // stop-gap solution until we fix the weighting algorithm.
  //
  // OIV_FORCE_PIXEL_FORMAT matches what TGS uses for their
  // InventorWin library. SOWIN_OVERRIDE_PIXELFORMAT is for backward
  // compatibility with old client code.

  int overrideformat = -1;
  const char * pixelformatoverride =
    SoAny::si()->getenv("OIV_FORCE_PIXEL_FORMAT");
  if (!pixelformatoverride) {
    pixelformatoverride = SoAny::si()->getenv("SOWIN_OVERRIDE_PIXELFORMAT");
  }
  if (pixelformatoverride) {
    overrideformat = atoi(pixelformatoverride);
  }

  do {    
    pf = new so_pixel_format;
    pf->format = format;
    pf->override = 0;
    
    maxformat = DescribePixelFormat(this->hdcNormal, format,
                                    sizeof(PIXELFORMATDESCRIPTOR), &pf->pfd);
    if (maxformat == 0) {
      DWORD dummy;
      SbString err = Win32::getWin32Err(dummy);
      SbString s = "DescribePixelFormat() failed with error message: ";
      s += err;
      SoDebugError::post("SoWinGLWidgetP::createGLContext", s.getString());
      goto panic;
    }
    
    pf->weight = SoWinGLWidgetP::weighPixelFormat(&pf->pfd,
                                                  (this->glModes & SO_GL_RGB) != 0,
                                                  (this->glModes & SO_GL_DOUBLE) != 0,
                                                  (this->glModes & SO_GL_ZBUFFER) != 0,
                                                  (this->glModes & SO_GL_STEREO) != 0,
                                                  this->accumulationenabled,
                                                  this->stencilenabled,
                                                  // FIXME: overlay support not
                                                  // implemented yet. 20020720 mortene.
                                                  FALSE,
                                                  this->alphachannelenabled);
    if (SoWinGLWidgetP::debugGLContextCreation()) {
      SoWinGLWidgetP::dumpPixelFormat(this->hdcNormal, format);
      SoDebugError::postInfo("SoWinGLWidgetP::createGLContext",
                             "weight==%f\n", pf->weight);
    }
    if (format == overrideformat) {
      pf->override = 1;
    }
    pflist.append(pf);
    format++;
  } while (format <= maxformat);
  
  if (pflist.getLength() == 0) { goto panic; }
  
  // sort descriptors based on descending weight
  qsort((void*) pflist.getArrayPtr(), pflist.getLength(), sizeof(void*),
        compare_pixel_formats);
  
  if (SoWinGLWidgetP::debugGLContextCreation()) {
    SoDebugError::postInfo("SoWinGLWidgetP::createGLContext",
                           "bestformat==%d, maxweight==%f",
                           pflist[0]->format, pflist[0]->weight);
  }

  if (!SoWinGLWidgetP::glWidgetTmpAtom) {
    WNDCLASS wc;
    wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc    = DefWindowProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = GetModuleHandle(NULL);
    wc.hIcon          = NULL;
    wc.hCursor        = NULL;
    wc.hbrBackground  = NULL;
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = "sowin_glwidget_temp";
    
    SoWinGLWidgetP::glWidgetTmpAtom = Win32::RegisterClass(&wc);
  }

  // Get the width and height from the supplied window handle and use
  // this when creating dummy windows in the loop below.

  // FIXME: After SoWin v1.2 has been released, use width and height
  // instead as 10, 10 when creating the dummy windows. 
  // 2004-01-08 thammer.

  RECT rect;
  Win32::GetWindowRect(window, &rect);
  int width, height;
  width = rect.right - rect.left;
  height = rect.bottom - rect.top;

  if ( (width < 0) || (height < 0) ) {
    SoDebugError::postWarning("SoWinGLWidgetP::createGLContext", 
      "Width or height (%d, %d) for the supplied windows handle [0x%lX] "
      "was < 0. This could lead to problems with selecting "
      "the optimal pixel format for the GL context.", width, height, 
      window);
  }
  
  int desktopwidth, desktopheight;
  Win32::GetWindowRect(GetDesktopWindow(), &rect);
  desktopwidth = rect.right - rect.left;
  desktopheight = rect.bottom - rect.top;
  
  if ( (width > desktopwidth) || (height > desktopheight) ) {
    SoDebugError::postWarning("SoWinGLWidgetP::createGLContext", 
      "Width or height (%d, %d) for the supplied windows handle [0x%lX] "
      "was larger than the width or height of the desktop (%d, %d). "
      "This could lead to problems with selecting "
      "the optimal pixel format for the GL context.", width, height, 
      window, desktopwidth, desktopheight);
  }

  i = 0;
  do {
    pf = pflist[i];
 
    HWND tempwindow;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    if (!(tempwindow = CreateWindow(
                                    "sowin_glwidget_temp",   /* class name */
                                    "sowin_glwidget_temp",   /* window title */
                                    0,               /* selected window style */
                                    0, 0,            /* window position */
                                    10,              /* dummy */
                                    10,              /* dummy */
                                    NULL,            /* no parent window */
                                    NULL,            /* no menu */
                                    hInstance,       /* Instance */
                                    NULL))) {        /* don't pass anything to WM_CREATE */
      DWORD dummy;
      SbString err = Win32::getWin32Err(dummy);
      SbString s = "Could not create temporary window, ";
      s += "as CreateWindow failed with error message: ";
      s += err;
      SoDebugError::postWarning("SoWinGLWidgetP::createGLContext", s.getString());
      i=pflist.getLength();
      break;
    }
    
    HDC tempdc = GetDC(tempwindow);
    if (tempdc == NULL) {
      DWORD dummy;
      SbString err = Win32::getWin32Err(dummy);
      SbString s = "Could not get device context for temporary window, ";
      s += "as GetDC failed with error message: ";
      s += err;
      SoDebugError::postWarning("SoWinGLWidgetP::createGLContext", s.getString());
      i=pflist.getLength();
      break;
    }
    
    if (SetPixelFormat(tempdc, pf->format, &pf->pfd)) {
      this->ctxNormal = wglCreateContext(tempdc);
      if (this->ctxNormal != NULL) {
        // test if we can make it current
        if (!SoWinGLWidgetP::wglMakeCurrent(tempdc, this->ctxNormal) ||
            !SoWinGLWidgetP::wglMakeCurrent(NULL, NULL)) {
          BOOL r = wglDeleteContext(this->ctxNormal);
          assert(r && "wglDeleteContext() failed -- investigate");
          this->ctxNormal = NULL;
          
          SoDebugError::postInfo("SoWinGLWidgetP::createGLContext",
                                 "Failed to make context current for format == %d",
                                 pf->format);
        }
        else {
          foundone = TRUE;
        }
      }
      else if (SoWinGLWidgetP::debugGLContextCreation()) {
        SoDebugError::postInfo("SoWinGLWidgetP::createGLContext",
                               "Failed to create context for format == %d",
                               pf->format);
      }
    }
    else if (SoWinGLWidgetP::debugGLContextCreation()) {
      SoDebugError::postInfo("SoWinGLWidgetP::createGLContext",
                             "Failed to set pixel format for format == %d",
                             pf->format);
    }
    // delete the test context before looping or exiting to avoid
    // memory leaks.
    if (this->ctxNormal) {
      BOOL r = wglDeleteContext(this->ctxNormal);
      assert(r && "wglDeleteContext() failed -- investigate");
      this->ctxNormal = NULL;
    }
    Win32::ReleaseDC(tempwindow, tempdc);
    Win32::DestroyWindow(tempwindow);
    
    if (!foundone) i++;
  } while (i < pflist.getLength() && !foundone);
  
  if (!foundone) { goto panic; }

  // We found a format. Now set this up for this->hdcNormal (the main window)
  if (SetPixelFormat(this->hdcNormal, pf->format, &pf->pfd)) {
    this->ctxNormal = wglCreateContext(this->hdcNormal);
    if (this->ctxNormal != NULL) {
      // test if we can make it current
      if (!SoWinGLWidgetP::wglMakeCurrent(this->hdcNormal, this->ctxNormal) ||
          !SoWinGLWidgetP::wglMakeCurrent(NULL, NULL)) {
        BOOL r = wglDeleteContext(this->ctxNormal);
        assert(r && "wglDeleteContext() failed -- investigate");
        this->ctxNormal = NULL;
          
        SoDebugError::post("SoWinGLWidgetP::createGLContext",
                           "Failed to make context current for format == %d (for main window).",
                           pf->format);
        mainwinerror = TRUE;
      }
    }
    else {
      SoDebugError::post("SoWinGLWidgetP::createGLContext",
                         "Failed to create context for format == %d (for main window).",
                         pf->format);
      mainwinerror = TRUE;
    }
  }
  else {
    SoDebugError::post("SoWinGLWidgetP::createGLContext",
                       "Failed to set pixel format for format == %d (for main window).",
                       pf->format);
    mainwinerror = TRUE;
  }

  if (mainwinerror) {
    RECT rect;
    Win32::GetWindowRect(window, &rect);
    int width, height;
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    SoDebugError::post("SoWinGLWidgetP::createGLContext",
      " The previous error occured when using the windows handle [0x%lX] that "
      "was supplied "
      "to the createGLContext function (by way of the SoWinGLWidget::buildWidget "
      "function). "
      "This handle was created by the application "
      "programmer and not by SoWin. The createGLContext function tried to perform "
      "the same function call for "
      "a temporary (dummy) window handle, without any problems. The cause of the "
      "problem is most likely that there is something about the supplied windows "
      "handle that the function that failed didn't like. One such thing could be "
      "the window size. The window size for the supplied window handle is (%ld, %ld). "
      "To resolve this problem, please investigate the parts of your code that "
      "creates and "
      "in any way manipulates the windows handle before it is sent to createGLContext. "
      "Systems In Motion would like to hear about this problem, so we would "
      "appreciate if you would send a brief email to coin-support@sim.no "
      "including the window size listed above, your OS version, the type of "
      "graphics card and driver version, and any other info that you think is "
      "important.", window, width, height);
    goto panic;
  }

  assert(i < pflist.getLength());
  pf = pflist[i];
  
  (void)memcpy(&this->pfdNormal, &pf->pfd, sizeof(PIXELFORMATDESCRIPTOR));
  
  // share context
  share = (SoWinGLWidget *)SoAny::si()->getSharedGLContext(NULL, NULL);
  
  if (share != NULL) {
    BOOL ok = wglShareLists(PRIVATE(share)->ctxNormal, this->ctxNormal);
    // FIXME: how should we properly react to ok==FALSE?
    // 20010920 mortene.
  }
  SoAny::si()->registerGLContext((void *) PUBLIC(this), NULL, NULL);
  
  // FIXME: what's this good for -- first setting then unsetting?
  // 20010924 mortene.
  if (!SoWinGLWidgetP::wglMakeCurrent(this->hdcNormal, this->ctxNormal) ||
      !SoWinGLWidgetP::wglMakeCurrent(NULL, NULL)) {
    SoDebugError::post("SoWinGLWidgetP::createGLContext",
                       "Couldn't make the picked GL context current! "
                       "Something is seriously wrong on this system!");
    goto panic;
  }

  // Sets up the app-programmer visible format flags from what kind of
  // canvas we actually got.
  {
    this->glModes = 0;
    this->glModes |= (this->pfdNormal.iPixelType == PFD_TYPE_RGBA) ? SO_GL_RGB : 0;
    this->glModes |= (this->pfdNormal.dwFlags & PFD_DOUBLEBUFFER) ? SO_GL_DOUBLE : 0;
    this->glModes |= (this->pfdNormal.cDepthBits > 0) ? SO_GL_ZBUFFER : 0;
    this->glModes |= (this->pfdNormal.dwFlags & PFD_STEREO) ? SO_GL_STEREO : 0;
    this->accumulationenabled = (this->pfdNormal.cAccumBits > 0);
    this->stencilenabled = (this->pfdNormal.cStencilBits > 0);
    // FIXME: check for overlay planes when support for this is
    // implemented. 20020719 mortene.
  }
  
#if 0 // temporary disabled because overlay planes is not supported yet
  // create overlay
  if (overlay) {
    this->ctxOverlay = wglCreateLayerContext(this->hdcOverlay, 1);
    // FIXME: set overlay plane. mariusbu 20010801.
  }
#endif // tmp disabled
  
  for (i = 0; i < pflist.getLength(); i++) {
    delete pflist[i];
  }
  return;

panic:
  // FIXME: clean up as good as possible, in case the application has
  // registered a "fatal error" handler callback and is able to run
  // without the functionality provided by SoWin.
  //
  // Note that this must be done _before_ calling
  // SoAny::invokeFatalErrorHandler(), since the fatal error handler
  // might throw an exception -- in which case it will not return by
  // using our stack address.
  //
  // 20011014 mortene.

  // FIXME: should provide more details about the error condition, if
  // possible. Could for instance use the GL vendor and / or version
  // and / or renderer string to smoke out the exact ATI driver known
  // to cause problems in accelerated mode (when DirectX is in
  // non-accelerated mode at the same time) -- as reported by Alan
  // Walford of Eos. 20011014 mortene.
  SbString s = "Could not find any supported OpenGL mode on your system.";
  if (!SoAny::si()->invokeFatalErrorHandler(s, SoWin::NO_OPENGL_CANVAS)) {
    // FIXME: this will actually cause the application to exit before
    // the error dialog has been shown. 20011123 mortene.
    exit(1);
  }
  return;
}

LRESULT
SoWinGLWidgetP::onCreate(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  int orgmodes = this->glModes;
  this->createGLContext(window);
  if (SoWinGLWidgetP::debugGLContextCreation() && (orgmodes != this->glModes)) {
    SoDebugError::postWarning("SoWinGLWidgetP::onCreate",
                              "wanted glModes==0x%x, got 0x%x",
                              orgmodes, this->glModes);
  }

  if ( PUBLIC(this)->isStealFocus() )
    (void)Win32::SetFocus(window);
  PUBLIC(this)->widgetChanged(window);
  return 0;
}

LRESULT
SoWinGLWidgetP::onPaint(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  // I believe the BeginPaint() / EndPaint() pair of calls are here
  // because that is how the window with the WM_PAINT event is
  // supposed to be told it is has been handled. I couldn't find a
  // definitive answer to this in the relevant MSDN documentation,
  // though, but that's what it looks like from their examples etc.
  //
  // --mortene.
  PAINTSTRUCT ps;
  HDC dc = Win32::BeginPaint(window, &ps);
  assert((dc == this->hdcNormal) && "incorrect BeginPaint() return value");

  // First time a paint event came along for this window? If so,
  // initialize what needs initializing in the OpenGL context.
  if (! this->glRealized) {
    this->glRealized = TRUE;
    PUBLIC(this)->initGraphic();
  }

  // The virtual glScheduleRedraw() passes on the information that an
  // expose event has happened, down to the subclasses.
  if (! PUBLIC(this)->glScheduleRedraw()) {
    SoDebugError::postWarning("SoWinGLWidgetP::onPaint",
                              "glScheduleRedraw() not implemented in "
                              "subclass -- this may cause trouble");
    // The original SGI InventorXt documents glScheduleRedraw() to be
    // handled like this: if a subclass didn't pick up the event, call
    // redraw() directly. In SoWin, SoWinRenderArea will invoke
    // scheduleRedraw() from within the overridden glScheduleRedraw().
    PUBLIC(this)->redraw();
  }

  Win32::EndPaint(window, &ps);
  return 0;
}

void
SoWinGLWidgetP::destroyWindow(HWND window)
{
  SoAny::si()->unregisterGLContext((void *) PUBLIC(this));

  // Release context. wglDeleteContext() will automatically make the
  // context non-current if it was current.
  const BOOL r = wglDeleteContext(this->ctxNormal);
  assert(r && "wglDeleteContext() failed -- investigate");
  this->ctxNormal = NULL;

  Win32::ReleaseDC(window, this->hdcNormal);
  this->hdcNormal = NULL;

  // FIXME: Overlay not supported. mariusbu 20010808.
}

// Wrap wglMakeCurrent() for convenience with regard to verbose
// warning output when it fails -- which it really shouldn't.  :-/
//
// The wglMakeCurrent(hdc, hglrc) call *has* been observed to fail.
BOOL
SoWinGLWidgetP::wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
  // FIXME: workaround for problems with wglMakeCurrent(NULL, NULL)
  // with the Microsoft GDI Generic 1.1.0 driver. 20010924 mortene.
  //
  // FIXME UPDATE: it seems necessary to be able to do
  // wglMakeCurrent(NULL, NULL) anyway, or there will be too little
  // updates. 20010924 mortene.
  //
  // if (hglrc == NULL) { glFlush(); return TRUE; }
  if (hglrc == NULL) { ::wglMakeCurrent(NULL, NULL); return TRUE; }

  if (::wglMakeCurrent(hdc, hglrc)) { return TRUE; }

  DWORD dummy;
  SbString err = Win32::getWin32Err(dummy);
  SbString s = "The rendering context <%p, %p>, could not be made current, ";
  s += "as wglMakeCurrent() failed with error message: ";
  s += err;
  SoDebugError::postWarning("SoWinGLWidgetP::wglMakeCurrent", s.getString(),
                            hdc, hglrc);
  return FALSE;
}

// Return a flag indicating whether or not OpenGL rendering is
// happening directly from the CPU(s) to the GPU(s), ie on a local
// display. With GLX on X11, it is possible to do remote rendering.
SbBool
SoWinGLWidgetP::isDirectRendering(void)
{
  return TRUE;
}

#endif // ! DOXYGEN_SKIP_THIS

#undef PRIVATE
#undef PUBLIC
