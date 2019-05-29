#ifndef SOWINFULLVIEWERP_H
#define SOWINFULLVIEWERP_H

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

#ifndef SOWIN_INTERNAL
#error this is a private header file
#endif /* !SOWIN_INTERNAL */

#include <Inventor/Win/viewers/SoGuiFullViewerP.h>
#include <Inventor/Win/SoWinCursor.h>
#include <Inventor/Win/widgets/SoWinThumbWheel.h>

class SbStorage;

// ************************************************************************

// This class contains private data and methods used within the
// SoWinFullViewer class.

class SoWinFullViewerP : public SoGuiFullViewerP
{
public:
  SoWinFullViewerP(SoWinFullViewer * publ);
  ~SoWinFullViewerP();

  enum ButtonId {
    VIEWERBUTTON_PICK,
    VIEWERBUTTON_VIEW,
    VIEWERBUTTON_HOME,
    VIEWERBUTTON_SET_HOME,
    VIEWERBUTTON_VIEW_ALL,
    VIEWERBUTTON_SEEK
  };

  class SoWinBitmapButton * viewerButton(int idx);
  HWND appButton(int idx);

  static void setThumbWheelValue(void * wheel, float val);

  static void leftWheelCB(SoWinThumbWheel::Interaction type, float val,
                          void * userdata);
  static void bottomWheelCB(SoWinThumbWheel::Interaction type, float val,
                            void * userdata);
  static void rightWheelCB(SoWinThumbWheel::Interaction type, float val,
                           void * userdata);

  static void interactbuttonProc(SoWinBitmapButton * b, void * userdata);
  static void examinebuttonProc(SoWinBitmapButton * b, void * userdata);
  static void homebuttonProc(SoWinBitmapButton * b, void * userdata);
  static void sethomebuttonProc(SoWinBitmapButton * b, void * userdata);
  static void viewallbuttonProc(SoWinBitmapButton * b, void * userdata);

  static void seekbuttonProc(SoWinBitmapButton * b, void * userdata);
  void seekbuttonClicked(void);

  void showDecorationWidgets(SbBool onOff);

  HWND buildLeftWheel(HWND parent);
  HWND buildBottomWheel(HWND parent);
  HWND buildRightWheel(HWND parent);
  
  int layoutWidgets(int cx, int cy);
  static LRESULT CALLBACK systemEventHook(int code, WPARAM wparam, LPARAM lparam);

  static SbStorage * hookhandle;
  static int nrinstances;
  
  static SbDict * parentHWNDmappings;

  SbBool menuenabled;
  SbBool decorations;

  SoWinCursor cursor;

  HWND viewerwidget;
  HWND renderareawidget;

  SbPList lefttrimbuttons, righttrimbuttons;

  SoWinThumbWheel * leftthumbwheel, * bottomthumbwheel, * rightthumbwheel;
};

// ************************************************************************

#endif // ! SOWINFULLVIEWERP_H
