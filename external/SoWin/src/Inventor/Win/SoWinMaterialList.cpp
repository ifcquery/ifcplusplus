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

#include <stdlib.h>

#include <Inventor/SbPList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoMaterial.h>

#include <sowindefs.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoAnyMaterialList.h>

#include <Inventor/Win/SoWinMaterialList.h>

// *************************************************************************

/*!
  \class SoWinMaterialList Inventor/Win/SoWinMaterialList.h
  \brief The SoWinMaterialList class is a component for adding material
  selection to scene graph editors.
*/

// *************************************************************************

SOWIN_OBJECT_SOURCE(SoWinMaterialList);

// *************************************************************************

/*!
  Public constructor.
*/

SoWinMaterialList::SoWinMaterialList(
  HWND parent,
  const char * const name,
  SbBool embed,
  const char * const dir)
: inherited(parent, name, embed)
{
  this->constructor(dir, TRUE);
} // SoWinMaterialList()

/*!
  Protected constructor.
*/

SoWinMaterialList::SoWinMaterialList(// protected
  HWND parent,
  const char * const name,
  SbBool embed,
  const char * const dir,
  SbBool build)
: inherited(parent, name, embed)
{
  this->constructor(dir, build);
} // SoWinMaterialList()

/*!
  Common constructor code.
*/

void
SoWinMaterialList::constructor(// private
  const char * const dir,
  const SbBool build)
{
  this->common = new SoAnyMaterialList(dir);
  this->setSize(SbVec2s(200, 300));

  if (build) {
    this->setClassName("SoWinMaterialList");
    HWND materials = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(materials);
  }
} // constructor()

/*!
  Destructor.
*/

SoWinMaterialList::~SoWinMaterialList(
  void)
{
  delete this->common;
} // ~SoWinMaterialList()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWinMaterialList::addCallback(
  SoWinMaterialListCB * const callback,
  void * const closure)
{
  common->addCallback(callback, closure);
} // addCallback()

/*!
  FIXME: write doc
*/

void
SoWinMaterialList::removeCallback(
  SoWinMaterialListCB * const callback,
  void * const closure)
{
  common->removeCallback(callback, closure);
} // removeCallback()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoWinMaterialList::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoWinMaterialList";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoWinMaterialList::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Material List";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoWinMaterialList::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Material List";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write doc
*/

HWND
SoWinMaterialList::buildWidget(// protected
  HWND parent)
{
/*
  // we're building pulldown menu automatically...
  HWND root = XtVaCreateManagedWidget(this->getClassName(),
    xmFormWidgetClass, parent,
    NULL);

  HWND menubar = XtVaCreateManagedWidget("menubar",
    xmRowColumnWidgetClass, root,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrowColumnType, XmMENU_BAR,
    NULL);

  HWND palettemenu = this->buildPulldownMenu(menubar);

  SoWinMaterialDirectory * dir = common->getMaterialDirectory();

  int group, materials;
  XmStringTable list;
  if (dir->numGroups > 0) {
    group = dir->current;
    materials = dir->groups[group]->numMaterials;
    list = (XmStringTable) XtMalloc(sizeof(XmString) * materials);
    for (int i = 0; i < materials; i++)
      list[i] =
        SoWin::encodeString(dir->groups[group]->materials[i]->name);
  } else {
    materials = 0;
    list = NULL;
  }

  Arg argv[10];
  int argc = 0;
  XtSetArg(argv[argc], XmNleftAttachment, XmATTACH_FORM); argc++;
  XtSetArg(argv[argc], XmNtopAttachment, XmATTACH_WIDGET); argc++;
  XtSetArg(argv[argc], XmNtopWidget, menubar); argc++;
  XtSetArg(argv[argc], XmNrightAttachment, XmATTACH_FORM); argc++;
  XtSetArg(argv[argc], XmNbottomAttachment, XmATTACH_FORM); argc++;
  XtSetArg(argv[argc], XmNselectionPolicy, XmSINGLE_SELECT); argc++;
  XtSetArg(argv[argc], XmNscrollBarDisplayPolicy, XmSTATIC); argc++;
  XtSetArg(argv[argc], XmNitems, list); argc++;
  XtSetArg(argv[argc], XmNitemCount, materials); argc++;

  this->listwidget = XmCreateScrolledList(root, "materials", argv, argc);
  XtManageChild(this->listwidget);
  XtAddCallback(this->listwidget, XmNdefaultActionCallback,
    SoWinMaterialList::selection_cb, (void *) this);
  for (int i = 0; i < materials; i++)
    XmStringFree(list[i]);
  XtFree((char *) list);
  return root;
*/
  return NULL;
} // buildWidget()

// *************************************************************************

/*!
  \internal
  FIXME: write doc
*/

void
SoWinMaterialList::selectionCallback(// private
  int materialid)
{
  materialid--; // get index
  SoWinMaterialDirectory * data = common->getMaterialDirectory();

  assert(materialid >= 0 &&
          materialid < data->groups[data->current]->numMaterials);
  const char * materialdata =
    data->groups[data->current]->materials[materialid]->data;

  SoInput reader;
  if (data->flags & SOWIN_BUILTIN_MATERIALS) {
    reader.setBuffer((void *) materialdata, strlen(materialdata));
  } else {
    if (! reader.openFile(materialdata, FALSE)) {
      SoDebugError::postWarning("SoWinMaterialList::selectionCallback",
        "could not open file: \"%s\"", materialdata);
      return;
    }
  }

  SoNode * material = NULL;
  if (! SoDB::read(&reader, material)) {
    SoDebugError::postWarning("SoWinMaterialList::selectionCallback",
      "failed to read material");
    return;
  }
  if (! material) {
    SoDebugError::postWarning("SoWinMaterialList::selectionCallback",
      "read returned no data");
    return;
  }
  material->ref();
  if (! material->isOfType(SoMaterial::getClassTypeId())) {
    SoDebugError::postWarning("SoWinMaterialList::selectionCallback",
      "not a material node!");
    material->unref();
    return;
  }
  common->invokeCallbacks((SoMaterial *) material);
  material->unref();
} // selectionCallback()

/*!
  \internal
  FIXME: write doc
*/

void
SoWinMaterialList::selection_cb(// static, private
  HWND,
  void * closure,
  void * call_data)
{
/*
  SoWinMaterialList * component = (SoWinMaterialList *) closure;
  XmListCallbackStruct * data = (XmListCallbackStruct *) call_data;
  component->selectionCallback(data->item_position);
*/
} // selection_cb()

// *************************************************************************

/*!
  \internal
  FIXME: write doc
*/

void
SoWinMaterialList::paletteMenuCallback(// private
  HWND menuitem)
{
/*
  SoWinMaterialDirectory * data = common->getMaterialDirectory();
  int i, j;
  for (i = 0; i < data->numGroups; i++) {
    if (data->groups[i]->menuitem == menuitem) {
      XmStringTable list = (XmStringTable) XtMalloc(sizeof(XmString) *
        data->groups[i]->numMaterials);
      for (j = 0; j < data->groups[i]->numMaterials; j++)
        list[j] = SoWin::encodeString(data->groups[i]->materials[j]->name);
      XtVaSetValues(this->listwidget,
        XmNitemCount, data->groups[i]->numMaterials,
        XmNitems, list,
        NULL);
      for (j = 0; j < data->groups[i]->numMaterials; j++)
        XmStringFree(list[j]);
      XtFree((char *) list);
      data->current = i;
      return;
    }
  }
*/
} // paletteMenuCallback()

/*!
  \internal
  FIXME: write doc
*/

void
SoWinMaterialList::palette_menu_cb(// static, private
  HWND widget,
  void * closure,
  void *)
{
  assert(closure != NULL);
  SoWinMaterialList * component = (SoWinMaterialList *) closure;
  component->paletteMenuCallback(widget);
} /* palette_menu_cb */

// *************************************************************************

/*!
  This method builds the pulldown menu.
  The current implementation demands that the \a parent argument should be
  the menubar RowColumn widget - only the "Palettes" menubar button and the
  pulldown menu is built by this method.
*/

HWND
SoWinMaterialList::buildPulldownMenu(// protected
  HWND parent)
{
/*
  HWND palettes = XtVaCreateManagedWidget("palettes",
    xmCascadeButtonGadgetClass, parent,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Palettes", sizeof("Palettes") + 1,
    NULL);

  HWND shell = SoWin::getShellWidget(parent);
  assert(shell != (Widget) NULL);

  Visual * visual;
  Colormap colormap;
  int depth;

  XtVaGetValues(shell,
    XmNvisual, &visual,
    XmNcolormap, &colormap,
    XmNdepth, &depth,
    NULL);

  HWND menu =
    XmVaCreateSimplePulldownMenu(parent, "materialsmenu", 0, NULL, NULL);

  SoWinMaterialDirectory * data = common->getMaterialDirectory();
  for (int i = 0; i < data->numGroups; i++) {
    HWND item;
    item = XtVaCreateManagedWidget(data->groups[i]->name,
             xmPushButtonGadgetClass, menu,
             XtVaTypedArg,
               XmNlabelString, XmRString,
               data->groups[i]->name, strlen(data->groups[i]->name) + 1,
             NULL);
    data->groups[i]->menuitem = item;
    XtAddCallback(item, XmNactivateCallback,
      SoWinMaterialList::palette_menu_cb, (void *) this);
  }

  return palettes;
*/
  return NULL;
} // buildPulldownMenu()

// *************************************************************************
