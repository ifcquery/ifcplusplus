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

// Class documentation in common/viewers/SoGuiExaminerViewer.cpp.in.

// *************************************************************************

#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinCursor.h>
#include <Inventor/Win/common/pixmaps/ortho.xpm>
#include <Inventor/Win/common/pixmaps/perspective.xpm>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinExaminerViewerP.h>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <sowindefs.h>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

// The private data for the SoWinExaminerViewer.

#ifndef DOXYGEN_SKIP_THIS

SoWinExaminerViewerP::SoWinExaminerViewerP(SoWinExaminerViewer * o)
  : SoGuiExaminerViewerP(o)
{
  this->camerabutton = NULL;
}

SoWinExaminerViewerP::~SoWinExaminerViewerP()
{
}

// This contains the real constructor code (the two constructors are
// only entry points for this method).
void
SoWinExaminerViewerP::constructor(SbBool build)
{
  this->genericConstructor();

  PUBLIC(this)->setClassName("SoWinExaminerViewer");
  PUBLIC(this)->setPopupMenuString("Examiner Viewer");

  if (! build) return;

  HWND widget = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
  PUBLIC(this)->setBaseWidget(widget);

  PUBLIC(this)->setLeftWheelString("RotX");
  PUBLIC(this)->setBottomWheelString("RotY");

  PUBLIC(this)->setCursorEnabled(TRUE);
  PUBLIC(this)->setAnimationEnabled(TRUE);

  PUBLIC(this)->setSize(SbVec2s(500, 421));
  // FIXME: If the new size is the same as the old size, Windows will
  // never size the widget, and layoutWidgets() will never be
  // called. mariusbu 20010823.

}

void
SoWinExaminerViewerP::cameraButtonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinExaminerViewer * that = (SoWinExaminerViewer *)userdata;
  if (that->getCamera()) that->toggleCameraType();
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

SOWIN_OBJECT_SOURCE(SoWinExaminerViewer);

// *************************************************************************

// Documented in common/viewers/SoGuiExaminerViewer.cpp.in.
SoWinExaminerViewer::SoWinExaminerViewer(HWND parent,
                                         const char * name,
                                         SbBool embed,
                                         SoWinFullViewer::BuildFlag flag,
                                         SoWinViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoWinExaminerViewerP(this);
  PRIVATE(this)->constructor(TRUE);
}

// *************************************************************************

// Documented in common/viewers/SoGuiExaminerViewer.cpp.in.
SoWinExaminerViewer::SoWinExaminerViewer(HWND parent,
                                         const char * name,
                                         SbBool embed,
                                         SoWinFullViewer::BuildFlag flag,
                                         SoWinViewer::Type type,
                                         SbBool build)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoWinExaminerViewerP(this);
  PRIVATE(this)->constructor(build);
}

// *************************************************************************

SoWinExaminerViewer::~SoWinExaminerViewer()
{
  PRIVATE(this)->genericDestructor();
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoWinExaminerViewer::setCamera(SoCamera * newCamera)
{
  // This method overridden from parent class to toggle the camera
  // type selection button pixmap and string of the zoom/dolly
  // thumbwheel.

  inherited::setCamera(newCamera);

  if (! newCamera)
    return;

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
  SoWinBitmapButton * wbtn = PRIVATE(this)->camerabutton;
  // If viewer was made without decorations, button will not have been
  // made yet.
  if (wbtn) { wbtn->setBitmap(orthogonal ? 1 : 0); }
}

// *************************************************************************

// Documented in superclass.
void
SoWinExaminerViewer::createViewerButtons(HWND parent, SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  SoWinBitmapButton * b = PRIVATE(this)->camerabutton =
    new SoWinBitmapButton(parent, 24, "perspective", NULL);
  b->addBitmap(perspective_xpm);
  b->addBitmap(ortho_xpm);
  b->setBitmap(0);
  b->registerClickedProc(SoWinExaminerViewerP::cameraButtonProc, this);
  buttonlist->append(b);
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC
