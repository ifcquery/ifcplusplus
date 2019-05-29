// src\Inventor\Win\viewers\SoWinConstrainedViewer.h.  Generated from SoGuiConstrainedViewer.h.in by configure.

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

#ifndef SOWIN_CONSTRAINEDVIEWER_H
#define SOWIN_CONSTRAINEDVIEWER_H

#include <Inventor/Win/viewers/SoWinFullViewer.h>

// ************************************************************************

class SOWIN_DLL_API SoWinConstrainedViewer : public SoWinFullViewer {
  SOWIN_OBJECT_ABSTRACT_HEADER(SoWinConstrainedViewer, SoWinFullViewer);

public:
  void setUpDirection(const SbVec3f & dir);
  const SbVec3f & getUpDirection(void) const;

  virtual void setCamera(SoCamera * newcam);
  virtual void saveHomePosition(void);
  virtual void resetToHomePosition(void);

protected:
  SoWinConstrainedViewer(HWND parent,
                           const char * const name,
                           SbBool embed,
                           SoWinFullViewer::BuildFlag flag, 
                           SoWinViewer::Type type, 
                           SbBool build);

  ~SoWinConstrainedViewer();

  virtual void tiltCamera(float delta);
  virtual void bottomWheelMotion(float value);
  virtual void leftWheelMotion(float value);

  virtual void changeCameraValues(SoCamera * camera);
  void findUpDirection(SbVec2s mouselocation);
  void checkForCameraUpConstrain(void);

private:
  class SoWinConstrainedViewerP * pimpl;
};

// ************************************************************************

#endif // ! SOWIN_CONSTRAINEDVIEWER_H
