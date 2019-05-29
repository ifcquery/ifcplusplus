// src\Inventor\Win\devices\SoWinDeviceCommon.cpp.  Generated from SoGuiDevice.cpp.in by configure.

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

// This file contains common sourcecode and documentation for the
// So[Gui]Device classes, in template form.

/*!
  \class SoWinDevice SoWinDevice.h Inventor/Win/devices/SoWinDevice.h
  \brief The SoWinDevice class is the base class for the translation devices.
  \ingroup devices

  The SoWin device classes provide glue functionality for
  translating native GUI events from the underlying toolkit to
  Inventor scenegraph SoEvent events.

  The device classes are mainly of interest to application programmers
  when writing extensions for new types of devices, and seldom in
  other contexts -- so they can most often be ignored.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Win/devices/SoWinDevice.h>
#include <Inventor/Win/devices/SoWinDeviceP.h>
#include <Inventor/SbPList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoEvent.h>

#include <Inventor/Win/devices/SoWinInputFocus.h>
#include <Inventor/Win/devices/SoWinKeyboard.h>
#include <Inventor/Win/devices/SoWinMouse.h>

#ifdef HAVE_JOYSTICK_LINUX
#include <Inventor/Win/devices/SoWinLinuxJoystick.h>
#endif // HAVE_JOYSTICK_LINUX

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

/*!
  \fn SoWinDevice::SoWinDevice(void)
  Constructor.  Protected to only enable invocation from derived device
  classes.
*/
/*!
  \fn SoWinDevice::~SoWinDevice()
  Public virtual destructor.
*/

/*!
  \fn void SoWinDevice::enable(HWND widget, SoWinEventHandler * handler, void * closure)

  This method will enable the device for the widget.

  \a handler is invoked with the \a closure argument when an event
  occur in \a widget.
*/

/*!
  \fn void SoWinDevice::disable(HWND widget, SoWinEventHandler * handler, void * closure)

  This method will disable the handler for the device.
*/

/*!
  \fn const SoEvent * SoWinDevice::translateEvent(MSG * event)

  This method translates from native events to Open Inventor SoEvent
  events.
*/

/*!
  \fn void SoWinDevice::invokeHandlers(MSG * event)

  Invoke all handlers registered with SoWinDevice::addEventHandler().
*/

// *************************************************************************

SOWIN_OBJECT_ABSTRACT_SOURCE(SoWinDevice);

// *************************************************************************

/*!
  This function initializes the type system for all the SoWin
  device classes.  It is called indirectly when calling
  SoWin::init(), so the application programmer doesn't have to
  bother with it.
*/
void
SoWinDevice::initClasses(void)
{
  SoWinDevice::initClass();
  SoWinInputFocus::initClass();
  SoWinKeyboard::initClass();
  SoWinMouse::initClass();
#ifdef HAVE_JOYSTICK_LINUX
  SoWinLinuxJoystick::initClass();
#endif // HAVE_JOYSTICK_LINUX
}

// *************************************************************************

/*!
  This method sets the cached size of the window the device is
  "attached" to.
*/
void
SoWinDevice::setWindowSize(const SbVec2s size)
{
  PRIVATE(this)->widgetsize = size;
}

/*!
  This method returns the cached window size.
*/
SbVec2s
SoWinDevice::getWindowSize(void) const
{
  return PRIVATE(this)->widgetsize;
}

// *************************************************************************

/*!
  This method fills in the position information of \a event,
  translating the coordinates into the correct coordinate system.
*/
void
SoWinDevice::setEventPosition(SoEvent * event, int x, int y) const
{
  SoGuiDeviceP::lasteventposition = SbVec2s(x, y);
  event->setPosition(SbVec2s(x, PRIVATE(this)->widgetsize[1] - y - 1));
}

/*!
  Returns last event position that was set with
  SoWinDevice::setEventPosition().
 */
SbVec2s
SoWinDevice::getLastEventPosition(void)
{
  return SoGuiDeviceP::lasteventposition;
}

// *************************************************************************

/*!
  Stores an event \a handler for the \a widget, which can later be
  invoked with SoWinDevice::invokeHandlers(). Used by extension
  device types.
 */
void
SoWinDevice::addEventHandler(HWND widget,
                               SoWinEventHandler * handler,
                               void * closure)
{
  PRIVATE(this)->addEventHandler(widget, handler, closure);
}

/*!
  Remove a \a handler registered with SoWinDevice::addEventHandler().
 */
void
SoWinDevice::removeEventHandler(HWND widget,
                                  SoWinEventHandler * handler,
                                  void * closure)
{
  PRIVATE(this)->removeEventHandler(widget, handler, closure);
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// Remaining code is for the internal, private hidden implementation
// class.

SbVec2s SoGuiDeviceP::lasteventposition;

struct SoGuiDevicePHandlerInfo {
  HWND widget;
  SoWinEventHandler * handler;
  void * closure;
};

SoGuiDeviceP::SoGuiDeviceP(SoWinDevice * p)
{
  PUBLIC(this) = p;
  this->handlers = NULL;
  this->widgetsize = SbVec2s(0, 0);
}

SoGuiDeviceP::~SoGuiDeviceP()
{
  if (this->handlers) {
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoGuiDevicePHandlerInfo * info =
        (SoGuiDevicePHandlerInfo *) (*this->handlers)[i];
      delete info;
    }
    delete this->handlers;
  }
}

void
SoGuiDeviceP::addEventHandler(HWND widget,
                              SoWinEventHandler * handler,
                              void * closure)
{
  if (this->handlers == NULL) { this->handlers = new SbPList; }
  SoGuiDevicePHandlerInfo * info = new SoGuiDevicePHandlerInfo;
  info->widget = widget;
  info->handler = handler;
  info->closure = closure;
  this->handlers->append(info);
}

void
SoGuiDeviceP::removeEventHandler(HWND widget,
                                 SoWinEventHandler * handler,
                                 void * closure)
{
  if (this->handlers) {
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoGuiDevicePHandlerInfo * info =
        (SoGuiDevicePHandlerInfo *) (*this->handlers)[i];
      if ((info->widget == widget) && (info->handler == handler) &&
           (info->closure == closure)) {
        delete info;
        this->handlers->remove(i);
        return;
      }
    }
  }
#if SOWIN_DEBUG
  SoDebugError::post("SoGuiDeviceP::removeEventHandler",
                     "tried to remove nonexisting handler");
#endif // SOWIN_DEBUG
}

void
SoGuiDeviceP::invokeHandlers(HandlerCB * cb, MSG * event)
{
  if (this->handlers) {
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoGuiDevicePHandlerInfo * info =
        (SoGuiDevicePHandlerInfo *) (*this->handlers)[i];
      cb(info->handler, info->widget, event, info->closure);
    }
  }
}

#endif // DOXYGEN_SKIP_THIS

#undef PRIVATE
#undef PUBLIC

