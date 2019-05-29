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

// Class is documented in common/viewers/SoGuiPlaneViewer.cpp.in.

// *************************************************************************

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <sowindefs.h>
#include <Inventor/Win/widgets/SoWinThumbWheel.h>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>

#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewerP.h>

#include <Inventor/Win/common/pixmaps/ortho.xpm>
#include <Inventor/Win/common/pixmaps/perspective.xpm>
#include <Inventor/Win/common/pixmaps/x.xpm>
#include <Inventor/Win/common/pixmaps/y.xpm>
#include <Inventor/Win/common/pixmaps/z.xpm>

// ************************************************************************

// SoQtPlaneViewerP "private implementation" class.

#ifndef DOXYGEN_SKIP_THIS

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

SoWinPlaneViewerP::SoWinPlaneViewerP(SoWinPlaneViewer * publ)
  : SoGuiPlaneViewerP(publ)
{
  this->camerabutton = NULL;
}

SoWinPlaneViewerP::~SoWinPlaneViewerP()
{
}

// This contains the real constructor code (the two constructors are only
// entry points for this method).
void
SoWinPlaneViewerP::constructor(SbBool build)
{
  this->commonConstructor(); // init generic stuff

  PUBLIC(this)->setClassName("SoWinPlaneViewer");
  
  if (! build) return;

  HWND viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
  PUBLIC(this)->setBaseWidget(viewer);
  
  PUBLIC(this)->setLeftWheelString("TransY");
  PUBLIC(this)->setBottomWheelString("TransX");  

  PUBLIC(this)->setSize(SbVec2s(555, 515));
}

void
SoWinPlaneViewerP::xButtonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinPlaneViewerP * that = (SoWinPlaneViewerP *)userdata;
  that->viewPlaneX();
}

void
SoWinPlaneViewerP::yButtonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinPlaneViewerP * that = (SoWinPlaneViewerP *)userdata;
  that->viewPlaneY();
}

void
SoWinPlaneViewerP::zButtonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinPlaneViewerP * that = (SoWinPlaneViewerP *)userdata;
  that->viewPlaneZ();
}

void
SoWinPlaneViewerP::cameraButtonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinPlaneViewerP * that = (SoWinPlaneViewerP *)userdata;
  if (PUBLIC(that)->getCamera()) { PUBLIC(that)->toggleCameraType(); }
}

#endif // DOXYGEN_SKIP_THIS

// ************************************************************************

// Documented in superclass.
void
SoWinPlaneViewer::setCamera(SoCamera * newCamera)
{
  if (newCamera) {
    SoType camtype = newCamera->getTypeId();
    SbBool orthogonal =
      camtype.isDerivedFrom(SoOrthographicCamera::getClassTypeId());

    const char * oldLabel = this->getRightWheelString();
    if (oldLabel) {
      if (orthogonal) {
        if (strcmp("Dolly",oldLabel) == 0)
          this->setRightWheelString("Zoom");
      }
      else if (strcmp("Zoom",oldLabel) == 0)
        this->setRightWheelString("Dolly");
    }
    
    SoWinBitmapButton * b = (SoWinBitmapButton *)PRIVATE(this)->camerabutton;
    if (b) { b->setBitmap(orthogonal ? 1 : 0); }
  }
  inherited::setCamera(newCamera);
}

// ************************************************************************

// Documented in superclass.
HWND
SoWinPlaneViewer::buildWidget(HWND parent)
{
  return inherited::buildWidget(parent);
}

// ************************************************************************

// Documented in superclass.
void
SoWinPlaneViewer::createViewerButtons(HWND parent, SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  SoWinBitmapButton * b = new SoWinBitmapButton(parent, 24, "x", NULL);
  b->addBitmap(x_xpm);
  b->setBitmap(0);
  b->registerClickedProc(SoWinPlaneViewerP::xButtonProc, PRIVATE(this));
  buttonlist->append(b);
  
  b = new SoWinBitmapButton(parent, 24, "y", NULL);
  b->addBitmap(y_xpm);
  b->setBitmap(0);
  b->registerClickedProc(SoWinPlaneViewerP::yButtonProc, PRIVATE(this));
  buttonlist->append(b);

  b = new SoWinBitmapButton(parent, 24, "z", NULL);
  b->addBitmap(z_xpm);
  b->setBitmap(0);
  b->registerClickedProc(SoWinPlaneViewerP::zButtonProc, PRIVATE(this));
  buttonlist->append(b);
  
  b = new SoWinBitmapButton(parent, 24, "camera", NULL);
  b->addBitmap(perspective_xpm);
  b->addBitmap(ortho_xpm);
  b->setBitmap(0);
  b->registerClickedProc(SoWinPlaneViewerP::cameraButtonProc, PRIVATE(this));
  buttonlist->append(b);
  PRIVATE(this)->camerabutton = b;
}

// ************************************************************************
