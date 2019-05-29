#ifndef SOWIN_KEYBOARD_H
#define SOWIN_KEYBOARD_H

// src\Inventor\Win\devices\SoWinKeyboard.h.  Generated from SoGuiKeyboard.h.in by configure.

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

#include <Inventor/Win/devices/SoWinDevice.h>

class SoKeyboardEvent;

// *************************************************************************

#define SO_WIN_ALL_KEYBOARD_EVENTS SoWinKeyboard::ALL

class SOWIN_DLL_API SoWinKeyboard : public SoWinDevice {
  SOWIN_OBJECT_HEADER(SoWinKeyboard, SoWinDevice);

public:
  enum Events {
    KEY_PRESS = 0x01,
    KEY_RELEASE = 0x02,
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoWinKeyboard(int eventmask = ALL_EVENTS);
  virtual ~SoWinKeyboard(void);

  virtual void enable(HWND widget, SoWinEventHandler * handler, void * closure);
  virtual void disable(HWND widget, SoWinEventHandler * handler, void * closure);

  virtual const SoEvent * translateEvent(MSG * event);

private:
  class SoWinKeyboardP * pimpl;
  friend class SoWinKeyboardP;
  friend class SoGuiKeyboardP;
};

// *************************************************************************

#endif // ! SOWIN_KEYBOARD_H
