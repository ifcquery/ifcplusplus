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

#include <Inventor/lists/SbPList.h>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>
#include <Inventor/Win/SoWin.h>
#include <sowindefs.h>
#include <Inventor/Win/Win32API.h>

#include <assert.h>
#include <stdio.h>


// *************************************************************************

class SoWinBitmapButtonP {
public:
  SoWinBitmapButtonP(SoWinBitmapButton * master)
  {
    this->owner = master;
    this->clickproc = NULL;
    this->clickprocdata = NULL;
    this->buttonwindow = NULL;
  }

  ~SoWinBitmapButtonP()
  {
    // Note: since the buttonwindow is owned by it's parent,
    // it will be destroyed (DestroyWindow()) when parent is destroyed.
    // This could happen before or after this destructor is called,
    // so we need to be robust. 2004-01-21 thammer.

    // Note: We need to make sure the button_proc() is not called
    // after the destructor has been called. 2004-01-21 thammer.
    if ( (this->buttonwindow) && IsWindow(this->buttonwindow) ) {
      Win32::SetWindowLongPtr(this->buttonwindow, GWLP_WNDPROC,
                              (LONG_PTR)this->prevwndfunc);
      this->buttonwindow = NULL;
    }

    const int len = this->bitmaplist.getLength();
    for (int i = 0; i < len; i++) { Win32::DeleteObject(this->bitmaplist[i]); }
  }

  HWND buildWidget(HWND parent, RECT rect);
  HBITMAP createDIB(int width, int height, int bpp, void ** bits);
  HBITMAP parseXpm(const char ** xpm, int dibdepth = 24);
  static int axtoi(const char * str);

  HWND buttonwindow;
  SbPList bitmaplist;

  SoWinBitmapButton::ClickedProc * clickproc;
  void * clickprocdata;

  WNDPROC prevwndfunc;
  static LRESULT CALLBACK button_proc(HWND hwnd, UINT msg,
                                      WPARAM wparam, LPARAM lparam);

private:
  SoWinBitmapButton * owner;
};

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->owner)

// *************************************************************************

SoWinBitmapButton::SoWinBitmapButton(HWND parent,
                                     int x,
                                     int y,
                                     int width,
                                     int height,
                                     int depth,
                                     const char * name,
                                     void * bits)
{
  PRIVATE(this) = new SoWinBitmapButtonP(this);

  RECT rect = { x, y, width, height };
  PRIVATE(this)->buildWidget(parent, rect);

  if (bits != NULL) {
    this->addBitmap(width, height, depth, bits);
    this->setBitmap(0);
  }

}

SoWinBitmapButton::SoWinBitmapButton(HWND parent,
                                     int depth,
                                     const char * name,
                                     void * bits)
{
  PRIVATE(this) = new SoWinBitmapButtonP(this);

  RECT rect = { 0, 0, 30, 30 };
  PRIVATE(this)->buildWidget(parent, rect);

  if (bits != NULL) {
    this->addBitmap(rect.right - rect.left - 1,
                    rect.bottom - rect.top - 1,
                    depth, bits);
    this->setBitmap(0);
  }

}

SoWinBitmapButton::~SoWinBitmapButton(void)
{
  delete PRIVATE(this);
}

HWND
SoWinBitmapButton::getWidget(void)
{
  return PRIVATE(this)->buttonwindow;
}

int
SoWinBitmapButton::width(void) const
{
  RECT rect;
  Win32::GetWindowRect(PRIVATE(this)->buttonwindow, & rect);
  return (rect.right - rect.left);
}

int
SoWinBitmapButton::height(void) const
{
  RECT rect;
  Win32::GetWindowRect(PRIVATE(this)->buttonwindow, & rect);
  return (rect.bottom - rect.top);
}

void
SoWinBitmapButton::move(int x, int y)
{
  assert(IsWindow(PRIVATE(this)->buttonwindow));
  UINT flags = SWP_NOSIZE | SWP_NOZORDER;
  Win32::SetWindowPos(PRIVATE(this)->buttonwindow, NULL, x, y, 0, 0, flags);
}

void
SoWinBitmapButton::move(int x, int y, int width, int height)
{
  assert(IsWindow(PRIVATE(this)->buttonwindow));
  Win32::MoveWindow(PRIVATE(this)->buttonwindow, x, y, width, height, TRUE);
}

void
SoWinBitmapButton::resize(int width, int height)
{
  assert(IsWindow(PRIVATE(this)->buttonwindow));
  UINT flags = SWP_NOMOVE | SWP_NOZORDER;
  Win32::SetWindowPos(PRIVATE(this)->buttonwindow, NULL, 0, 0, width, height, flags);
}

void
SoWinBitmapButton::show(void)
{
  (void)ShowWindow(PRIVATE(this)->buttonwindow, SW_SHOW);
}

void
SoWinBitmapButton::hide(void)
{
  (void)ShowWindow(PRIVATE(this)->buttonwindow, SW_HIDE);
}

void
SoWinBitmapButton::registerClickedProc(ClickedProc * func, void * userdata)
{
  PRIVATE(this)->clickproc = func;
  PRIVATE(this)->clickprocdata = userdata;
}

LRESULT CALLBACK
SoWinBitmapButtonP::button_proc(HWND hwnd, UINT msg,
                                WPARAM wparam, LPARAM lparam)
{
  LONG_PTR l = Win32::GetWindowLongPtr(hwnd, GWLP_USERDATA);
  SoWinBitmapButtonP * that = (SoWinBitmapButtonP *)l;

  if (that->clickproc) {
    // Find out if the button was clicked.
    SbBool clicked = (msg == WM_LBUTTONUP);
    clicked = clicked && (GetCapture() == hwnd);
    // FIXME: should also check that button is a) pressed and b)
    // highlighted. (We would then at least match all tests done in
    // Wine's BUTTON class before sending a WM_COMMAND message to it's
    // parent.) 20030425 mortene.
    if (clicked) {
      POINT mousept = { LOWORD(lparam),  HIWORD(lparam) };
      RECT rect;
      Win32::GetClientRect(hwnd, &rect);
      clicked = PtInRect(&rect, mousept);
    }

    if (clicked) { that->clickproc(that->owner, that->clickprocdata); }
  }

  return CallWindowProc(that->prevwndfunc, hwnd, msg, wparam, lparam);
}

HWND
SoWinBitmapButtonP::buildWidget(HWND parent, RECT rect)
{
  assert(IsWindow(parent));

  this->buttonwindow =
    Win32::CreateWindow_("BUTTON", // lpClassName
                         NULL, // lpWindowName
                         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS |
                         BS_PUSHBUTTON | BS_BITMAP | BS_CENTER, // dwStyle
                         rect.left, rect.top, // position
                         rect.right, rect.bottom, // window width & height
                         parent, // hWndParent (owner window)
                         NULL, // hMenu
                         NULL, // hInstance (application instance)
                         NULL); // lpParam (window-creation data)

  this->prevwndfunc = (WNDPROC)
    Win32::SetWindowLongPtr(this->buttonwindow, GWLP_WNDPROC,
                            (LONG_PTR)SoWinBitmapButtonP::button_proc);
  (void)Win32::SetWindowLongPtr(this->buttonwindow, GWLP_USERDATA,
                                (LONG_PTR)this);

  return this->buttonwindow;
}

void
SoWinBitmapButton::setEnabled(SbBool enable)
{
  Win32::EnableWindow(PRIVATE(this)->buttonwindow, enable);
}

SbBool
SoWinBitmapButton::isEnabled(void) const
{
  return (! (Win32::GetWindowLongPtr(PRIVATE(this)->buttonwindow, GWL_STYLE) & WS_DISABLED));
}

void
SoWinBitmapButton::setPressedState(SbBool enable)
{
  (void)SendMessage(PRIVATE(this)->buttonwindow, BM_SETSTATE,
                    (WPARAM)enable, 0);
}

SbBool
SoWinBitmapButton::getPressedState(void) const
{
  return (SendMessage(PRIVATE(this)->buttonwindow, BM_GETSTATE, 0, 0) &
          BST_PUSHED);
}

void
SoWinBitmapButton::addBitmap(HBITMAP hbmp)
{
  PRIVATE(this)->bitmaplist.append(hbmp);
}

void
SoWinBitmapButton::addBitmap(int width, int height, int bpp, void * src)
{
 void * dest;

 HBITMAP hbmp = PRIVATE(this)->createDIB(width, height, bpp, & dest);
 (void)memcpy(dest, src, width * height * (bpp / 8));
 this->addBitmap(hbmp);
}

void
SoWinBitmapButton::addBitmap(const char ** xpm, int bpp)
{
  HBITMAP bm = PRIVATE(this)->parseXpm(xpm, bpp);
  this->addBitmap(bm);
}

HBITMAP
SoWinBitmapButton::getBitmap(int index) const
{
  return (HBITMAP) PRIVATE(this)->bitmaplist[index];
}

void
SoWinBitmapButton::setBitmap(int index)
{
  assert(IsWindow(PRIVATE(this)->buttonwindow));

  (void)SendMessage(PRIVATE(this)->buttonwindow,
                    BM_SETIMAGE,
                    (WPARAM) IMAGE_BITMAP,
                    (LPARAM) this->getBitmap(index));

  Win32::InvalidateRect(PRIVATE(this)->buttonwindow, NULL, FALSE);
}

HBITMAP
SoWinBitmapButtonP::createDIB(int width, int height, int bpp, void ** bits) // 16||24||32 bpp
{
  assert(bpp > 8);

  HBITMAP bitmap = NULL;
  HDC hdc = CreateCompatibleDC(NULL);
  assert(hdc!=NULL && "CreateCompatibleDC() failed -- investigate");
  int heapspace = sizeof(BITMAPINFOHEADER);

  // FIXME: allocating the BITMAPINFO on the heap seems unnecessary..?
  // Investigate. 20051020 mortene.
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
  Win32::DeleteDC(hdc);

  return bitmap;
}

// Convert from xpm to DIB (demands hex colors).
HBITMAP
SoWinBitmapButtonP::parseXpm(const char ** xpm, int dibdepth)
{
  unsigned char pixelsize = dibdepth / 8;

  int width, height, numcol, numchars;
  int nr = sscanf(xpm[0], "%d %d %d %d", &width, &height, &numcol, &numchars);
  assert((nr == 4) && "corrupt xpm?");

  // create color lookup table
  char * charlookuptable = new char[numcol * numchars];
  int * colorlookuptable = new int[numcol];

  // get colors
  int i;
  for (i = 0; i < numcol; i++) {

    int j;
    for (j = 0; j < numchars; j ++) {
      charlookuptable[(i * numchars) + j] = * (xpm[i + 1] + j);
    }

    // FIXME: make sure it is 'c '
    const char * strstart = strchr((xpm[i + 1] + numchars), 'c');

    const char * strend = strstart + 2;

    if (*strend == '#')
      colorlookuptable[i] = SoWinBitmapButtonP::axtoi(strend + 1);
    else
      colorlookuptable[i] = -1; // Parse string (color name)

  }

  // create bitmap
  void * dest;
  HBITMAP hbmp = this->createDIB(width, height, dibdepth, &dest);

  // put pixels
  for (i = 0; i < height; i++) {

    const char * line = xpm[i + 1 + numcol];

    int y = i * width * pixelsize;

    int j;
    for (j = 0; j < width; j++) {

      int x = j * pixelsize;

      // for every color
      int k;
      for (k = 0; k < numcol; k++) {

        int l;
        for (l = 0; l < numchars; l++)
          if (charlookuptable[(k * numchars) + l] != line[(j * numchars) + l])
            break;

        // if we found the char in the lookup table
        if (l >= numchars) {

          unsigned int colorvalue;
          if (colorlookuptable[k] == -1)
            colorvalue = GetSysColor(COLOR_3DFACE) & 0x00FFFFFF; // FIXME: color make param
          else
            colorvalue = colorlookuptable[k] | 0xFF000000;

          // FIXME: may not work with depth < 24
          // for each color byte in the pixel
          int m;
          for (m = 0; m < pixelsize; m++) {

            // put color byte (and only one byte)
            ((char *) dest)[y + x + m] =
              (char) ((colorvalue & (0x000000FF << (m << 3))) >> (m << 3));

          }

          // next pixel
          break;

        }

      }

    }

  }

  // cleanup
  delete [] charlookuptable;
  delete [] colorlookuptable;

  // return bitmap
  return hbmp;
}

int
SoWinBitmapButtonP::axtoi(const char * str) // convert from ASCII hex to int
{
  const char * c = str;
  int n = (strchr(c, '\0') - c);

  int x = 0;

  // convert n nibbles
  for (int i = 0; i < n; i++) {

    // numbers 0 - 9
    if ((c[i] > 0x2F) && (c[i] < 0x3A))
      x += ((c[i] - 0x30) << ((n - i - 1) * 4));

    // capital letters A - F
    if ((c[i] > 0x40) && (c[i] < 0x47))
      x += ((c[i] - 0x41 + 0x0A) << ((n - i - 1) * 4));

    // lower case letters a - f
    if ((c[i] > 0x60) && (c[i] < 0x67))
      x += ((c[i] - 0x61 + 0x0A) << ((n - i - 1) * 4));

  }

  return x;
}
