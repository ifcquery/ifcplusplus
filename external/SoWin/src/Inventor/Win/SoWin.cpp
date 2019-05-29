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

// Class documentation in common/SoGuiCommon.cpp.in.

// The mainpage documentation below is heavily based on the one for
// SoQt in SoQt.cpp.

/*!
  \mainpage

  The %SoWin library is a C++ GUI toolkit that binds together the
  \COIN rendering library with the user interface parts of the
  MSWindows Win32 API, thereby providing application programmers a
  convenient method of using the \COIN library from MSWindows
  development environments.

  By using the combination of \COIN and %SoWin for your 3D
  applications, you have a complete framework for writing software for
  Microsoft Windows operating systems.

  %SoWin, like \COIN, provides the programmer with a high-level
  application programmer's interface (API) in C++. The library
  primarily includes a class-hierarchy of viewer components of varying
  functionality and complexity, with various modes for the end-user to
  control the 3D-scene camera interaction.

  For a small, completely stand-alone usage example on how to
  initialize the library and set up a viewer instance window, see the
  following code:

  \code
  #include <Inventor/Win/SoWin.h>
  #include <Inventor/Win/viewers/SoWinExaminerViewer.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  int
  main(int argc, char ** argv)
  {
    // Initializes SoWin library (and implicitly also the Coin
    // library). Returns a top-level / shell window to use.
    HWND mainwin = SoWin::init(argc, argv, argv[0]);
  
    // Make a dead simple scene graph by using the Coin library, only
    // containing a single yellow cone under the scenegraph root.
    SoSeparator * root = new SoSeparator;
    root->ref();

    SoBaseColor * col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
  
    // Use one of the convenient SoWin viewer classes.
    SoWinExaminerViewer * eviewer = new SoWinExaminerViewer(mainwin);
    eviewer->setSceneGraph(root);
    eviewer->show();
  
    // Pop up the main window.
    SoWin::show(mainwin);
    // Loop until exit.
    SoWin::mainLoop();

    // Clean up resources.
    delete eviewer;
    root->unref();
    SoWin::done();

    return 0;
  }
  \endcode

  As compiled and run, this example provides the end-user with a full
  fledged 3D viewer. The viewer automatically contains mouse
  interaction handling logic to let the end-user "examine" the
  3D-model / scene (since this is the SoWinExaminerViewer class), plus
  toolbar controls on the right-side decorations border for often used
  controls:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/general/sogui-class-example.png">
  </center>

  The %SoWin library contains several such high-level classes as the
  SoWinExaminerViewer used in the above example. These are primarily
  used for doing Rapid Application Development (RAD) of new concepts
  and ideas for your 3D system.  The "real" application will typically
  use one of the lower-complexity classes higher up in the inheritance
  hierarchy, such as the SoWinRenderArea, which provides the
  application programmer with full control over the user interface
  components and general layout to present for the end-user, as
  suitable for the specific application needs.

  This is how the %SoWin library fits in with the other system
  components:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/mainpage/sowin-boxology.png">
  </center>

  As can be seen from the above figure, %SoWin builds on Systems in
  Motion's \COIN library for the 3D graphics, and the standard
  Microsoft Windows Win32 API for the 2D user interface components.

  The additional functionality provided by %SoWin over \COIN is:

  <ul>

  <li>The most convenient management of OpenGL context types, such as
  singlebuffered versus doublebuffered rendering, the use of overlay
  planes, stereo rendering, etc. This is handled through the
  SoWinGLWidget class and through the SoWinRenderArea class (which
  contains the main binding into the \COIN library's main data
  structures).</li>

  <li>The translation of native Win32 interaction device events (from
  e.g. the mouse or the keyboard) into the \COIN library's event
  types. The translation is done by the SoWinDevice classes,
  controlled by the SoWinRenderArea.

  These "generic" \COIN events are then passed into the 3D scenegraph
  for further processing, for instance by \COIN's 3D user interaction
  components -- like this "trackball manipulator" attached to a simple
  cone:

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/trackball-cone.png">
  </center>
  </li>

  <li>Some abstract viewer classes, like the SoWinViewer and
  SoWinFullViewer, which provides additional services on top of the
  SoWinRenderArea for assisting the application programmer in
  convenient handling of cameras and lightsources in the 3D scene (by
  the SoWinViewer), plus adding the basic, common user interface
  components (by the SoWinFullViewer).</li>

  <li>A set of high-level viewer classes, as has been presented by the
  SoWinExaminerViewer in the above source code example. There are
  currently three different non-abstract viewer classes to choose
  from: the SoWinExaminerViewer (a plain model viewer), the
  SoWinFlyViewer (for fly-throughs in larger 3D scenes) and the
  SoWinPlaneViewer (for CAD-style viewing and interaction along the 3
  principal axes).</li>

  </ul>

  For those who are using the implementation of the Inventor API from
  SGI, we would like to point out that %SoWin can also be used on top
  of that library instead of the \COIN library from Kongsberg Oil &
  Gas Technologies.

  The %SoWin API is based on and closely matches the InventorXt
  library API, originally developed by SGI. This should make it
  straigthforward to port simple InventorXt code over to %SoWin, for
  instance to gain greater portability.

  For more information about the Coin3D family of libraries, including
  licensing information, please visit the Coin3D web site
  <http://www.coin3d.org>.

  \sa The documentation for the \COIN library: <http://doc.coin3d.org/Coin>.
*/

// *************************************************************************

#include <limits.h>
#include <Inventor/SbTime.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoMemoryError.h>

#include <Inventor/Win/Win32API.h>
#include <sowindefs.h> // SOWIN_STUB
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoGuiP.h>
#include <Inventor/Win/devices/SoWinDevice.h>
#include <Inventor/Win/SoWinComponent.h>
#include <Inventor/Win/SoAny.h>

#undef ERROR // workaround for MS Visual C++ stupidity; this is an
             // enum in SoDebugError.h, but also a #define somewhere
             // in the MSVS system headers

// *************************************************************************

// The private data for the SoWin class.

class SoWinP {
  
public:
  static BOOL CALLBACK sizeChildProc(HWND window, LPARAM lparam);
  static void errorHandlerCB(const SoError * error, void * data);
  static LRESULT CALLBACK eventHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static SbBool pipeErrorMessagesToConsole(void);

  static UINT_PTR timerSensorId;
  static void CALLBACK timerSensorCB(HWND window,
                                     UINT message,
                                     UINT idevent,
                                     DWORD dwtime);

  static UINT_PTR delaySensorId;
  static void CALLBACK delaySensorCB(HWND window,
                                     UINT message,
                                     UINT idevent,
                                     DWORD dwtime);

  static UINT_PTR idleSensorId;
  static void CALLBACK idleSensorCB(HWND window,
                                    UINT message,
                                    UINT idevent,
                                    DWORD dwtime);
  
  static LRESULT onClose(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT onDestroy(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT onQuit(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  
  static HINSTANCE Instance;
  static HWND mainWidget;
  static char * appName;
  static char * className;

  static SbBool useParentEventHandler;
  static WNDPROC parentEventHandler;
  static UINT focusMessage;
  static SbBool hasFocus;
  static HWND savedFocus;
  static int DEBUG_LISTMODULES;

  static bool InitRawDevices(void);

private:
  SoWin * owner;
  
};

// *************************************************************************

HINSTANCE SoWinP::Instance = NULL;
HWND SoWinP::mainWidget = NULL;
char * SoWinP::appName = NULL;
char * SoWinP::className = NULL;
WNDPROC SoWinP::parentEventHandler = NULL;
SbBool SoWinP::useParentEventHandler = TRUE;
UINT SoWinP::focusMessage = 0xffffffff;
SbBool SoWinP::hasFocus = FALSE;
HWND SoWinP::savedFocus = NULL;

/* value 0 signifies "inactive": */
UINT_PTR SoWinP::timerSensorId = 0;
UINT_PTR SoWinP::delaySensorId = 0;
UINT_PTR SoWinP::idleSensorId = 0;

#define ENVVAR_NOT_INITED INT_MAX

int SoWinP::DEBUG_LISTMODULES = ENVVAR_NOT_INITED;

// *************************************************************************

// init()-method documented in common/SoGuiCommon.cpp.in.
HWND
SoWin::init(int & argc, char ** argv,
            const char * appname, const char * classname)
{
  if (appname)
    SoWinP::appName = strcpy(new char [ strlen(appname) + 1 ], appname);
  if (classname)
    SoWinP::className = strcpy(new char [ strlen(classname) + 1 ], classname);

  {
    WNDCLASS windowclass;
    LPCTSTR icon = MAKEINTRESOURCE(IDI_APPLICATION);
    HBRUSH brush = (HBRUSH) GetSysColorBrush(COLOR_BTNFACE);
    windowclass.lpszClassName = classname;
    windowclass.hInstance = SoWinP::Instance;
    windowclass.lpfnWndProc = SoWinP::eventHandler;
    windowclass.style = CS_OWNDC;
    windowclass.lpszMenuName = NULL;
    windowclass.hIcon = LoadIcon(NULL, icon);
    windowclass.hCursor = Win32::LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = brush;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = sizeof(LONG_PTR);
    (void)Win32::RegisterClass(&windowclass);
  }
  
  SIZE size = { 500, 500 };
  HWND toplevel =
    Win32::CreateWindowEx_(NULL, // exstyle
                           classname,
                           appname,
                           WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // style
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           size.cx,
                           size.cy,
                           NULL, // parent
                           NULL, // menu
                           NULL,
                           NULL);

  SoWinP::useParentEventHandler = FALSE;
  
  SoWin::init(toplevel);
  
  return toplevel;
}


// init()-method documented in common/SoGuiCommon.cpp.in.
void
SoWin::init(HWND toplevelwidget)
{
  SoGuiP::commonInit();
  SoWinP::InitRawDevices();

  // FIXME: should set on all! SoReadError will just disappear with
  // this.. 20050714 mortene.
  SoDebugError::setHandlerCallback(SoWinP::errorHandlerCB, NULL);

  // This should prove helpful for debugging the pervasive problem
  // under Win32 with loading multiple instances of the same library.
  if (SoWinP::DEBUG_LISTMODULES == ENVVAR_NOT_INITED) {
    const char * env = SoAny::si()->getenv("SOGUI_DEBUG_LISTMODULES");
    SoWinP::DEBUG_LISTMODULES = env ? atoi(env) : 0;
    if (SoWinP::DEBUG_LISTMODULES) { SoAny::listWin32ProcessModules(); }
  }

  SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged, NULL);
  if (IsWindow(toplevelwidget)) 
    SoWinP::mainWidget = toplevelwidget;

  if (SoWinP::focusMessage == 0xffffffff) {
    // note: windows will unregister message automatically
    SoWinP::focusMessage = RegisterWindowMessage("SoWin_set_focus_message");
    assert(SoWinP::focusMessage != 0 && "RegisterWindowMessage failed.");
  }

  if (SoWinP::useParentEventHandler) {
    SoWinP::parentEventHandler = (WNDPROC)
      Win32::GetWindowLongPtr(toplevelwidget, GWLP_WNDPROC);
    (void)Win32::SetWindowLongPtr(toplevelwidget, GWLP_WNDPROC, (LONG_PTR) SoWinP::eventHandler);
  }
  
}

// documented in common/SoGuiCommon.cpp.in
void
SoWin::mainLoop(void)
{
  MSG msg;
  while (TRUE) {
    // FIXME: shouldn't this be a while loop, i.e. process all
    // messages, before doing the idle tasks? 20040721 mortene.
    if (GetQueueStatus(QS_ALLINPUT) != 0) { // if messagequeue != empty
      BOOL bRet;
      if ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){ // if msg != WM_QUIT
        if (bRet == -1){
          // FIXME: handle the error and possibly exit
          break;
        }
        else {
          TranslateMessage(&msg); 
          DispatchMessage(&msg); 
        }
      }
      else break; // msg == WM_QUIT
    }
    else if (SoWinP::idleSensorId != 0) {
      doIdleTasks();
    }
    else {
      WaitMessage();
    }
  }
// Disabled invocation of SoWin::done(), since this calls SoDB::finish(),
// which means we would run into issues doing the usual...
//    SoWin::mainLoop();
//    root->unref();
}

// documented in common/SoGuiCommon.cpp.in
void
SoWin::exitMainLoop(void)
{
  PostQuitMessage(0);
}

// documented in common/SoGuiCommon.cpp.in
void
SoWin::done(void)
{
  // FIXME: should clean up *all* resources still dangling
  // about. -mortene.

  //FIXME: Test to make sure cleanup is not done more than once? -wiesener

  SoDB::getSensorManager()->setChangedCallback(NULL, NULL);
  if (SoWinP::idleSensorId != 0) Win32::KillTimer(NULL, SoWinP::idleSensorId);
  if (SoWinP::timerSensorId != 0) Win32::KillTimer(NULL, SoWinP::timerSensorId);
  if (SoWinP::delaySensorId != 0) Win32::KillTimer(NULL, SoWinP::delaySensorId);

  SoWinP::idleSensorId = SoWinP::timerSensorId = SoWinP::delaySensorId = 0;

  if (!SoWinP::useParentEventHandler){
    //destroy the window if we created it
    DestroyWindow(SoWinP::mainWidget);
    Win32::UnregisterClass(SoWinP::className, NULL);
  }
  else {
    //set the message handler back to the previous value
    //FIXME: We should make sure that this is not be done from within the message handler.
    (void)Win32::SetWindowLongPtr(SoWinP::mainWidget, GWLP_WNDPROC, (LONG_PTR)SoWinP::parentEventHandler);
  }
  SoGuiP::commonCleanup();

  //Added a call to SoDB::finish here to behave more like SoQt.
  SoDB::finish();
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call ShowWindow() with
  the argument SW_SHOW on the provided \a widget window handle.

  \sa hide()
*/
void
SoWin::show(HWND const widget)
{
  (void)ShowWindow(widget, SW_SHOW);
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call ShowWindow() with
  the argument SW_HIDE on the provided \a widget window handle.

  \sa show()
*/
void
SoWin::hide(HWND const widget)
{
  (void)ShowWindow(widget, SW_HIDE);
}

/*!
  This method is provided for easier porting of applications based on
  the Open Inventor SoXt component classes. It will call
  SetWindowPos() on the provided \a widget window handle.

  \sa getWidgetSize()
*/
void
SoWin::setWidgetSize(HWND widget, const SbVec2s size)
{
  UINT flags = SWP_NOMOVE | SWP_NOZORDER;
  Win32::SetWindowPos(widget, NULL, 0, 0, size[0], size[1], flags);
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call GetWindowExtEx()
  on the provided \a widget window handle's device context (returning
  an SbVec2s).

  \sa setWidgetSize()
*/
SbVec2s
SoWin::getWidgetSize(HWND widget)
{
  HDC hdc = GetDC(widget);

  SIZE size;
  if (! GetWindowExtEx(hdc, & size)) {
    size.cx = -1;
    size.cy = -1;
  }
  return SbVec2s((short) size.cx, (short) size.cy);
}

// Documented in common/SoGuiCommon.cpp.in.
HWND
SoWin::getTopLevelWidget(void)
{
  return SoWinP::mainWidget;
}

// Documented in common/SoGuiCommon.cpp.in.
//
// Just a simple wrap-around for the Win32 API MessageBox() call,
// provided for easier porting from applications using the Open
// Inventor SoXt component classes.
void
SoWin::createSimpleErrorDialog(HWND const widget,
                               const char * title,
                               const char * string1,
                               const char * string2)
{
  
  SbString t(title ? title : "");
  SbString errstr(string1 ? string1 : "");

  if (string2) {
    errstr += '\n';
    errstr += string2;
  }

  MessageBox(widget, errstr.getString(), t.getString(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
}

/*!
  Returns a pointer to the HWND which is the top level widget for the
  given HWND \a hwnd.

  \sa getTopLevelWidget()
*/
HWND
SoWin::getShellWidget(HWND hwnd)
{
  HWND parent = hwnd;
  
  do {
    hwnd = parent;
    LONG_PTR style = Win32::GetWindowLongPtr(hwnd, GWL_STYLE);
    // FIXME: this check seems bogus. 20020521 mortene.
    if (style & WS_OVERLAPPEDWINDOW) break;
    parent = GetParent(hwnd);
  } while(IsWindow(parent));
  
  return hwnd;
}

/*!
	Process of all Inventor sensors scheduled for execution when the
	system is idle.
*/
void
SoWin::doIdleTasks(void)
{
  SoWinP::idleSensorCB(0, 0, 0, 0);
}

bool 
SoWinP::InitRawDevices(void) 
{
  PRAWINPUTDEVICELIST rawInputDeviceList;
  PRAWINPUTDEVICE rawInputDevices;
  int usagePage1Usage8Devices;
  
  // Find the Raw Devices
  UINT nDevices;
  // Get Number of devices attached
  if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) { 
    return FALSE;
  }
  // Create list large enough to hold all RAWINPUTDEVICE structs
  rawInputDeviceList = (PRAWINPUTDEVICELIST) malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);
  if (rawInputDeviceList == NULL) {
    return FALSE;
  }
  // Now get the data on the attached devices
  if (GetRawInputDeviceList(rawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == -1) {
    return FALSE;
  }
  
  rawInputDevices = (PRAWINPUTDEVICE) malloc(nDevices * sizeof(RAWINPUTDEVICE));
  usagePage1Usage8Devices = 0;
  
  // Look through device list for RIM_TYPEHID devices with UsagePage == 1, Usage == 8
  for(UINT i=0; i<nDevices; i++) {
    if (rawInputDeviceList[i].dwType == RIM_TYPEHID) {
      UINT nchars = 300;
      TCHAR deviceName[300];      
      RID_DEVICE_INFO dinfo;
      UINT sizeofdinfo = sizeof(dinfo);
      dinfo.cbSize = sizeofdinfo;
      if (GetRawInputDeviceInfo(rawInputDeviceList[i].hDevice,
                                RIDI_DEVICEINFO, &dinfo, &sizeofdinfo ) >= 0) {
        if (dinfo.dwType == RIM_TYPEHID) {
          RID_DEVICE_INFO_HID *phidInfo = &dinfo.hid;          
          // Add this one to the list of interesting devices?
          // Actually only have to do this once to get input from all
          // usage 1, usagePage 8 devices This just keeps out the
          // other usages.  You might want to put up a list for users
          // to select amongst the different devices.  In particular,
          // to assign separate functionality to the different
          // devices.
          if (phidInfo->usUsagePage == 1 && phidInfo->usUsage == 8) {
            rawInputDevices[usagePage1Usage8Devices].usUsagePage = phidInfo->usUsagePage;
            rawInputDevices[usagePage1Usage8Devices].usUsage = phidInfo->usUsage;
            rawInputDevices[usagePage1Usage8Devices].dwFlags = 0;
            rawInputDevices[usagePage1Usage8Devices].hwndTarget = mainWidget;//NULL;
            usagePage1Usage8Devices++;
          }
        }
      }
    }
  }
  
  // Register for input from the devices in the list
  if (RegisterRawInputDevices(rawInputDevices, usagePage1Usage8Devices, 
                              sizeof(RAWINPUTDEVICE)) == FALSE) {
    return FALSE;
  }
  
  return TRUE;
}

// Return value of SOWIN_MSGS_TO_CONSOLE environment variable.
SbBool
SoWinP::pipeErrorMessagesToConsole(void)
{
  static const char * conmsgs = SoAny::si()->getenv("SOWIN_MSGS_TO_CONSOLE");
  int val = conmsgs ? atoi(conmsgs) : 0;
  return val > 0;
}

void
SoWinP::errorHandlerCB(const SoError * error, void * data)
{
  // Normally, errors, warnings and info goes to a dialog box in
  // SoWin, but during development it might be better to pipe it to
  // the console.
  if (SoWinP::pipeErrorMessagesToConsole()) {
    (void)printf("%s\n", error->getDebugString().getString());
  }
  else {
    const SbString debugstring = error->getDebugString();
    const char * title = "Message";
    UINT icontype = MB_ICONERROR;

    if (error->isOfType(SoReadError::getClassTypeId())) {
      title = "File import problem";
    }
    else if (error->isOfType(SoMemoryError::getClassTypeId())) {
      title = "Memory resource problem";
    }
    else if (error->isOfType(SoDebugError::getClassTypeId())) {
      switch (((SoDebugError *)error)->getSeverity()) {
      case SoDebugError::ERROR:
        title = "Error";
        break;
      case SoDebugError::WARNING:
        title = "Warning";
        icontype = MB_ICONWARNING;
        break;
      case SoDebugError::INFO:
        title = "Information";
        icontype = MB_ICONINFORMATION;
        break;
      default: assert(FALSE && "unknown severity"); break;
      }
    }
    
    // Note that the messagebox dialog is not entirely modal: event
    // queue handling is still done for the application's
    // windows. E.g. WM_PAINT-messages will still "slip through", so
    // the rendering canvas will continue to update upon expose
    // events, for instance.
    MessageBox(NULL, (LPCTSTR) debugstring.getString(), title,
               MB_OK | MB_TASKMODAL | icontype);
  }
}

LRESULT CALLBACK
SoWinP::eventHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  LRESULT retval = 0;
  BOOL handled = FALSE;

  if (message == focusMessage && hasFocus && savedFocus) {
    HWND r = SetFocus(savedFocus);
    assert(r && "SetFocus() failed.");
    savedFocus = NULL;
    return 0;
  }

  switch (message) {
  // WM_ACTIVATE message is necessary for restoring correct focus after the application
  // is ALT-tab-bed to background and brought to foreground again, or when message dialog
  // appears and is closed. In all these cases, the focus may be lost.
  case WM_ACTIVATE:
    
    // FIXME: Will this code handle multiple top-level windows? PCJohn-2006-09-26
    if (LOWORD(wparam) != WA_INACTIVE) {
      
      // mainWindget is activated
      assert(hasFocus == FALSE && "Something is wrong with Windows.");
      hasFocus = TRUE;
      
      if (savedFocus)
        if (PostMessage(window, focusMessage, (WPARAM)savedFocus, 0) == 0)
          assert(0);
    
    } else {
      
      // mainWindget is deactivated
      assert(hasFocus == TRUE && "Something is wrong with Windows.");
      hasFocus = FALSE;
      
      if (savedFocus == NULL)
        savedFocus = GetFocus();
    
    }
    break;

  case WM_CLOSE:
    if (! SoWinP::useParentEventHandler) {
      retval = SoWinP::onClose(window, message, wparam, lparam);
      handled = TRUE;
    }
    break;

  case WM_DESTROY:
    if (! SoWinP::useParentEventHandler) {
      retval = SoWinP::onDestroy(window, message, wparam, lparam);
      handled = TRUE;
    }
    break;
            
  case WM_QUIT:
    //This location will normally not be reached, as the mainLoop filters out 
    //the WM_QUIT message before pumping it through the loop. The exception
    //is if the user has her own main loop.
    retval = SoWinP::onQuit(window, message, wparam, lparam);
    handled = TRUE;
    break;
  }

  if (SoWinP::useParentEventHandler && SoWinP::parentEventHandler)
    return CallWindowProc(SoWinP::parentEventHandler, window, message, wparam, lparam);

  if (handled)
    return retval;
    
  return DefWindowProc(window, message, wparam, lparam);
}

///////////////////////////////////////////////////////////////////
//
//  (private)
//

void CALLBACK
SoWinP::timerSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
  SoDebugError::postInfo("SoWin::timerSensorCB", "called");
#endif // SOWIN_DEBUG
  SoDB::getSensorManager()->processTimerQueue();
  SoGuiP::sensorQueueChanged(NULL);
}

void CALLBACK
SoWinP::delaySensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
  SoDebugError::postInfo("SoWin::delaySensorCB", "called");
#endif // SOWIN_DEBUG
  SoDB::getSensorManager()->processDelayQueue(FALSE);
  SoGuiP::sensorQueueChanged(NULL);
}

void CALLBACK
SoWinP::idleSensorCB(HWND window, UINT message, UINT idevent, DWORD dwtime)
{
#if SOWIN_DEBUG && 0
  SoDebugError::postInfo("SoWin::idleSensorCB", "called");
#endif // SOWIN_DEBUG
  SoDB::getSensorManager()->processTimerQueue();
  SoDB::getSensorManager()->processDelayQueue(TRUE); // isidle = TRUE
  SoGuiP::sensorQueueChanged(NULL);
}

LRESULT 
SoWinP::onClose(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  PostQuitMessage(0);
  return 0;
}

LRESULT
SoWinP::onDestroy(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  return 0;
}

LRESULT
SoWinP::onQuit(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  return 0;
}

#ifndef DOXYGEN_SKIP_THIS

// This function gets called whenever something has happened to any of
// the sensor queues. It starts or reschedules a timer which will
// trigger when a sensor is ripe for plucking.
void
SoGuiP::sensorQueueChanged(void * cbdata)
{
  SoSensorManager * sensormanager = SoDB::getSensorManager();

  SbTime timevalue;
  if (sensormanager->isTimerSensorPending(timevalue)) {
    SbTime interval = timevalue - SbTime::getTimeOfDay();

    if (interval.getValue() < 0.0) interval.setValue(0.0);
    if (SoWinP::timerSensorId != 0) Win32::KillTimer(NULL, SoWinP::timerSensorId);
    
    SoWinP::timerSensorId =
      Win32::SetTimer(NULL,
                      /* ignored because of NULL first argument: */ 0,
                      interval.getMsecValue(),
                      (TIMERPROC)SoWinP::timerSensorCB);
  }
  else if (SoWinP::timerSensorId != 0) {
    Win32::KillTimer(NULL, SoWinP::timerSensorId);
    SoWinP::timerSensorId = 0;
  }

  if (sensormanager->isDelaySensorPending()) {
        
    if (SoWinP::idleSensorId == 0) {
      SoWinP::idleSensorId =
        Win32::SetTimer(NULL,
                        /* ignored because of NULL first argument: */ 0,

                        // FIXME: this seems like a rather bogus way
                        // of setting up a timer to check when the
                        // system goes idle. Should investigate how
                        // this actually works, and perhaps if there
                        // is some other mechanism we could
                        // use. 20040721 mortene.
                        0,

                        (TIMERPROC)SoWinP::idleSensorCB);
    }

    if (SoWinP::delaySensorId == 0) {
      unsigned long timeout = SoDB::getDelaySensorTimeout().getMsecValue();
      SoWinP::delaySensorId =
        Win32::SetTimer(NULL,
                        /* ignored because of NULL first argument: */ 0,
                        timeout,
                        (TIMERPROC)SoWinP::delaySensorCB);
    }
  }
  else {
    if (SoWinP::idleSensorId != 0) {
      Win32::KillTimer(NULL, SoWinP::idleSensorId);
      SoWinP::idleSensorId = 0;
    }

    if (SoWinP::delaySensorId != 0) {
      Win32::KillTimer(NULL, SoWinP::delaySensorId);
      SoWinP::delaySensorId = 0;
    }
  }
}



#endif // !DOXYGEN_SKIP_THIS
