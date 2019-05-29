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

// Basic test example for embedding a SoWin-component (the
// examinerviewer, to be exact) into a native Win32 window.
//
// Build with: ``sowin-config --build embedded embedded.cpp''
//
// <mortene@sim.no>

#include <assert.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>

LRESULT CALLBACK
mainwinproc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  if ( message == WM_DESTROY ) {
    PostQuitMessage( 0 );
    return 0;
  }
  
  if ( message == WM_SIZE ) {
    printf("size\n");
  }

  return DefWindowProc(window, message, wparam, lparam);
}

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nShowCmd)
{
  WNDCLASS windowclass;
  windowclass.lpszClassName = "MainWindow";
  windowclass.hInstance = hInstance;
  windowclass.lpfnWndProc = mainwinproc;
  windowclass.style = CS_OWNDC;
  windowclass.lpszMenuName = NULL;
  windowclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
  windowclass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
  windowclass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
  windowclass.cbClsExtra = 0;
  windowclass.cbWndExtra = 4;

  RegisterClass(&windowclass);

  HWND rootwin =
    CreateWindow("MainWindow", // class name
                 "Embedding SoWin component", // window name
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, // window style
                 CW_USEDEFAULT, // xpos
                 0, // ypos (ignored because of WS_VISIBLE + CWD_USEDEFAULT)
                 600, // width
                 600, // height
                 NULL, // parent
                 NULL, // menu or child-window
                 hInstance, // application instance
                 NULL);

  assert(IsWindow(rootwin) && "window creation failed");

  SoWin::init(rootwin);

  SoSeparator * s = new SoSeparator; // Scene root
  s->addChild(new SoCone);

  SoWinExaminerViewer * eviewer = new SoWinExaminerViewer(rootwin);
  eviewer->setSceneGraph(s);
  eviewer->show();

  SoWin::mainLoop();

  return 0;
}
