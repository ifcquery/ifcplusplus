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

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <Inventor/SbDict.h>

#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/Win32API.h>
#include <Inventor/Win/widgets/SoAnyThumbWheel.h>
#include <Inventor/Win/widgets/SoWinThumbWheel.h>
#include <sowindefs.h>

// *************************************************************************

ATOM SoWinThumbWheel::wheelWndClassAtom = NULL;
int SoWinThumbWheel::wheelWidgetCounter = 0;
SbDict * SoWinThumbWheel::hwnddict = NULL;

SoWinThumbWheel::SoWinThumbWheel(HWND parent,
                                 long id,
                                 int x,
                                 int y,
                                 const char * name)
{
  this->constructor(SoWinThumbWheel::Vertical, parent, id, x, y, name);
}

SoWinThumbWheel::SoWinThumbWheel(Orientation orientation,
                                 HWND parent,
                                 long id,
                                 int x,
                                 int y,
                                 const char * name)
{
  this->constructor(orientation, parent, id, x, y, name);
}

void
SoWinThumbWheel::constructor(Orientation orientation,
                             HWND parent, long id, int x, int y,
                             const char * name)
{
  SoWinThumbWheel::wheelWidgetCounter++;
  this->orient = orientation;
  this->state = SoWinThumbWheel::Idle;
  this->wheelValue = this->tempWheelValue = 0.0f;
  this->wheel = new SoAnyThumbWheel;
  this->wheel->setMovement(SoAnyThumbWheel::UNIFORM);
  this->wheel->setGraphicsByteOrder(SoAnyThumbWheel::ARGB);
  this->pixmaps = NULL;
  this->numPixmaps = 0;
  this->currentPixmap = -1;
  this->viewerCB = NULL;
  this->labelWindow = NULL;
  this->wheelWindow = NULL;

  RECT rect = { x, y, this->sizeHint().cx, this->sizeHint().cy };
  this->buildWidget(parent, rect, name);
  this->setId(id);

  if (SoWinThumbWheel::hwnddict == NULL) {
    SoWinThumbWheel::hwnddict = new SbDict;
  }

  const SbDict::Key key = (SbDict::Key)this->getWidget();
  SbBool isnewentry = SoWinThumbWheel::hwnddict->enter(key, this);
  assert(isnewentry);
}

SoWinThumbWheel::~SoWinThumbWheel(void)
{
  delete this->wheel;

  const SbDict::Key key = (SbDict::Key)this->getWidget();
  SbBool found = SoWinThumbWheel::hwnddict->remove(key);
  assert(found);

  if (this->pixmaps) {
    for (int i = 0; i < this->numPixmaps; i++)
      Win32::DeleteObject(this->pixmaps[i]);
    delete [] this->pixmaps;
  }
  if (IsWindow(this->wheelWindow))
    Win32::DestroyWindow(this->wheelWindow);
  if (IsWindow(this->labelWindow))
    Win32::DestroyWindow(this->labelWindow);

  SoWinThumbWheel::wheelWidgetCounter--;
  assert(SoWinThumbWheel::wheelWidgetCounter >= 0);
  if (SoWinThumbWheel::wheelWidgetCounter == 0) {
    if (SoWinThumbWheel::wheelWndClassAtom)
      Win32::UnregisterClass("ThumbWheel Widget", NULL);
    SoWinThumbWheel::wheelWndClassAtom = NULL;
    delete SoWinThumbWheel::hwnddict;
    SoWinThumbWheel::hwnddict = NULL;
  }
}

SoWinThumbWheel *
SoWinThumbWheel::getWheelFromHWND(HWND h)
{
  if (SoWinThumbWheel::hwnddict == NULL) { return NULL; }

  void * w;
  SbBool found = SoWinThumbWheel::hwnddict->find((SbDict::Key)h, w);
  return found ? ((SoWinThumbWheel *)w) : NULL;
}

SIZE
SoWinThumbWheel::sizeHint(void) const
{
  const int length = 118;
  int thick = 14;
  SIZE size;

  if (this->orient == SoWinThumbWheel::Horizontal) {
    size.cx = length;
    size.cy = thick;
    return size;
  }
  else {
    size.cx = thick;
    size.cy = length;
    return size;
  }
}

HWND
SoWinThumbWheel::getWidget(void)
{
  return this->wheelWindow;
}

void
SoWinThumbWheel::setId(long id)
{
  (void)Win32::SetWindowLongPtr(this->wheelWindow, GWL_ID, id);
}

long
SoWinThumbWheel::id(void) const
{
  return Win32::GetWindowLongPtr(this->wheelWindow, GWL_ID);
}

void
SoWinThumbWheel::setOrientation(Orientation orientation)
{
  this->orient = orientation;
}

SoWinThumbWheel::Orientation
SoWinThumbWheel::orientation(void) const
{
  return this->orient;
}

LRESULT CALLBACK
SoWinThumbWheel::onCreate(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onSize(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onPaint(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  PAINTSTRUCT ps;
  HDC hdc = Win32::BeginPaint(window, & ps);

  int w, d;
  if (this->orient == SoWinThumbWheel::Vertical) {
    w = this->width() - 2;
    d = this->height() - 2;
  } else {
    w = this->height() - 2;
    d = this->width() - 2;
  }

  // Handle resizing to too small dimensions gracefully.
  if ((d <= 0) || (w <= 0)) return 0;

  this->initWheel(d, w);

  int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                              (this->state == SoWinThumbWheel::Disabled) ?
                                              SoAnyThumbWheel::DISABLED : SoAnyThumbWheel::ENABLED);

  this->blitBitmap(this->pixmaps[pixmap], hdc, 0, 0, this->width() - 2, this->height() - 2);

  this->currentPixmap = pixmap;

  Win32::EndPaint(window, & ps);
  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onLButtonDown(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  if (this->state != SoWinThumbWheel::Idle)
    return 0;

  short x = LOWORD(lparam);
  short y = HIWORD(lparam);

  SetCapture(window);

  this->state = SoWinThumbWheel::Dragging;
  if (this->orient == SoWinThumbWheel::Vertical)
    this->mouseDownPos = y;
  else
    this->mouseDownPos = x;

  this->mouseLastPos = this->mouseDownPos;

  if (this->viewerCB) {
    this->viewerCB(START, this->wheelValue, this->userdataCB);
  }

  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onMouseMove(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  if (this->state != SoWinThumbWheel::Dragging)
    return 0;

  short x = LOWORD(lparam);
  short y = HIWORD(lparam);

  if (this->orient == SoWinThumbWheel::Vertical)
    this->mouseLastPos = y;
  else
    this->mouseLastPos = x;

  this->tempWheelValue =
    this->wheel->calculateValue(this->wheelValue,
                                this->mouseDownPos,
                                this->mouseLastPos - this->mouseDownPos);

#if SOWIN_DEBUG && 0 // debug
  SoDebugError::postInfo("onMouseMove",
                         "delta==%d, wheelValue==%f, tmpWheelValue==%f",
                         this->mouseLastPos - this->mouseDownPos,
                         this->wheelValue, this->tempWheelValue);
#endif // debug

  Win32::InvalidateRect(this->wheelWindow, NULL, FALSE);

  if (this->viewerCB) {
    this->viewerCB(MOVE, this->tempWheelValue, this->userdataCB);
  }

  this->setValue(this->tempWheelValue);

  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onLButtonUp(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  ReleaseCapture();
  if (this->state != SoWinThumbWheel::Dragging)
    return 0;

  this->wheelValue = this->tempWheelValue;
  this->mouseLastPos = this->mouseDownPos;
  this->state = SoWinThumbWheel::Idle;

  if (this->viewerCB) {
    this->viewerCB(END, this->wheelValue, this->userdataCB);
  }

  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::onDestroy(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  return 0;
}

LRESULT CALLBACK
SoWinThumbWheel::windowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  if (message == WM_CREATE) {
    CREATESTRUCT * createstruct;
    createstruct = (CREATESTRUCT *) lparam;

    (void)Win32::SetWindowLongPtr(window, 0,
                                  (LONG_PTR)(createstruct->lpCreateParams));

    SoWinThumbWheel * object = (SoWinThumbWheel *)(createstruct->lpCreateParams);
    return object->onCreate(window, message, wparam, lparam);
  }

  SoWinThumbWheel * object = (SoWinThumbWheel *)
    Win32::GetWindowLongPtr(window, 0);

  if (object && object->getWidget()) {

    switch (message)
      {
      case WM_SIZE:
        return object->onSize(window, message, wparam, lparam);

      case WM_PAINT:
        return object->onPaint(window, message, wparam, lparam);

      case WM_LBUTTONDOWN:
        return object->onLButtonDown(window, message, wparam, lparam);

      case WM_LBUTTONUP:
        return object->onLButtonUp(window, message, wparam, lparam);

      case WM_MOUSEMOVE:
        return object->onMouseMove(window, message, wparam, lparam);

      case WM_DESTROY:
        return object->onDestroy(window, message, wparam, lparam);
      }
  }
  return DefWindowProc(window, message, wparam, lparam);
}

int
SoWinThumbWheel::width(void)
{
  RECT rect;
  Win32::GetWindowRect(this->wheelWindow, & rect);
  return (rect.right - rect.left);

  //return this->sizeHint().cx;
}

int
SoWinThumbWheel::height(void)
{
  RECT rect;
  Win32::GetWindowRect(this->wheelWindow, & rect);
  return (rect.bottom - rect.top);

  //return this->sizeHint().cy;
}


void
SoWinThumbWheel::move(int x, int y, int width, int height)
{
  this->size(width, height);
  this->move(x, y);
}

void
SoWinThumbWheel::move(int x, int y)
{
  UINT flags = SWP_NOSIZE | SWP_NOZORDER;

  Win32::SetWindowPos(this->wheelWindow, NULL, x, y, 0, 0, flags);

  if (IsWindow(this->labelWindow)) {

    RECT rect;
    Win32::GetClientRect(this->labelWindow, & rect);

    if (this->orient == SoWinThumbWheel::Vertical) {
      Win32::SetWindowPos(this->labelWindow, NULL,
                          x + this->labelOffset.x,
                          y + this->labelOffset.y + this->height(),
                          0, 0, flags);
    }
    else {
      Win32::SetWindowPos(this->labelWindow, NULL,
                          x + this->labelOffset.x - rect.right,
                          y + this->labelOffset.y,
                          0, 0, flags);
    }
  }
}

void
SoWinThumbWheel::size(int width, int height)
{
  UINT flags = SWP_NOMOVE | SWP_NOZORDER;
  Win32::SetWindowPos(this->wheelWindow, NULL, 0, 0, width, height, flags);
  Win32::InvalidateRect(this->wheelWindow, NULL, FALSE);
  if (IsWindow(this->labelWindow))
    Win32::InvalidateRect(this->labelWindow, NULL, FALSE);
}

void
SoWinThumbWheel::show(void)
{
  (void)ShowWindow(this->wheelWindow, SW_SHOW);
  (void)ShowWindow(this->labelWindow, SW_SHOW);
}

void
SoWinThumbWheel::hide(void)
{
  (void)ShowWindow(this->wheelWindow, SW_HIDE);
  (void)ShowWindow(this->labelWindow, SW_HIDE);
}

void
SoWinThumbWheel::setCallback(ThumbWheelCB * func, void * userdata)
{
  this->viewerCB = func;
  this->userdataCB = userdata;
}

HWND
SoWinThumbWheel::buildWidget(HWND parent, RECT rect, const char * name)
{

  HMENU menu = NULL;
  LPSTR wndclassname = "ThumbWheel Widget";

  if (! SoWinThumbWheel::wheelWndClassAtom) {

    WNDCLASS windowclass;

    windowclass.lpszClassName = wndclassname;
    windowclass.hInstance = NULL;
    windowclass.lpfnWndProc = SoWinThumbWheel::windowProc;
    windowclass.style = CS_HREDRAW | CS_VREDRAW;
    windowclass.lpszMenuName = NULL;
    windowclass.hIcon = NULL;
    windowclass.hCursor = Win32::LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = NULL;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = sizeof(LONG_PTR);

    SoWinThumbWheel::wheelWndClassAtom = Win32::RegisterClass(& windowclass);

  }

  this->wheelWindow = Win32::CreateWindow_(wndclassname,
                                           wndclassname,
                                           WS_VISIBLE |
                                           WS_CLIPCHILDREN |
                                           WS_CLIPSIBLINGS |
                                           WS_CHILD |
                                           WS_BORDER,
                                           rect.left,
                                           rect.top,
                                           rect.right,
                                           rect.bottom,
                                           parent,
                                           menu,
                                           NULL,
                                           this);

  if (name) {
    this->labelWindow = this->createLabel(parent, rect.right, rect.bottom, name);
  }
  this->setLabelOffset(0, 0);

  return this->wheelWindow;
}

void
SoWinThumbWheel::initWheel(int diameter, int width)
{
  int d, w;
  this->wheel->getSize(d, w);
  if (d == diameter && w == width) return;

  this->wheel->setSize(diameter, width);

  int pwidth = width;
  int pheight = diameter;
  if (this->orient == Horizontal) {
    pwidth = diameter;
    pheight = width;
  }

  if (this->pixmaps != NULL) {
    for (int i = 0; i < this->numPixmaps; i++) {
      Win32::DeleteObject(this->pixmaps[i]);
      this->pixmaps[i] = NULL;
    }
    delete [] this->pixmaps;
  }

  this->numPixmaps = this->wheel->getNumBitmaps();
  void * bits = NULL;

  this->pixmaps = new HBITMAP[numPixmaps];

  for (int i = 0; i < this->numPixmaps; i++) {
    this->pixmaps[i] = this->createDIB(pwidth, pheight, 32, &bits);
    this->wheel->drawBitmap(i, bits, (this->orient == Vertical) ?
                            SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);
  }
}

// *************************************************************************

void
SoWinThumbWheel::setEnabled(SbBool enable)
{
  if (enable)
    this->state = SoWinThumbWheel::Idle;
  else
    this->state = SoWinThumbWheel::Disabled;
  Win32::InvalidateRect(this->wheelWindow, NULL, FALSE);
  if (IsWindow(this->labelWindow))
    Win32::EnableWindow(this->labelWindow, enable);
}

SbBool
SoWinThumbWheel::isEnabled(void) const
{
  return (this->state != SoWinThumbWheel::Disabled);
}

void
SoWinThumbWheel::setValue(float value)
{
  this->wheelValue = this->tempWheelValue = value;
  this->mouseDownPos = this->mouseLastPos;
  Win32::InvalidateRect(this->wheelWindow, NULL, FALSE);
}

float
SoWinThumbWheel::value(void) const
{
  return this->wheelValue;
}

void
SoWinThumbWheel::setLabelText(const char * text)
{
  assert(IsWindow(this->wheelWindow));

  if (IsWindow(this->labelWindow)) {
    Win32::SetWindowText(this->labelWindow, text);
  }
  else {
    RECT rect;
    HWND parent = GetParent(this->wheelWindow);
    Win32::GetWindowRect(this->wheelWindow, & rect);
    this->labelWindow = this->createLabel(parent, rect.right + this->labelOffset.x,
                                          rect.bottom + labelOffset.y, text);
  }

  int len = strlen(text);
  HDC hdc = Win32::GetDC(this->labelWindow);
  SIZE textSize;
  Win32::GetTextExtentPoint(hdc, text, len, & textSize);

  UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW;
  Win32::SetWindowPos(this->labelWindow, NULL, 0, 0,
                      textSize.cx + 2, textSize.cy, flags);
}

void
SoWinThumbWheel::setLabelOffset(int x, int y)
{
  this->labelOffset.x = x;
  this->labelOffset.y = y;
}

SIZE
SoWinThumbWheel::getLabelSize(void)
{
  RECT rect;
  Win32::GetWindowRect(this->labelWindow, & rect);
  SIZE size = { rect.right - rect.left, rect.bottom - rect.top };
  return (size);
}

// *************************************************************************

void
SoWinThumbWheel::setRangeBoundaryHandling(boundaryHandling handling)
{
  switch (handling) {
  case CLAMP:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::CLAMP);
    break;
  case MODULATE:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::MODULATE);
    break;
  case ACCUMULATE:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
    break;
  default:
    assert(0 && "impossible");
  }
}

// *************************************************************************

SoWinThumbWheel::boundaryHandling
SoWinThumbWheel::getRangeBoundaryHandling(void) const
{
  switch (this->wheel->getBoundaryHandling()) {
  case SoAnyThumbWheel::CLAMP:
    return CLAMP;
  case SoAnyThumbWheel::MODULATE:
    return MODULATE;
  case SoAnyThumbWheel::ACCUMULATE:
    return ACCUMULATE;
  default:
    assert(0 && "impossible");
  }
  return CLAMP; // never reached
}

HWND
SoWinThumbWheel::createLabel(HWND parent, int x, int y, const char * text)
{
  assert(IsWindow(parent));
  // FIXME: assumes the same font as parent
  SIZE textSize = this->getTextSize(parent, text);
  HWND hwnd = Win32::CreateWindow_("STATIC",
                                   (text ? text : " "),
                                   WS_VISIBLE | WS_CHILD | SS_CENTER,
                                   x, y,
                                   textSize.cx + 2, textSize.cy, // SIZE
                                   parent,
                                   NULL,
                                   NULL,
                                   NULL);
  return hwnd;
}

HBITMAP
SoWinThumbWheel::createDIB(int width, int height, int bpp, void ** bits) // 16||24||32 bpp
{
  assert(bpp > 8);

  HBITMAP bitmap = NULL;
  HDC hdc = CreateCompatibleDC(NULL);
  assert(hdc!=NULL && "CreateCompatibleDC() failed -- investigate");
  int heapspace = sizeof(BITMAPINFOHEADER);

  HANDLE heap = GetProcessHeap();
  BITMAPINFO * format = (BITMAPINFO *) HeapAlloc(heap, 0, heapspace);

  BITMAPINFOHEADER * header = (BITMAPINFOHEADER *) format;
  header->biSize = sizeof(BITMAPINFOHEADER);
  header->biWidth = width;
  header->biHeight = -height;
  header->biPlanes = 1;
  header->biBitCount = bpp;
  header->biCompression = BI_RGB;
  header->biSizeImage = 0;
  header->biXPelsPerMeter = 0;
  header->biYPelsPerMeter = 0;
  header->biClrUsed = 0;
  header->biClrImportant = 0;

  UINT flag = DIB_RGB_COLORS;
  bitmap = CreateDIBSection(hdc, format, flag, (void **) bits, NULL, 0);
  assert(* bits);

  HeapFree(heap, 0, format);
  DeleteDC(hdc);

  return bitmap;
}

void
SoWinThumbWheel::blitBitmap(HBITMAP bitmap, HDC dc, int x,int y, int width, int height) const
{
  HDC memorydc = CreateCompatibleDC(dc);
  assert(memorydc!=NULL && "CreateCompatibleDC() failed -- investigate");
  HBITMAP oldBitmap = (HBITMAP) SelectObject(memorydc, bitmap);
  Win32::BitBlt(dc, x, y, width, height, memorydc, 0, 0, SRCCOPY);
  Win32::SelectObject(memorydc, oldBitmap);
  Win32::DeleteDC(memorydc);
}

SIZE
SoWinThumbWheel::getTextSize(HWND window, const char * text)
{
  assert(IsWindow(window));

  int len = strlen(text);
  HDC hdc = Win32::GetDC(window);

  SIZE size;
  Win32::GetTextExtentPoint(hdc, text, len, & size);
  return size;
}
