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

// ************************************************************************

// Class is documented in common/viewers/SoGuiWalkViewer.cpp.in.

// ************************************************************************

#include <sowindefs.h>
#include <Inventor/Win/viewers/SoWinWalkViewer.h>
#include <Inventor/Win/viewers/SoGuiWalkViewerP.h>

// ************************************************************************

SOWIN_OBJECT_SOURCE(SoWinWalkViewer);

// ************************************************************************

class SoWinWalkViewerP : public SoGuiWalkViewerP {
public:
  SoWinWalkViewerP(SoWinWalkViewer * o)
    : SoGuiWalkViewerP(o)
  {
  }
};

#define PRIVATE(ptr) ptr->pimpl
#define PUBLIC(ptr) ptr->pub

// ************************************************************************

/*!
  The public constructor.
*/

SoWinWalkViewer::SoWinWalkViewer(
  HWND parent,
  const char * name,
  SbBool embed,
  SoWinFullViewer::BuildFlag flag,
  SoWinViewer::Type type)
: inherited(parent, name, embed, flag, type, TRUE)
{
  PRIVATE(this) = new SoWinWalkViewerP(this);
//  this->constructor(TRUE);
} // SoWinWalkViewer()

// ************************************************************************

/*!
  A protected constructor.
*/

SoWinWalkViewer::SoWinWalkViewer(// protected
  HWND parent,
  const char * name,
  SbBool embed,
  SoWinFullViewer::BuildFlag flag,
  SoWinViewer::Type type,
  SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoWinWalkViewerP(this);
//  this->constructor(build);
} // SoWinWalkViewer()

// ************************************************************************

/*!
  The destructor.
*/

SoWinWalkViewer::~SoWinWalkViewer()
{
  delete PRIVATE(this);
} // ~SoWinWalkViewer()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::setViewing(// virtual
  SbBool enable)
{
} // setViewing()

// ************************************************************************

// Documented in superclass.
void
SoWinWalkViewer::setCamera(SoCamera * camera)
{
}

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::setCursorEnabled(// virtual
  SbBool enable)
{
} // setCursorEnabled()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::setCameraType(// virtual
  SoType type)
{
} // setCameraType()

// ************************************************************************

/*!
*/

const char *
SoWinWalkViewer::getDefaultWidgetName(// virtual
  void) const
{
  static const char defaultWidgetName[] = "";
  return defaultWidgetName;
} // getDefaultWidgetName()

// ************************************************************************

/*!
*/

const char *
SoWinWalkViewer::getDefaultTitle(// virtual
  void) const
{
  static const char defaultTitle[] = "Walk Viewer";
  return defaultTitle;
} // getDefaultTitle()

// ************************************************************************

/*!
*/

const char *
SoWinWalkViewer::getDefaultIconTitle(// virtual
  void) const
{
  static const char defaultIconTitle[] = "";
  return defaultIconTitle;
} // getDefaultIconTitle()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::processEvent(// virtual
  MSG * event)
{
} // processEvent()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::setSeekMode(// virtual
  SbBool enable)
{
} // setSeekMode()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::actualRedraw(// virtual
  void)
{
} // actualRedraw()

// ************************************************************************

/*!
*/

void
SoWinWalkViewer::rightWheelMotion(// virtual
  float value)
{
} // rightWheelMotion()

// ************************************************************************

/*!
*/

HWND
SoWinWalkViewer::buildLeftTrim(// virtual
  HWND parent)
{
  return NULL;
} // buildLeftTrim()

// ************************************************************************
