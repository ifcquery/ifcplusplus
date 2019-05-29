#ifndef COIN_SOWINCOLOREDITOR_H
#define COIN_SOWINCOLOREDITOR_H

// src\Inventor\Win\editors\SoWinColorEditor.h.  Generated from SoGuiColorEditor.h.in by configure.

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

#include <Inventor/Win/nodes/SoGuiColorEditor.h>
#include <Inventor/Win/SoWinRenderArea.h>

typedef void SoWinColorEditorCB(void * closure, const SbColor * color);
        
class SbColor;
class SoSFColor;
class SoMFColor;
class SoMFUInt32;

class SoWinColorEditor : public SoWinRenderArea {
  SOWIN_OBJECT_HEADER(SoWinColorEditor, SoWinRenderArea);

public:
  SoWinColorEditor(HWND parent = NULL, const char * name = NULL, SbBool embed = TRUE);
  ~SoWinColorEditor(void);

  enum Sliders {
    NONE = SoGuiColorEditor::NONE, 
    INTENSITY = SoGuiColorEditor::INTENSITY,
    RGB = SoGuiColorEditor::RGB, 
    HSV = SoGuiColorEditor::HSV, 
    RGB_V = SoGuiColorEditor::RGB_V, 
    RGB_HSV = SoGuiColorEditor::RGB_HSV
  };
  
  enum UpdateFrequency {
    CONTINUOUS = SoGuiColorEditor::CONTINUOUS,
    AFTER_ACCEPT = SoGuiColorEditor::AFTER_ACCEPT
  };
  
  void attach(SoSFColor * color, SoBase * node = NULL);
  void attach(SoMFColor * color, int idx = 0, SoBase * node = NULL);
  void attach(SoMFUInt32 * color, int idx = 0, SoBase * node = NULL);
  void detach(void);
  SbBool isAttached(void) const;
  
  void addColorChangedCallback(SoWinColorEditorCB * cb, void * closure = NULL);
  void removeColorChangedCallback(SoWinColorEditorCB * cb, void * closure = NULL);
  
  void setColor(const SbColor & color);
  const SbColor & getColor(void) const;
  
  void setWYSIWYG(SbBool enable);
  SbBool isWYSIWYG(void) const;
  
  void setCurrentSliders(Sliders which);
  Sliders getCurrentSliders(void) const;
  
  void setUpdateFrequency(UpdateFrequency freq);
  UpdateFrequency getUpdateFrequency(void) const;

  SoGuiColorEditor * getEditor(void) const;

protected:
  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void * internals;

};

#endif // !COIN_SOWINCOLOREDITOR_H
