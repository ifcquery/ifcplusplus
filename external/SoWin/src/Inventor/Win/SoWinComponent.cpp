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

// Class documentation in common/SoGuiComponentCommon.cpp.in.

// *************************************************************************

#include <assert.h>
#include <string.h>
#include <windows.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoLists.h>
#include <Inventor/SbDict.h>
#include <Inventor/threads/SbStorage.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinComponent.h>
#include <Inventor/Win/SoWinComponentP.h>
#include <Inventor/Win/SoWinGLWidget.h>
#include <Inventor/Win/SoWinRenderArea.h>
#include <Inventor/Win/viewers/SoWinViewer.h>
#include <Inventor/Win/viewers/SoWinFullViewer.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/Win/SoWinCursor.h>
#include <Inventor/Win/SoAny.h>
#include <sowindefs.h> // SOWIN_STUB
#include <Inventor/Win/Win32API.h>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

SOWIN_OBJECT_ABSTRACT_SOURCE(SoWinComponent);

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// The private data and code for the SoWinComponent.

SoWinComponentP::SoWinComponentP(SoWinComponent * o)
  : SoGuiComponentP(o)
{
  this->hascursor = FALSE;
  this->parent = NULL;
}

SoWinComponentP::~SoWinComponentP()
{
  if (SoGuiComponentP::nrofcomponents == 1 && SoWinComponentP::wndClassAtom) { // if wndclass is registered
    Win32::UnregisterClass("Component Widget", NULL);
    SoWinComponentP::wndClassAtom = NULL;
  }

  if (SoGuiComponentP::nrofcomponents == 1) {
    delete cursordict;
    cursordict = NULL;
  }
}

void
SoWinComponentP::commonEventHandler(UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message) {
  case WM_SIZE:
#if SOWIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoWinComponentP::commonEventHandler",
                           "WM_SIZE==<%d, %d>",
                           LOWORD(lparam), HIWORD(lparam));
#endif // debug
    PUBLIC(this)->sizeChanged(SbVec2s(LOWORD(lparam), HIWORD(lparam)));
    break;

  case WM_CLOSE:
    PUBLIC(this)->hide();
    if (this->closeCB) { this->closeCB(this->closeCBdata, PUBLIC(this)); }
    break;

  case WM_SHOWWINDOW:
    if (!this->realized) {
      PUBLIC(this)->afterRealizeHook();
      this->realized = TRUE;
    }
    break;

  case WM_SETCURSOR:
    // this->cursor can be NULL when the virtual
    // setComponentCursor() method is overridden in subclasses.
    if (this->hascursor) {
      SoWinComponent::setWidgetCursor(PUBLIC(this)->getWidget(),
                                      this->currcursor);
    }
    break;
  }
}

// Message hook
LRESULT CALLBACK
SoWinComponentP::systemEventFilter(int code, WPARAM wparam, LPARAM lparam)
{
  CWPSTRUCT * msg = (CWPSTRUCT *)lparam;
  void * comp;
  if (SoWinComponentP::embeddedparents->find((SbDict::Key)msg->hwnd, comp) &&
      // as per the API doc on CallWndProc(): only process msg if code>=0
      (code >= 0)) {
    SoWinComponent * component = (SoWinComponent *)comp;
    PRIVATE(component)->commonEventHandler(msg->message,
                                           msg->wParam, msg->lParam);
  }
  HHOOK * hookhandle = (HHOOK *) SoWinComponentP::hookhandle->get();
  return CallNextHookEx(*hookhandle, code, wparam, lparam);
}

ATOM SoWinComponentP::wndClassAtom = 0;
SbDict * SoWinComponentP::cursordict = NULL;
SbStorage * SoWinComponentP::hookhandle = NULL;
SbDict * SoWinComponentP::embeddedparents = NULL;

LRESULT CALLBACK
SoWinComponentP::frameWindowHandler(HWND window, UINT message,
                                    WPARAM wparam, LPARAM lparam)
{
  SoWinComponent * component = (SoWinComponent *)
    Win32::GetWindowLongPtr(window, GWLP_USERDATA);

  if (component) {
    PRIVATE(component)->commonEventHandler(message, wparam, lparam);
  }

  return DefWindowProc(window, message, wparam, lparam);
}

HWND
SoWinComponentP::buildFormWidget(HWND parent)
{
  // When this method is called, the component is *not*
  // embedded.

  if (! SoWinComponentP::wndClassAtom) {
    HBRUSH brush = (HBRUSH) GetSysColorBrush(COLOR_BTNFACE);

    WNDCLASS windowclass;
    windowclass.lpszClassName = "Component Widget";
    windowclass.hInstance = NULL;
    windowclass.lpfnWndProc = SoWinComponentP::frameWindowHandler;
    windowclass.style = CS_OWNDC;
    windowclass.lpszMenuName = NULL;
    windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowclass.hCursor = Win32::LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = brush;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = sizeof(LONG_PTR);

    SoWinComponentP::wndClassAtom = Win32::RegisterClass(&windowclass);
  }

  HWND parentwidget = Win32::CreateWindow_("Component Widget",
                                           PUBLIC(this)->getTitle(),
                                           WS_OVERLAPPEDWINDOW |
                                           WS_VISIBLE |
                                           WS_CLIPSIBLINGS |
                                           WS_CLIPCHILDREN,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           500, 500,
                                           parent,
                                           NULL,
                                           NULL,
                                           NULL);

  (void)Win32::SetWindowLongPtr(parentwidget, GWLP_USERDATA, (LONG_PTR)PUBLIC(this));

  assert(IsWindow(parentwidget));
  return parentwidget;
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::initClasses(void)
{
  SoWinComponent::initClass();
  SoWinGLWidget::initClass();
  SoWinRenderArea::initClass();
  SoWinViewer::initClass();
  SoWinFullViewer::initClass();
  SoWinExaminerViewer::initClass();
  SoWinPlaneViewer::initClass();
}

// *************************************************************************

static void
hookhandle_constructor(void * closure)
{
  HHOOK * hookhandle = (HHOOK *) closure;
  *hookhandle = NULL;
}

static void
hookhandle_destructor(void *)
{

}

// Documented in common/SoGuiComponentCommon.cpp.in.
SoWinComponent::SoWinComponent(HWND const parent,
                               const char * const name,
                               const SbBool embed)
{
  PRIVATE(this) = new SoWinComponentP(this);
  PRIVATE(this)->realized = FALSE;

  PRIVATE(this)->closeCB = NULL;
  PRIVATE(this)->closeCBdata = NULL;
  PRIVATE(this)->visibilitychangeCBs = new SbPList;

  PRIVATE(this)->widget = NULL;
  PRIVATE(this)->embedded = embed;

  SoAny::si()->addInternalFatalErrorHandler(SoWinComponentP::fatalerrorHandler,
                                            PRIVATE(this));

  if (SoWinComponentP::embeddedparents == NULL) {
    SoWinComponentP::embeddedparents = new SbDict;
  }

  if (IsWindow(parent) && embed) {
    PRIVATE(this)->parent = parent;
    SoWinComponentP::embeddedparents->enter((SbDict::Key)parent, this);
  }
  else {
    PRIVATE(this)->parent = PRIVATE(this)->buildFormWidget(parent);
  }
  
  if (SoWinComponentP::hookhandle == NULL) {
    SoWinComponentP::hookhandle = new SbStorage(sizeof(HHOOK), 
                                                hookhandle_constructor,
                                                hookhandle_destructor);
  }

  HHOOK * hookhandle = (HHOOK *) SoWinComponentP::hookhandle->get();
  if (!(*hookhandle)) {
    *hookhandle = Win32::SetWindowsHookEx(WH_CALLWNDPROC,
                                          (HOOKPROC)SoWinComponentP::systemEventFilter,
                                          NULL, GetCurrentThreadId());
  }

  if (name) {
    PRIVATE(this)->widgetname = name;
    this->setTitle(name);
  }
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SoWinComponent::~SoWinComponent()
{
  if (PRIVATE(this)->widget) { this->unregisterWidget(PRIVATE(this)->widget); }

  delete PRIVATE(this)->visibilitychangeCBs;

  (void)SoWinComponentP::embeddedparents->remove((SbDict::Key)this->getParentWidget());

  // Clean up static data if this is the last component.
  if (SoGuiComponentP::nrofcomponents == 1) {
    delete SoWinComponentP::embeddedparents;
    SoWinComponentP::embeddedparents = NULL;

    HHOOK * hookhandle = (HHOOK *) SoWinComponentP::hookhandle->get();
    if (*hookhandle) {
      Win32::UnhookWindowsHookEx(*hookhandle);
    }

    delete SoWinComponentP::hookhandle;
    SoWinComponentP::hookhandle = NULL;
  }

  PRIVATE(this)->cleanupWin32References();
  delete PRIVATE(this);
}

#ifndef DOXYGEN_SKIP_THIS

// We were having a specific problem which triggered the need for a
// "fatal error cleanup" routine. It was reported by Alan Walford of
// Eos. It is a complex issue where a bug could be triggered like
// this:
//
// 0) the construction of a SoWinGLWidget-derived instance (like for
// instance a renderarea) fails due to lack of or faulty
// OpenGL-support -- or due to any other fatal error condition
//
// 1) SoWinGLWidget::buildGLWidget() (for instance) then calls
// SoAny::invokeFatalErrorHandler()
//
// 2) the app-programmer's registered fatal error handler throws a C++
// exception which is caught "higher up", so the construction of the
// SoWinGLWidget-derived instance is never completed, and neither has
// it been destructed -- it is therefore in an invalid state, which
// goes undetected from within SoWin
//
// 3) in the process of cleaning up, the application client code
// causes a message to be sent to the SoWinGLWidget-instance's window,
// which triggers the SoWinComponent::frameWindowHandler() function
//
// 4) in that method, operations on the SoWinComponent instance is
// done -- which of course causes a crash, since it was never properly
// initialized
//
// This is a hard bug to detect and solve for the application
// programmers, so what we do is to clean up anything that could cause
// crash bugs because of Win32 having references into any of our data.
//
// mortene (thanks to Alan for explaining the problem).
void
SoWinComponentP::fatalerrorHandler(void * userdata)
{
  SoWinComponentP * that = (SoWinComponentP *)userdata;
  that->cleanupWin32References();
}

void
SoWinComponentP::cleanupWin32References(void)
{
  if (IsWindow(this->parent) && !this->embedded) { Win32::DestroyWindow(this->parent); }
}

#endif // !DOXYGEN_SKIP_THIS

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::show(void)
{
  (void)ShowWindow(PRIVATE(this)->parent, SW_SHOW);
  Win32::InvalidateRect(PRIVATE(this)->parent, NULL, FALSE);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::hide(void)
{
  (void)ShowWindow(PRIVATE(this)->parent, SW_HIDE);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoWinComponent::setFullScreen(const SbBool enable)
{
  SoWinComponentP::FullscreenData * data = &(PRIVATE(this)->fullscreendata);
  if (!!enable == !!data->on) { return TRUE; }
  data->on = enable;

  // FIXME: this check to see if we're a top-level component doesn't
  // match the one in SoWinComponent::setTitle(). Which one is better?
  // 20020824 mortene.
  HWND hwnd = this->getParentWidget();
  if (hwnd != SoWin::getShellWidget(this->getWidget())) {
    return FALSE;
  }

  // FIXME: The FullscreenData struct is now obsolete, since we dont need to
  // store the old size, position and style. We now only need to store if we are
  // in fullscreen or not. 20030806 frodo.
  if (enable) {
    // Go to fullscreen. We must change the style to get rid of the title bar and the border
    Win32::SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    // FIXME: no error handling, argh. should wrap in the Win32 class.
    // -mortene.
    ShowWindow(hwnd,SW_MAXIMIZE);
  }
  else {
    // Restore old window position and size.
    // FIXME: no error handling, argh. should wrap in the Win32 class.
    // -mortene.
    ShowWindow(hwnd,SW_RESTORE);
    // Restore old window style
    Win32::SetWindowLongPtr(hwnd, GWL_STYLE,
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE |
                            WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    
    // Certain window data such as style is cached, so we must call
    // SetWindowPos() in order for the changes to take effect, 20030806 frodo.
    RECT rect;
    Win32::GetWindowRect(hwnd, &rect);
    // FIXME: no error handling, argh. should wrap in the Win32 class.
    // -mortene.
    SetWindowPos(hwnd,HWND_TOPMOST, rect.left, rect.top,
                                    rect.right - rect.left, rect.bottom - rect.top,
                                    SWP_FRAMECHANGED);
  }

  return TRUE;
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoWinComponent::isFullScreen(void) const
{
  return PRIVATE(this)->fullscreendata.on;
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoWinComponent::isVisible(void)
{
  // FIXME: no error handling, argh. should wrap in the Win32 class.
  // -mortene.
  return IsWindowVisible(PRIVATE(this)->widget);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
HWND
SoWinComponent::getWidget(void) const
{
  return PRIVATE(this)->widget;
}

// Documented in common/SoGuiComponentCommon.cpp.in.
HWND
SoWinComponent::getBaseWidget(void) const
{
  return this->getWidget();
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoWinComponent::isTopLevelShell(void) const
{
  return (PRIVATE(this)->embedded ? FALSE : TRUE);
  // FIXME: compare with SoWin::getTopLevelWidget() ? mariusbu 20010806.
}

// Documented in common/SoGuiComponentCommon.cpp.in.
HWND
SoWinComponent::getParentWidget(void) const
{
  return PRIVATE(this)->parent;
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setSize(const SbVec2s size)
{
  this->setFullScreen(FALSE);

  UINT flags = SWP_NOMOVE | SWP_NOZORDER; // do redraw
  Win32::SetWindowPos(this->getParentWidget(), NULL, 0, 0,
                      size[0], size[1], flags);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
SbVec2s
SoWinComponent::getSize(void) const
{
  RECT rect;
  Win32::GetWindowRect(this->getParentWidget(), & rect);
  return SbVec2s((short)(rect.right - rect.left),
                 (short)(rect.bottom - rect.top));
}

// Documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoWinComponent::getWidgetName(void) const
{
  return PRIVATE(this)->widgetname.getLength() ?
    PRIVATE(this)->widgetname.getString() : this->getDefaultWidgetName();
}

// Documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoWinComponent::getClassName(void) const
{
  return PRIVATE(this)->classname.getString();
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setTitle(const char * const title)
{
  PRIVATE(this)->title = title ? title : "";

  // Check if the internal Win32 Window for the SoWinComponent has
  // been built, as this method may be invoked before that has
  // happened.
  if (PRIVATE(this)->widget == NULL) { return; }

  // FIXME: this check to see if we're a top-level component doesn't
  // match the one in SoWinComponent::setFullscreen(). Which one is
  // better? 20020824 mortene.
  HWND shellWidget = SoWin::getShellWidget(this->getWidget());
  if (shellWidget == SoWin::getTopLevelWidget() ||
      shellWidget == this->getParentWidget()) {
    Win32::SetWindowText(shellWidget, PRIVATE(this)->title.getString());
  }


}

// Documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoWinComponent::getTitle(void) const
{
  return (PRIVATE(this)->title.getLength() > 0 ?
          PRIVATE(this)->title.getString() : this->getDefaultTitle());
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setIconTitle(const char * const title)
{
  // FIXME: seems to easy.. is this really correct? 20011012 mortene.
  this->setTitle(title);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoWinComponent::getIconTitle(void) const
{
  // FIXME: seems to easy.. is this really correct? 20011012 mortene.
  return this->getTitle();
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setWindowCloseCallback(SoWinComponentCB * const func,
                                       void * const data)
{
  // FIXME: generic code, can we collect from all So* toolkits? The
  // implementation in SoXt is more complex (and better?). 20030411 mortene.

  PRIVATE(this)->closeCB = func;
  PRIVATE(this)->closeCBdata = data;
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setBaseWidget(HWND widget)
{
  assert(IsWindow(widget));

  if (PRIVATE(this)->widget) { this->unregisterWidget(PRIVATE(this)->widget); }
  PRIVATE(this)->widget = widget;
  this->registerWidget(PRIVATE(this)->widget);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setClassName(const char * const name)
{
  // FIXME: generic code, can we collect from all So* toolkits?
  // 20030411 mortene.

  if (name)
    PRIVATE(this)->classname = name;
  else
    PRIVATE(this)->classname = "";
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::sizeChanged(const SbVec2s & size)
{
  // The default implementation does nothing.
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::afterRealizeHook(void) // virtual
{
  // Set shell widget title, in case it was attempted set earlier,
  // before the Win32 window of the SoWinComponent was built.
  const SbString t = PRIVATE(this)->title.getLength() == 0 ?
    this->getDefaultTitle() : PRIVATE(this)->title;
  this->setTitle(t.getString());
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::addVisibilityChangeCallback(SoWinComponentVisibilityCB * const func,
                                            void * const user)
{
  // FIXME: generic code, can we collect from all So* toolkits?
  // 20030411 mortene.

  void ** combo = new void * [2];
  combo[0] = (void*)func;
  combo[1] = user;
  PRIVATE(this)->visibilitychangeCBs->append(combo);
  //FIXME: the functions are never called. mariusbu 20010824.
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::removeVisibilityChangeCallback(SoWinComponentVisibilityCB * const func,
                                               void * const user)
{
  // FIXME: generic code, can we collect from all So* toolkits?
  // 20030411 mortene.

  void ** combo;
  for (int i = 0; i < PRIVATE(this)->visibilitychangeCBs->getLength(); i++) {
    combo = (void **) PRIVATE(this)->visibilitychangeCBs->get(i);
    if ((combo[0] == func) &&  (combo[1] == user)) {
      PRIVATE(this)->visibilitychangeCBs->remove(i);
      delete combo;
      return;
    }
  }
}

#ifndef DOXYGEN_SKIP_THIS

static inline unsigned char
reverse_byte(unsigned char b)
{
  b = ((b & 0xf0) >> 4) | ((b & 0x0f) << 4);
  b = ((b & 0xcc) >> 2) | ((b & 0x33) << 2);
  b = ((b & 0xaa) >> 1) | ((b & 0x55) << 1);
  return b;
}

// Converts from the common generic cursor format to a Win32 HCURSOR
// instance.
HCURSOR
SoWinComponentP::getNativeCursor(const SoWinCursor::CustomCursor * cc)
{
  if (SoWinComponentP::cursordict == NULL) { // first call, initialize
    SoWinComponentP::cursordict = new SbDict; // FIXME: mem leak. 20011121 mortene.
  }

  void * qc;
  SbBool b = SoWinComponentP::cursordict->find((SbDict::Key)cc, qc);
  if (b) { return (HCURSOR)qc; }

  const short cursorwidth = GetSystemMetrics(SM_CXCURSOR);
  const short cursorheight = GetSystemMetrics(SM_CYCURSOR);
  assert(cursorwidth!=0 && cursorheight!=0 &&
         "GetSystemMetrics() failed -- investigate");
  const int cursorbytewidth = (cursorwidth + 7) / 8;
  const int cursorbytes = cursorbytewidth * cursorheight;

  unsigned char * ANDbitmap = new unsigned char[cursorbytes];
  unsigned char * XORbitmap = new unsigned char[cursorbytes];
  (void)memset(ANDbitmap, 0xff, cursorbytes);
  (void)memset(XORbitmap, 0x00, cursorbytes);

  if (SOWIN_DEBUG) {
    if ((cc->dim[0] > cursorwidth) || (cc->dim[1] > cursorheight)) {
      SoDebugError::postWarning("SoWinComponentP::getNativeCursor",
                                "internal cursor bitmap too large: <%d, %d> "
                                "(max dimensions are <%d, %d>)",
                                cc->dim[0], cc->dim[1],
                                cursorwidth, cursorheight);
      // Only a warning is necessary, as below we just crop the
      // cursor.
    }
  }

  // The input data for CreateCursor() should follow this format:
  //
  //  AND | XOR | Display
  // -----+-----+--------
  //   0  |  0  | Black
  //   0  |  1  | White
  //   1  |  0  | Screen (transparency)
  //   1  |  1  | Reverse screen
  //
  // ..so we need to do some processing on the original data here.
  // (Also, the bits should be reversed versus the original format.)

  const int nativebytewidth = (cc->dim[0] + 7) / 8;
  for (int h=0; h < SoWinMin(cc->dim[1], cursorheight); h++) {
    for (int w=0; w < SoWinMin(nativebytewidth, cursorbytewidth); w++) {
      const int cursorpos = h * cursorbytewidth + w;
      const int nativepos = h * nativebytewidth + w;
      unsigned char b = reverse_byte(cc->bitmap[nativepos]);
      unsigned char m = reverse_byte(cc->mask[nativepos]);
      ANDbitmap[cursorpos] = ~(b | m);
      XORbitmap[cursorpos] = b ^ m;
    }
  }

  // FIXME: plug this memory leak by using DestroyCursor(). 20011126 mortene.
  HCURSOR c = Win32::CreateCursor(NULL,
                                  cc->hotspot[0], cc->hotspot[1],
                                  cursorwidth, cursorheight,
                                  ANDbitmap, XORbitmap);
  delete ANDbitmap;
  delete XORbitmap;

  SoWinComponentP::cursordict->enter((SbDict::Key)cc, c);
  return c;
}

#endif // !DOXYGEN_SKIP_THIS

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setComponentCursor(const SoWinCursor & cursor)
{
  PRIVATE(this)->currcursor = cursor;
  PRIVATE(this)->hascursor = TRUE;
  SoWinComponent::setWidgetCursor(this->getWidget(), cursor);
}

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoWinComponent::setWidgetCursor(HWND w, const SoWinCursor & cursor)
{
  if (cursor.getShape() == SoWinCursor::CUSTOM_BITMAP) {
    const SoWinCursor::CustomCursor * cc = &cursor.getCustomCursor();
    SetCursor(SoWinComponentP::getNativeCursor(cc));
  }
  else {
    switch (cursor.getShape()) {
    case SoWinCursor::DEFAULT:
      SetCursor(Win32::LoadCursor(NULL, IDC_ARROW));
      break;

    case SoWinCursor::BUSY:
      SetCursor(Win32::LoadCursor(NULL, IDC_WAIT));
      break;

    case SoWinCursor::CROSSHAIR:
      SetCursor(Win32::LoadCursor(NULL, IDC_CROSS));
      break;

    case SoWinCursor::UPARROW:
      SetCursor(Win32::LoadCursor(NULL, IDC_UPARROW));
      break;

    default:
      assert(FALSE && "unsupported cursor shape type");
      break;
    }
  }
}

#undef PRIVATE
#undef PUBLIC
