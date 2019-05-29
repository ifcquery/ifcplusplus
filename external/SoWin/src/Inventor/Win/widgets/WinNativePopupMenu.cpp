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

#include <windows.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <sowindefs.h>
#include <Inventor/Win/widgets/WinNativePopupMenu.h>
#include <Inventor/Win/Win32API.h>

/*!
  \class WinNativePopupMenu Inventor/Win/widgets/WinNativePopupMenu.h
  \brief The WinNativePopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.
*/

// *************************************************************************

struct MenuRecord {
  int menuid;
  char * name;
  char * title;
  HMENU menu;
  HMENU parent;
}; // struct MenuRecord

struct ItemRecord {
  int itemid;
  int flags;
  char * name;
  char * title;
  HMENU parent;
}; // struct ItemRecord

#define ITEM_TOGGLE       0x0001
#define ITEM_MARKED       0x0002
#define ITEM_SEPARATOR    0x0004
#define ITEM_ENABLED      0x0008

// *************************************************************************

WinNativePopupMenu::WinNativePopupMenu(void)
{
  this->menus = new SbPList;
  this->items = new SbPList;

  this->notify = FALSE;
  this->selectedItem = -1;
}

WinNativePopupMenu::~WinNativePopupMenu(void)
{
  const int numMenus = this->menus->getLength();
  HMENU popup = NULL;
  int i;
  for (i = 0; i < numMenus; i++) {
    MenuRecord * rec = (MenuRecord *) (* this->menus)[i];
    if (rec->menuid == 0) popup = rec->menu;
    delete [] rec->name;
    delete [] rec->title;
    // FIXME: the second part of the AND expression was added recently
    // to avoid a bug with multiple destruction of menu instance. It's
    // just a quick fix -- look over the rest of the code which
    // handles MenuRecord instances and see how it can be
    // fixed/re-designed properly. 20020521 mortene.
    if ((rec->parent == NULL) &&     // menu not attached
        (rec->menu != popup)) {
      Win32::DestroyMenu(rec->menu);
    }
    delete rec;
  }

  // delete root popup menu
  if (popup) { Win32::DestroyMenu(popup); }

  const int numItems = this->items->getLength();
  for (i = 0; i < numItems; i++) {
    ItemRecord * rec = (ItemRecord *) (* this->items)[i];
    delete [] rec->name;
    delete [] rec->title;
    delete rec;
  }
  
  delete this->menus;
  delete this->items;
}

// *************************************************************************

int
WinNativePopupMenu::newMenu(const char * name, int menuid)
{
  // FIXME: this function is the same in the other So-libraries --
  // move to common abstraction layer SoGuiPopupMenu of
  // possible. 20031012 mortene.

  int id = menuid;
  if (id == -1) {
    id = 1;
    while (this->getMenuRecord(id) != NULL) id++;
  } else {
    assert(this->getMenuRecord(id) == NULL &&
            "requested menuid already taken");
  }
  // id contains ok ID
  MenuRecord * rec = createMenuRecord(name);
  rec->menuid = id;
  this->menus->append((void *) rec);
  return id;
}

int
WinNativePopupMenu::getMenu(const char * name)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++) {
    if (strcmp(((MenuRecord *) (* this->menus)[i])->name, name) == 0) {
      return ((MenuRecord *) (* this->menus)[i])->menuid;
    }
  }
  return -1;
}

void
WinNativePopupMenu::setMenuTitle(int menuid, const char * title)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec != NULL && "no such menu");
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);

  if (rec->parent)
    Win32::ModifyMenu(rec->parent, rec->menuid, MF_BYPOSITION | MF_STRING, rec->menuid, rec->title);

}

const char *
WinNativePopupMenu::getMenuTitle(int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec != NULL && "no such menu");
  return rec->title;
}

// *************************************************************************

int
WinNativePopupMenu::newMenuItem(const char * name, int itemid)
{
  int id = itemid;
  if (id == -1) {
    id = 1;
    while (this->getItemRecord(itemid) != NULL) id++;
  } else {
    if (this->getItemRecord(itemid) != NULL) {
#if SOWIN_DEBUG
      SoDebugError::postInfo("WinNativePopupMenu::newMenuItem",
                              "requested itemid already taken");
#endif // SOWIN_DEBUG
      return -1;
    }
  }
  ItemRecord * rec = createItemRecord(name);
  rec->itemid = id;
  this->items->append(rec);
  return id;
}

int
WinNativePopupMenu::getMenuItem(const char * name)
{
  const int numItems = this->items->getLength();
  int i;
  for (i = 0; i < numItems; i++)
    if (strcmp(((ItemRecord *) (* this->items)[i])->name, name) == 0)
      return ((ItemRecord *) (* this->items)[i])->itemid;
  return -1;
}

void
WinNativePopupMenu::setMenuItemTitle(int itemid, const char * title)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec != NULL && "no such menu");
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);

  if (rec->parent)
    Win32::ModifyMenu(rec->parent, rec->itemid, MF_BYCOMMAND | MF_STRING, rec->itemid, rec->title);
}

const char *
WinNativePopupMenu::getMenuItemTitle(int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec != NULL && "no such menu");
  return rec->title;
}

void
WinNativePopupMenu::setMenuItemEnabled(int itemid, SbBool enabled)
{
  ItemRecord * rec = this->getItemRecord(itemid);

  assert(rec && "could not find item record");
  assert(rec->parent && "a menuitem must have a parent to be enabled/disabled");
  assert(IsMenu(rec->parent));
  
  MENUITEMINFO info;
  // (no need to ZeroMemory() structure)

  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  
  if (enabled) {
    rec->flags |= ITEM_ENABLED;
    info.fState = MFS_ENABLED;
  }
  else {
    rec->flags &= ~ITEM_ENABLED;    
    info.fState = MFS_DISABLED | MFS_GRAYED;
  }
  
  Win32::SetMenuItemInfo(rec->parent, rec->itemid, FALSE, & info);
}

SbBool
WinNativePopupMenu::getMenuItemEnabled(int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  
  assert(rec && "could not find item record");
  assert(IsMenu(rec->parent));

  //MENUITEMINFO  menuiteminfo;
  //memset((void *) & menuiteminfo, 0, sizeof(menuiteminfo));
  //Win32::GetMenuItemInfo(rec->parent, rec->itemid, TRUE, & menuiteminfo);
  //return (menuiteminfo.fState & MFS_ENABLED) ? TRUE : FALSE;
 
  return (rec->flags & ITEM_ENABLED ? TRUE : FALSE);
}

void
WinNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked)
{
  ItemRecord * rec = this->getItemRecord(itemid);

  if(rec == NULL) return;
  if(rec->parent == NULL) return;

  rec->flags |= ITEM_TOGGLE;

  MENUITEMINFO info;
  
  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  
  if (marked) {
    rec->flags |= ITEM_MARKED;
    info.fState = MFS_CHECKED;
  }
  else {
    rec->flags &= ~ITEM_MARKED;    
    info.fState = MFS_UNCHECKED;
  }
  
  Win32::SetMenuItemInfo(rec->parent, rec->itemid, FALSE, & info);
}

SbBool
WinNativePopupMenu::getMenuItemMarked(int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if(rec == NULL) return FALSE;
  if(rec->parent == NULL) return FALSE;
  
  MENUITEMINFO info;

  info.cbSize = sizeof(MENUITEMINFO);
  info.fMask = MIIM_STATE;
  
  Win32::GetMenuItemInfo(rec->parent, rec->itemid, FALSE, & info);

  return (info.fState & MFS_CHECKED ? TRUE : FALSE);
}

// *************************************************************************

void
WinNativePopupMenu::addMenu(int menuid, int submenuid, int pos)
{
  MenuRecord * super = this->getMenuRecord(menuid);
  MenuRecord * sub = this->getMenuRecord(submenuid);
  assert(super != NULL && sub != NULL && "no such menu");

  MENUITEMINFO menuiteminfo;
  memset((void *) & menuiteminfo, 0, sizeof(menuiteminfo));
  menuiteminfo.cbSize = sizeof(menuiteminfo);
  menuiteminfo.fMask = MIIM_SUBMENU | MIIM_TYPE;
  menuiteminfo.fType = MFT_STRING;
  menuiteminfo.hSubMenu = sub->menu;
  menuiteminfo.dwTypeData = sub->title;
  menuiteminfo.cch = strlen(sub->title);

  if (pos == -1)
    Win32::InsertMenuItem(super->menu, sub->menuid, FALSE, & menuiteminfo);
  else
    Win32::InsertMenuItem(super->menu, pos, TRUE, & menuiteminfo);
  sub->parent = super->menu;
}

void
WinNativePopupMenu::addMenuItem(int menuid, int itemid, int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  ItemRecord * item = this->getItemRecord(itemid);
  assert(menu != NULL && item != NULL && "no such menu");
  
  Win32::InsertMenu(menu->menu, pos, MF_BYPOSITION | MF_STRING, item->itemid, item->title);

  item->parent = menu->menu;
  if (item->flags & ITEM_MARKED)
    Win32::CheckMenuItem(item->parent, item->itemid, MF_BYCOMMAND | MF_CHECKED);
}

void
WinNativePopupMenu::addSeparator(int menuid, int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  assert(menu != NULL && "no such menu");
  ItemRecord * rec = createItemRecord("separator");
 
  Win32::InsertMenu(menu->menu, pos, MF_BYPOSITION | MF_SEPARATOR, pos, NULL);
  rec->flags |= ITEM_SEPARATOR;
  this->items->append(rec);
}

void
WinNativePopupMenu::removeMenu(int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec != NULL && "no such menu");

  // FIXME: just assumes root-menu has id==0. Bad. 20010810 mortene.
  if (rec->menuid == 0) {
#if SOWIN_DEBUG
    SoDebugError::postInfo("WinNativePopupMenu::removeMenu", "can't remove root");
#endif // SOWIN_DEBUG
    return;
  }
  if (rec->parent == NULL) {
#if SOWIN_DEBUG
    SoDebugError::postInfo("WinNativePopupMenu::removeMenu", "menu not attached");
#endif // SOWIN_DEBUG
    return;
  }
  Win32::RemoveMenu(rec->menu, rec->menuid, MF_BYCOMMAND);
  rec->parent = NULL;
}

void
WinNativePopupMenu::removeMenuItem(int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec != NULL && "no such menu");
  if (rec->parent == NULL) {
#if SOWIN_DEBUG
    SoDebugError::postInfo("WinNativePopupMenu::removeMenuItem", "item not attached");
#endif // SOWIN_DEBUG
    return;
  }
  Win32::RemoveMenu(rec->parent, rec->itemid, MF_BYCOMMAND);
  rec->parent = NULL;
}

// *************************************************************************

void
WinNativePopupMenu::popUp(HWND inside, int x, int y)
{
  POINT point;
  point.x = x;
  point.y = y;
  Win32::ClientToScreen(inside, &point);

  MenuRecord * menurec = this->getMenuRecord(0);
  // Note: the Win32 API TrackPopupMenu() is modal.
  this->selectedItem = TrackPopupMenu(menurec->menu,
                                      TPM_LEFTALIGN |
                                      TPM_TOPALIGN |
                                      TPM_RIGHTBUTTON |
                                      TPM_RETURNCMD |
                                      (this->notify ? 0 : TPM_NONOTIFY),
                                      point.x,
                                      point.y,
                                      0,
                                      inside,
                                      NULL);

  if (this->selectedItem != 0) { // 0 == no item selected (user aborted)
    this->invokeMenuSelection(this->selectedItem);
  }
}

int
WinNativePopupMenu::getSelectedItem(void)
{
 return (this->selectedItem);
}

void
WinNativePopupMenu::setNotify(SbBool enable)
{
 this->notify = enable;
}

// *************************************************************************

MenuRecord *
WinNativePopupMenu::getMenuRecord(int menuid)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++)
    if (((MenuRecord *) (* this->menus)[i])->menuid == menuid)
      return (MenuRecord *) (* this->menus)[i];
  return (MenuRecord *) NULL;
}

ItemRecord *
WinNativePopupMenu::getItemRecord(int itemid)
{
  const int numItems = this->items->getLength();
  int i;
 
  for (i = 0; i < numItems; i++)
    if (((ItemRecord *) (* this->items)[i])->itemid == itemid)
      return (ItemRecord *) (* this->items)[i];

  return (ItemRecord *) NULL;
}

// *************************************************************************

MenuRecord *
WinNativePopupMenu::createMenuRecord(const char * name)
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->name = strcpy(new char [strlen(name) + 1], name);
  rec->title = strcpy(new char [strlen(name) + 1], name);
  rec->menu = Win32::CreatePopupMenu();
  rec->parent = NULL;
  return rec;
}

ItemRecord *
WinNativePopupMenu::createItemRecord(const char * name)
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->flags = 0;
  rec->name = strcpy(new char [strlen(name) + 1], name);
  rec->title = strcpy(new char [strlen(name) + 1], name);
  rec->parent = NULL;
  return rec;
}

// *************************************************************************
