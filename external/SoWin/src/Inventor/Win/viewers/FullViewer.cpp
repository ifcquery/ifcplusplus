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

// Class is documented in common/viewers/SoGuiFullViewer.cpp.in.

// *************************************************************************

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/threads/SbStorage.h>

#include <sowindefs.h>
#include <Inventor/Win/Win32API.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/widgets/SoWinBitmapButton.h>
#include <Inventor/Win/widgets/WinNativePopupMenu.h>
#include <Inventor/Win/viewers/SoWinFullViewer.h>
#include <Inventor/Win/viewers/SoWinFullViewerP.h>

#include <float.h> // FLT_MAX

// Button icons.
#include <Inventor/Win/common/pixmaps/pick.xpm>
#include <Inventor/Win/common/pixmaps/view.xpm>
#include <Inventor/Win/common/pixmaps/home.xpm>
#include <Inventor/Win/common/pixmaps/set_home.xpm>
#include <Inventor/Win/common/pixmaps/view_all.xpm>
#include <Inventor/Win/common/pixmaps/seek.xpm>

static const int DECORATION_SIZE = 30;
static const int DECORATION_BUFFER = 5;

#define PRIVATE(o) (o->pimpl)
#define PUBLIC(o) (o->pub)

// *************************************************************************

SOWIN_OBJECT_ABSTRACT_SOURCE(SoWinFullViewer);

// *************************************************************************

static void hookhandle_constructor(void * closure)
{
  HHOOK * hookhandle = (HHOOK *) closure;
  *hookhandle = NULL;
}

static void hookhandle_destructor(void *)
{
  
}

SoWinFullViewer::SoWinFullViewer(HWND parent,
                                 const char * name,
                                 SbBool embedded,
                                 BuildFlag flag,
                                 SoWinViewer::Type type,
                                 SbBool build) :
  inherited(parent, name, embedded, type, FALSE)
{
  SoWinFullViewerP::nrinstances++;
  this->pimpl = new SoWinFullViewerP(this);


  if (SoWinFullViewerP::hookhandle == NULL) {
    SoWinFullViewerP::hookhandle = new SbStorage(sizeof(HHOOK),
                                                 hookhandle_constructor,
                                                 hookhandle_destructor);
  }

  HHOOK * hookhandle = (HHOOK *) SoWinFullViewerP::hookhandle->get();
  if (!(*hookhandle)) {
    *hookhandle = Win32::SetWindowsHookEx(WH_CALLWNDPROC,
                                          (HOOKPROC)SoWinFullViewerP::systemEventHook,
                                          NULL, GetCurrentThreadId());
  }

  if (SoWinFullViewerP::parentHWNDmappings == NULL) {
    SoWinFullViewerP::parentHWNDmappings = new SbDict;
  }

  PRIVATE(this)->viewerwidget = NULL;
  PRIVATE(this)->renderareawidget = NULL;

  PRIVATE(this)->menuenabled = (flag & SoWinFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  PRIVATE(this)->decorations = (flag & SoWinFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  this->prefmenu = NULL;

  PRIVATE(this)->leftthumbwheel = NULL;
  PRIVATE(this)->bottomthumbwheel = NULL;
  PRIVATE(this)->rightthumbwheel = NULL;
  this->leftWheel = NULL;
  this->bottomWheel = NULL;
  this->rightWheel = NULL;
  this->rightWheelStr = NULL;

  // Let these be bogus until we actually set up the wheels.
  this->leftWheelVal = FLT_MAX;
  this->bottomWheelVal = FLT_MAX;
  this->rightWheelVal = FLT_MAX;

  if (build) {
    this->setClassName("SoWinFullViewer");
    this->setBaseWidget(this->buildWidget(this->getParentWidget()));
  }

  /* FIXME: We should use the actual size of the parent window instead
     of hardcoding the size here, at least if the size of the parent
     window is reasonable. See also
     SoWinExaminerViewerP::constructor(). 2004-01-08 thammer.  
  */
  this->setSize(SbVec2s(500, 420));

  if (! this->isViewing())
    this->setViewing(TRUE);
}

SoWinFullViewer::~SoWinFullViewer()
{
  (void)SoWinFullViewerP::parentHWNDmappings->remove((SbDict::Key)this->getParentWidget());

  SoWinFullViewerP::nrinstances--;
  if (SoWinFullViewerP::nrinstances == 0) {
    // Parent HWND -> SoWinFullViewer dict.
    delete SoWinFullViewerP::parentHWNDmappings;
    SoWinFullViewerP::parentHWNDmappings = NULL;

    HHOOK * hookhandle = (HHOOK *) SoWinFullViewerP::hookhandle->get();
    if (*hookhandle) {
      Win32::UnhookWindowsHookEx(*hookhandle);
    }

    delete SoWinFullViewerP::hookhandle;
    SoWinFullViewerP::hookhandle = NULL;

  }
  
  delete this->prefmenu;

  delete PRIVATE(this);
}

void
SoWinFullViewer::setDecoration(SbBool enable)
{
#if SOWIN_DEBUG
  if ((enable && this->isDecoration()) ||
       (! enable && ! this->isDecoration())) {
    SoDebugError::postWarning("SoWinFullViewer::setDecoration",
                               "decorations already turned %s",
                              enable ? "on" : "off");
    return;
  }
#endif // SOWIN_DEBUG

  PRIVATE(this)->decorations = enable;
  PRIVATE(this)->showDecorationWidgets(enable);

  // reposition all widgets
  RECT rect;
  Win32::GetClientRect(this->getParentWidget(), & rect);
  PRIVATE(this)->layoutWidgets(rect.right, rect.bottom);
  if (enable) {
    rect.right -= DECORATION_SIZE * 2;
    rect.bottom -= DECORATION_SIZE;
  }
  SoWinRenderArea::sizeChanged(SbVec2s((short)rect.right, (short)rect.bottom));
  Win32::InvalidateRect(this->getParentWidget(), NULL, TRUE);
}

SbBool
SoWinFullViewer::isDecoration(void) const
{
  return PRIVATE(this)->decorations;
}

void
SoWinFullViewer::setPopupMenuEnabled(SbBool enable)
{
#if SOWIN_DEBUG
  if ((enable && this->isPopupMenuEnabled()) ||
       (! enable && ! this->isPopupMenuEnabled())) {
    SoDebugError::postWarning("SoWinFullViewer::setPopupMenuEnabled",
                               "popup menu already turned %s",
                               enable ? "on" : "off");
    return;
  }
#endif // SOWIN_DEBUG
  PRIVATE(this)->menuenabled = enable;
}

SbBool
SoWinFullViewer::isPopupMenuEnabled(void) const
{
  return PRIVATE(this)->menuenabled;
}

HWND
SoWinFullViewer::getAppPushButtonParent(void) const
{
  return PRIVATE(this)->viewerwidget;
}

void
SoWinFullViewer::addAppPushButton(HWND newButton)
{
  PRIVATE(this)->lefttrimbuttons.append(newButton);
}

void
SoWinFullViewer::insertAppPushButton(HWND newButton, int index)
{
  PRIVATE(this)->lefttrimbuttons.insert(newButton, index);
}

void
SoWinFullViewer::removeAppPushButton(HWND oldButton)
{
  int index = PRIVATE(this)->lefttrimbuttons.find(oldButton);
  PRIVATE(this)->lefttrimbuttons.remove(index);
}

int
SoWinFullViewer::findAppPushButton(HWND oldButton) const
{
  return PRIVATE(this)->lefttrimbuttons.find(oldButton);
}

int
SoWinFullViewer::lengthAppPushButton(void) const
{
  return PRIVATE(this)->lefttrimbuttons.getLength();
}

HWND
SoWinFullViewer::getRenderAreaWidget(void) const
{
  return PRIVATE(this)->renderareawidget;
}

// Doc in superclass.
void
SoWinFullViewer::setComponentCursor(const SoWinCursor & cursor)
{
  // Overridden to apply the new cursor only for the rendering canvas
  // widget. Otherwise, the default SoWinComponent setComponentCursor()
  // method will set the cursor for the top-most parent widget, which
  // makes it affect all sub-widgets, like the decorations stuff.

  PRIVATE(this)->cursor = cursor;
  SoWinComponent::setWidgetCursor(this->getRenderAreaWidget(), cursor);
}

// Documented in superclass.
void
SoWinFullViewer::setViewing(SbBool enable)
{
  if ((enable && this->isViewing()) || (! enable && ! this->isViewing())) {
#if SOWIN_DEBUG
    SoDebugError::postWarning("SoWinFullViewer::setViewing", "view mode already %s",
                               enable ? "on" : "off");
#endif // SOWIN_DEBUG
    return;
  }

  inherited::setViewing(enable);

  if (PRIVATE(this)->viewerButton(SoWinFullViewerP::VIEWERBUTTON_VIEW) != NULL) {
    PRIVATE(this)->viewerButton(SoWinFullViewerP::VIEWERBUTTON_VIEW)->setPressedState(enable);
    PRIVATE(this)->viewerButton(SoWinFullViewerP::VIEWERBUTTON_PICK)->setPressedState(enable ? FALSE : TRUE);
    PRIVATE(this)->viewerButton(SoWinFullViewerP::VIEWERBUTTON_SEEK)->setEnabled(enable);
  }
}

///////////////////////////////////////////////////////////////////
//
//  (protected)
//
//

// Documented in superclass.
HWND
SoWinFullViewer::buildWidget(HWND parent)
{
  // This method will always be called with a parent.

  assert(IsWindow(parent));
  
  SoWinFullViewerP::parentHWNDmappings->enter((SbDict::Key)parent, this);

  PRIVATE(this)->viewerwidget = parent;
  PRIVATE(this)->renderareawidget = inherited::buildWidget(parent);
  assert(IsWindow(PRIVATE(this)->renderareawidget));

  // Change default cursor from pointer arrow, to *no* default
  // cursor. This must be done for the SetCursor()-call in
  // SoWinFullViewerP::systemEventHook() to work even when the canvas
  // has not grabbed the mouse.
  //
  (void)Win32::SetClassLongPtr(this->getGLWidget(), GCLP_HCURSOR, 0);
  
  if (PRIVATE(this)->menuenabled) { this->buildPopupMenu(); }
  if (PRIVATE(this)->decorations) { this->buildDecoration(parent); }

  return PRIVATE(this)->renderareawidget;
}

// doc in super
void
SoWinFullViewer::sizeChanged(const SbVec2s & size)
{
  if (! IsWindow(this->getBaseWidget())) return;
  
  if (PRIVATE(this)->decorations) {
    SoWinRenderArea::sizeChanged(SbVec2s(size[0] - (2 * DECORATION_SIZE),
                                         size[1] - DECORATION_SIZE));
  }
  else {
    SoWinRenderArea::sizeChanged(size);
  }
  PRIVATE(this)->layoutWidgets(size[0], size[1]);
  Win32::InvalidateRect(this->getParentWidget(), NULL, TRUE);
}

void
SoWinFullViewer::buildDecoration(HWND parent)
{
  this->leftWheel = PRIVATE(this)->buildLeftWheel(parent);
  this->bottomWheel = PRIVATE(this)->buildBottomWheel(parent);
  this->rightWheel = PRIVATE(this)->buildRightWheel(parent);

  (void)this->buildViewerButtons(parent);
}

HWND
SoWinFullViewer::buildViewerButtons(HWND parent)
{
  this->createViewerButtons(parent, & PRIVATE(this)->righttrimbuttons);

  // Now position the buttons.

  RECT rect;
  Win32::GetClientRect(parent, &rect);
  const int x = rect.right - DECORATION_SIZE;

  for (int i=0; i < PRIVATE(this)->righttrimbuttons.getLength(); i++) {
    SoWinBitmapButton * b = (SoWinBitmapButton *)
      PRIVATE(this)->righttrimbuttons[i];
    b->move(x, i * DECORATION_SIZE, DECORATION_SIZE, DECORATION_SIZE);
  }

  return parent;
}

void
SoWinFullViewer::buildPopupMenu(void)
{
  this->prefmenu = PRIVATE(this)->setupStandardPopupMenu();
}

void
SoWinFullViewer::openPopupMenu(const SbVec2s position)
{
  assert(this->prefmenu != NULL);
  PRIVATE(this)->prepareMenu(this->prefmenu);

  RECT clientrect;
  Win32::GetClientRect(PRIVATE(this)->renderareawidget, &clientrect);

  this->prefmenu->popUp(PRIVATE(this)->renderareawidget,
                        position[0], clientrect.bottom - position[1]);
}

void
SoWinFullViewer::setLeftWheelString(const char * const name)
{
  if (PRIVATE(this)->leftthumbwheel)
    PRIVATE(this)->leftthumbwheel->setLabelText(name);
}

void
SoWinFullViewer::setBottomWheelString(const char * const name)
{
  if (PRIVATE(this)->bottomthumbwheel)
    PRIVATE(this)->bottomthumbwheel->setLabelText(name);
}

const char *
SoWinFullViewer::getRightWheelString() const
{
  return this->rightWheelStr;
}

void
SoWinFullViewer::setRightWheelString(const char * const string)
{
  if (this->rightWheelStr)
    delete [] this->rightWheelStr;
  this->rightWheelStr = NULL;

  if (string)
    this->rightWheelStr = strcpy(new char [strlen(string)+1], string);

  if (PRIVATE(this)->rightthumbwheel) {
    PRIVATE(this)->rightthumbwheel->setLabelText(string);
  }
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

int SoWinFullViewerP::nrinstances = 0;
SbStorage * SoWinFullViewerP::hookhandle = NULL;
SbDict * SoWinFullViewerP::parentHWNDmappings = NULL;

HWND
SoWinFullViewerP::buildLeftWheel(HWND parent)
{
  // Create coords are not needed - the widget is moved into place
  // by layoutWidgets
  this->leftthumbwheel =
    new SoWinThumbWheel(SoWinThumbWheel::Vertical,
                        parent,
                        0,
                        0,
                        0,
                        "RotX");
  PUBLIC(this)->leftWheelVal = this->leftthumbwheel->value();
  this->leftthumbwheel->setCallback(this->leftWheelCB, this);
  this->leftthumbwheel->setRangeBoundaryHandling(SoWinThumbWheel::ACCUMULATE);
  this->leftthumbwheel->setLabelOffset(0,
                                          ((DECORATION_SIZE - this->leftthumbwheel->sizeHint().cx) / 2)
                                          + DECORATION_BUFFER + 1);

  return this->leftthumbwheel->getWidget();
}

HWND
SoWinFullViewerP::buildBottomWheel(HWND parent)
{
  // Create coords are not needed - the widget is moved into place
  // by layoutWidgets
  this->bottomthumbwheel =
    new SoWinThumbWheel(SoWinThumbWheel::Horizontal,
                        parent,
                        1,
                        0,
                        0,
                        "RotY");
  PUBLIC(this)->bottomWheelVal = this->bottomthumbwheel->value();
  this->bottomthumbwheel->setCallback(this->bottomWheelCB, this);
  this->bottomthumbwheel->setRangeBoundaryHandling(SoWinThumbWheel::ACCUMULATE);
  this->bottomthumbwheel->setLabelOffset(-4, 0);

  return this->bottomthumbwheel->getWidget();
}

HWND
SoWinFullViewerP::buildRightWheel(HWND parent)
{
  // Create coords are not needed - the widget is moved into place
  // by layoutWidgets
  this->rightthumbwheel =
    new SoWinThumbWheel(SoWinThumbWheel::Vertical,
                        parent,
                        2,
                        0,
                        0,
                        "Dolly");
  PUBLIC(this)->setRightWheelString("Dolly");
  PUBLIC(this)->rightWheelVal = this->rightthumbwheel->value();
  this->rightthumbwheel->setCallback(this->rightWheelCB, this);
  this->rightthumbwheel->setRangeBoundaryHandling(SoWinThumbWheel::ACCUMULATE);
  this->rightthumbwheel->setLabelOffset(- (this->bottomthumbwheel->getLabelSize().cx - this->rightthumbwheel->sizeHint().cx),
                                           ((DECORATION_SIZE - this->leftthumbwheel->sizeHint().cx) / 2)
                                           + DECORATION_BUFFER + 1);

  return this->rightthumbwheel->getWidget();
}

void
SoWinFullViewerP::seekbuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  that->seekbuttonClicked();
}

void
SoWinFullViewerP::seekbuttonClicked(void)
{
  PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoWinFullViewerP::leftWheelCB(SoWinThumbWheel::Interaction type, float val,
                              void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  if (type == SoWinThumbWheel::START) { PUBLIC(that)->leftWheelStart(); }
  else if (type == SoWinThumbWheel::END) { PUBLIC(that)->leftWheelFinish(); }
  else { PUBLIC(that)->leftWheelMotion(val); }
}

void
SoWinFullViewerP::bottomWheelCB(SoWinThumbWheel::Interaction type, float val,
                                void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  if (type == SoWinThumbWheel::START) { PUBLIC(that)->bottomWheelStart(); }
  else if (type == SoWinThumbWheel::END) { PUBLIC(that)->bottomWheelFinish(); }
  else { PUBLIC(that)->bottomWheelMotion(val); }
}

void
SoWinFullViewerP::rightWheelCB(SoWinThumbWheel::Interaction type, float val,
                               void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  if (type == SoWinThumbWheel::START) { PUBLIC(that)->rightWheelStart(); }
  else if (type == SoWinThumbWheel::END) { PUBLIC(that)->rightWheelFinish(); }
  else { PUBLIC(that)->rightWheelMotion(val); }
}

void
SoWinFullViewerP::interactbuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  that->viewerButton(SoWinFullViewerP::VIEWERBUTTON_PICK)->setPressedState(TRUE);
  that->viewerButton(SoWinFullViewerP::VIEWERBUTTON_VIEW)->setPressedState(FALSE);
  if (PUBLIC(that)->isViewing()) { PUBLIC(that)->setViewing(FALSE); }
}

void
SoWinFullViewerP::examinebuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  that->viewerButton(SoWinFullViewerP::VIEWERBUTTON_VIEW)->setPressedState(TRUE);
  that->viewerButton(SoWinFullViewerP::VIEWERBUTTON_PICK)->setPressedState(FALSE);
  if (!PUBLIC(that)->isViewing()) { PUBLIC(that)->setViewing(TRUE); }
}

void
SoWinFullViewerP::homebuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  PUBLIC(that)->resetToHomePosition();
}

void
SoWinFullViewerP::sethomebuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  PUBLIC(that)->saveHomePosition();
}

void
SoWinFullViewerP::viewallbuttonProc(SoWinBitmapButton * b, void * userdata)
{
  SoWinFullViewerP * that = (SoWinFullViewerP *)userdata;
  PUBLIC(that)->viewAll();
}

int
SoWinFullViewerP::layoutWidgets(int cx, int cy)
{
  int x, y, width, height, bottom, right, top;
  const int numViewerButtons = this->righttrimbuttons.getLength();
  const int numAppButtons = this->lefttrimbuttons.getLength();
  HWND renderArea = PUBLIC(this)->getBaseWidget();
  UINT flags = SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW;
  
  // RenderArea
  assert(IsWindow(renderArea));

  if (this->decorations) {
    Win32::SetWindowPos(renderArea, NULL, DECORATION_SIZE, 0, 0, 0, flags);
  }
  else {
    Win32::SetWindowPos(renderArea, NULL, 0, 0, 0, 0, flags);
    return 0;
  }
  
  // Viewer buttons
  int i;
  for(i = 0; i < numViewerButtons; i++)
    this->viewerButton(i)->move(cx - DECORATION_SIZE, DECORATION_SIZE * i);

  // App buttons
  for(i = 0; i < numAppButtons; i++) {
    Win32::MoveWindow(this->appButton(i),
                      0, DECORATION_SIZE * i,
                      DECORATION_SIZE, DECORATION_SIZE, TRUE);
  }

  // Wheels

  bottom = (cy - (DECORATION_SIZE + DECORATION_BUFFER));
  right = (cx - ((this->rightthumbwheel ? this->rightthumbwheel->getLabelSize().cx : 0) + 8));

  // Left wheel
  if (this->leftthumbwheel) {

    x = (DECORATION_SIZE / 2) - (this->leftthumbwheel->sizeHint().cx / 2) - 1;
    width = this->leftthumbwheel->sizeHint().cx;

    top = numAppButtons * DECORATION_SIZE + DECORATION_BUFFER;

    // if area is large enough for full height
    if ((bottom - top) > this->leftthumbwheel->sizeHint().cy) {

      height = this->leftthumbwheel->sizeHint().cy;

      y = bottom - height;

    } // else we must use all available space
    else {

      y = top;

      height = bottom - top;

    }

    this->leftthumbwheel->move(x, y, width, height);
  }

  // Bottom wheel
  if (this->bottomthumbwheel) {

    x += this->leftthumbwheel->getLabelSize().cx +
      this->bottomthumbwheel->getLabelSize().cx + 8;
    y = (cy - DECORATION_SIZE) +
      ((DECORATION_SIZE / 2) - (this->bottomthumbwheel->sizeHint().cy / 2) + 1);

    height = this->bottomthumbwheel->sizeHint().cy;

    if (right < (x + this->bottomthumbwheel->sizeHint().cx)) {

      width = right - x;

    }
    else {

      width =  this->bottomthumbwheel->sizeHint().cx;

    }

    this->bottomthumbwheel->move(x, y, width, height);

  }

  // Right wheel
  if (this->rightthumbwheel) {

    width = this->rightthumbwheel->sizeHint().cx;

    x = (cx - DECORATION_SIZE) +
      ((DECORATION_SIZE / 2) - (width / 2) + 1);

    top = numViewerButtons * DECORATION_SIZE + DECORATION_BUFFER;

    // if area is large enough for original height
    if ((bottom - top) > this->rightthumbwheel->sizeHint().cy) {

      height = this->rightthumbwheel->sizeHint().cy;

      y = bottom - height;

    } // else we must use all available space
    else {

      y = top;

      height = bottom - top;

    }

    this->rightthumbwheel->move(x, y, width, height);
  }

  return 0;
}

void
SoWinFullViewerP::showDecorationWidgets(SbBool enable)
{
  const int numViewerButtons = this->righttrimbuttons.getLength();
  const int numAppButtons = this->lefttrimbuttons.getLength();

  // Viewer buttons
  int i;
  for(i = 0; i < numViewerButtons; i++) {
    (void)ShowWindow(this->viewerButton(i)->getWidget(),
                     (enable ? SW_SHOW : SW_HIDE));
  }

  // App buttons
  for(i = 0; i < numAppButtons; i++) {
    (void)ShowWindow(this->appButton(i), (enable ? SW_SHOW : SW_HIDE));
  }

  // Thumbwheels
  if (enable) {
    this->leftthumbwheel->show();
    this->bottomthumbwheel->show();
    this->rightthumbwheel->show();
  }
  else {
    this->leftthumbwheel->hide();
    this->bottomthumbwheel->hide();
    this->rightthumbwheel->hide();
  }
}

LRESULT CALLBACK
SoWinFullViewerP::systemEventHook(int code, WPARAM wparam, LPARAM lparam)
{
  // FIXME: if I'm not mistaken, this message hook catches ~ all
  // messages in our process, for _all_ windows / widgets. This is
  // superbly inefficient, and should be cleaned up. 20030424 mortene.

  CWPSTRUCT * msg = (CWPSTRUCT *)lparam;

  void * comp;
  SbBool found =
    SoWinFullViewerP::parentHWNDmappings->find((SbDict::Key)msg->hwnd, comp);

  if (found) {
    SoWinFullViewer * object = (SoWinFullViewer *)comp;

    // According to MSVC++ Win32 API doc on CallWndProc(), if code<0
    // we should not do any processing -- just pass message along to
    // next hook.
    if (code >= 0) {
      // FIXME: if code==HC_ACTION the Win32 API doc says we _must_
      // process the message. Weird. Try to find out what that really
      // means. 20011126 mortene.

      switch (msg->message) {
      case WM_SETCURSOR:
        SoWinComponent::setWidgetCursor(object->getRenderAreaWidget(),
                                        PRIVATE(object)->cursor);
        break;

      case WM_LBUTTONDOWN:
      case WM_SETFOCUS:
	if ( object->isStealFocus() )
	  (void)Win32::SetFocus(object->getGLWidget());
        break;
      }
    }
  }

  HHOOK * hookhandle = (HHOOK *) SoWinFullViewerP::hookhandle->get();
  return CallNextHookEx(*hookhandle, code, wparam, lparam);
}

// *************************************************************************

void
SoWinFullViewer::createViewerButtons(HWND parent, SbPList * buttonlist)
{
  SoWinBitmapButton * button = new SoWinBitmapButton(parent, 24, "pick", NULL);
  button->addBitmap(pick_xpm);
  button->setBitmap(0); // use first (and only) bitmap
  button->registerClickedProc(SoWinFullViewerP::interactbuttonProc, PRIVATE(this));
  buttonlist->append(button);
  button->setPressedState(this->isViewing() ? FALSE : TRUE);

  button = new SoWinBitmapButton(parent, 24, "view", NULL);
  button->addBitmap(view_xpm);
  button->setBitmap(0); // use first (and only) bitmap
  button->registerClickedProc(SoWinFullViewerP::examinebuttonProc, PRIVATE(this));
  buttonlist->append(button);
  button->setPressedState(this->isViewing());

  button = new SoWinBitmapButton(parent, 24, "home", NULL);
  button->addBitmap(home_xpm);
  button->setBitmap(0); // use first (and only) bitmap
  button->registerClickedProc(SoWinFullViewerP::homebuttonProc, PRIVATE(this));
  buttonlist->append(button);

  button = new SoWinBitmapButton(parent, 24, "set_home", NULL);
  button->addBitmap(set_home_xpm);
  button->setBitmap(0);
  button->registerClickedProc(SoWinFullViewerP::sethomebuttonProc, PRIVATE(this));
  buttonlist->append(button);

  button = new SoWinBitmapButton(parent, 24, "view_all", NULL);
  button->addBitmap(view_all_xpm);
  button->setBitmap(0); // use first (and only) bitmap
  button->registerClickedProc(SoWinFullViewerP::viewallbuttonProc, PRIVATE(this));
  buttonlist->append(button);

  button = new SoWinBitmapButton(parent, 24, "seek", NULL);
  button->addBitmap(seek_xpm);
  button->setBitmap(0); // use first (and only) bitmap
  button->registerClickedProc(SoWinFullViewerP::seekbuttonProc, PRIVATE(this));
  buttonlist->append(button);
}

// FIXME: the build*Trim() functions are present and working in the
// other So* toolkits, but are unimplemented missing in SoWin. Why?
// 20020111 mortene.

HWND
SoWinFullViewer::buildLeftTrim(HWND parent)
{
  return NULL;
}

HWND
SoWinFullViewer::buildBottomTrim(HWND parent)
{
  return NULL;
}

HWND
SoWinFullViewer::buildRightTrim(HWND parent)
{
  return NULL;
}

// *************************************************************************

SoWinFullViewerP::SoWinFullViewerP(SoWinFullViewer * publ)
  : SoGuiFullViewerP(publ)
{
}

SoWinFullViewerP::~SoWinFullViewerP()
{
  const int len = this->righttrimbuttons.getLength();
  for (int i = 0; i < len; i++) {
    delete (SoWinBitmapButton *)this->righttrimbuttons[i];
  }
  if (this->leftthumbwheel) 
    delete this->leftthumbwheel;
  if (this->rightthumbwheel) 
    delete this->rightthumbwheel;
  if (this->bottomthumbwheel) 
    delete this->bottomthumbwheel;
}

SoWinBitmapButton *
SoWinFullViewerP::viewerButton(int idx)
{
  if (this->righttrimbuttons.getLength() <= idx) { return NULL; }

  return (SoWinBitmapButton *)this->righttrimbuttons[idx];
}

HWND
SoWinFullViewerP::appButton(int idx)
{
  if (this->lefttrimbuttons.getLength() <= idx) { return NULL; }

  return (HWND)this->lefttrimbuttons[idx];
}

void
SoWinFullViewerP::setThumbWheelValue(void * wheel, float val)
{
  SoWinThumbWheel * twheel = SoWinThumbWheel::getWheelFromHWND((HWND)wheel);
  assert(twheel != NULL);
  twheel->setValue(val);
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************
