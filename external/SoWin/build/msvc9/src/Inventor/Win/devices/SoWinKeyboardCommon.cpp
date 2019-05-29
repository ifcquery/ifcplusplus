// src\Inventor\Win\devices\SoWinKeyboardCommon.cpp.  Generated from SoGuiKeyboard.cpp.in by configure.

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

/*!
  \class SoWinKeyboard SoWinKeyboard.h Inventor/Win/devices/SoWinKeyboard.h
  \brief The SoWinKeyboard class is the keyboard input device abstraction.
  \ingroup devices

  The SoWinKeyboard class is the glue between native keyboard
  handling and keyboard interaction with the Inventor scenegraph.

  All components derived from the SoWinRenderArea have got an
  SoWinKeyboard device attached by default.
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Win/devices/SoWinKeyboard.h>
#include <Inventor/Win/devices/SoGuiKeyboardP.h>
#include <Inventor/events/SoKeyboardEvent.h>

// *************************************************************************

SOWIN_OBJECT_SOURCE(SoWinKeyboard);

// *************************************************************************

/*!
  \enum SoWinKeyboard::Events
  Enumeration over supported event types.
*/

/*!
  \var SoWinKeyboard::Events SoWinKeyboard::KEY_PRESS
  Maskbit for a keyboard button press event.
*/

/*!
  \var SoWinKeyboard::Events SoWinKeyboard::KEY_RELEASE
  Maskbit for a keyboard button release event.
*/

/*!
  \var SoWinKeyboard::Events SoWinKeyboard::ALL_EVENTS
  Combined bitmask for all possible events.
*/

/*!
  \fn SoWinKeyboard::SoWinKeyboard(int mask)

  Constructor. The \a mask specifies which keyboard-related events to
  handle. Others will just be ignored.
*/

/*!
  \fn SoWinKeyboard::~SoWinKeyboard()

  Destructor.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

SoGuiKeyboardP::SoGuiKeyboardP(void)
{
  this->kbdevent = new SoKeyboardEvent;
}

SoGuiKeyboardP::~SoGuiKeyboardP()
{
  delete this->kbdevent;
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************
