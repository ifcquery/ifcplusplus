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

#include <sowindefs.h>
#include <Inventor/Win/devices/SoWinInputFocus.h>
#include <Inventor/Win/devices/SoGuiInputFocusP.h>

#define PRIVATE(obj) ((obj)->pimpl)

// *************************************************************************

class SoWinInputFocusP : public SoGuiInputFocusP {
public:
  SoWinInputFocusP(SoWinInputFocus * p) : SoGuiInputFocusP(p) { }
};

// *************************************************************************

SoWinInputFocus::SoWinInputFocus(int mask)
{
  PRIVATE(this) = new SoWinInputFocusP(this);
  PRIVATE(this)->eventmask = mask;
}

SoWinInputFocus::~SoWinInputFocus()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoWinInputFocus::enable(HWND, // widget,
                        SoWinEventHandler *, // handler,
                        void *)
{
  SOWIN_STUB();
}

void
SoWinInputFocus::disable(HWND, // widget,
                         SoWinEventHandler *, // handler,
                         void *) // closure)
{
  SOWIN_STUB();
}

// *************************************************************************

const SoEvent *
SoWinInputFocus::translateEvent(MSG *) // event)
{
  /*
  long msec =  GetTickCount();
  if (soevent) soevent->setTime(SbTime((msec / 1000), (msec % 1000) * 1000)));
  */
  SOWIN_STUB();
  return NULL;
}

// *************************************************************************

#undef PRIVATE
