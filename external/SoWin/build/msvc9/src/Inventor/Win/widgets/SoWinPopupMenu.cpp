// src\Inventor\Win\widgets\SoWinPopupMenu.cpp.  Generated from SoGuiPopupMenu.cpp.in by configure.

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

/*!
  \class SoWinPopupMenu Inventor/Win/widgets/SoWinPopupMenu.h
  \brief The SoWinPopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.

  The point of this class is to enable client code to be exactly the
  same for all GUI libraries, even without being template-based.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <sowindefs.h>
#include <Inventor/Win/widgets/SoWinPopupMenu.h>
#include <Inventor/Win/widgets/WinNativePopupMenu.h>
#include <Inventor/Win/common/SbGuiList.h>

// *************************************************************************

// Documentation for abstract methods.

/*!
  \fn void SoWinPopupMenu::popUp(HWND inside, int x, int y)

  Invokes this popup menu instance.

  The coordinates for the popup will be used relative to the origo of
  the widget argument.
*/

// *************************************************************************

// The private data for the SoWinPopupMenu.

class SoWinPopupMenuP {
public:
  SbGuiList<int> menuitems;
  SbGuiList<int> radiogroups;
  SbPList * callbacks;
  SbPList * userdata;
};


#define PRIVATE(o) (o->pimpl)

// *************************************************************************

/*!
  Instantiation method.

  SoWinPopupMenu is implemented with this "factory" method instead of
  a public constructor to provide a completely generic interface on
  top of the toolkit's native popup menu implementation.
*/
SoWinPopupMenu *
SoWinPopupMenu::createInstance(void)
{
  return new WinNativePopupMenu();
}


/*!
  The class constructor is protected, as SoWinPopupMenu uses a factory
  instantiation method.
*/
SoWinPopupMenu::SoWinPopupMenu(void)
{
  PRIVATE(this) = new SoWinPopupMenuP;

  PRIVATE(this)->callbacks = NULL;
  PRIVATE(this)->userdata = NULL;
} // SoWinPopupMenu()

/*!
  Destructor.
*/
SoWinPopupMenu::~SoWinPopupMenu()
{
  delete PRIVATE(this)->callbacks;
  delete PRIVATE(this)->userdata;
  delete PRIVATE(this);
} // ~SoWinPopupMenu()

// *************************************************************************

void
SoWinPopupMenu::setMenuItemMarked(int itemid, SbBool marked)
{
  // forward to native implementation
  this->_setMenuItemMarked(itemid, marked);

  if (marked) {
    const int radiogroupid = this->getRadioGroup(itemid);
    if (radiogroupid != -1) {
      this->setRadioGroupMarkedItem(itemid);
    }
  }
}

// *************************************************************************

/*!
*/

int
SoWinPopupMenu::newRadioGroup(
  int groupid)
{
  const int numGroupedItems = PRIVATE(this)->radiogroups.getLength();
  int id = groupid;
  if (id != -1) {
    SbBool hit = FALSE;
    int i;
    for (i = 0; i < numGroupedItems && hit == FALSE; i++) {
      if (PRIVATE(this)->radiogroups[i] == id)
        hit = TRUE;
    }
    if (hit) {
#if SOWIN_DEBUG
      SoDebugError::postInfo("SoWinPopupMenu::NewRadioGroup",
        "requested group id already taken");
#endif // SOWIN_DEBUG
      return -1; 
    }
  } else {
    id = 0;
    SbBool found;
    do {
      id++;
      found = FALSE;
      int i;
      for (i = 0; i < numGroupedItems && found == FALSE; i++) {
        if (PRIVATE(this)->radiogroups[i] == id)
          found = TRUE;
      }
    } while (found);
  }
  // id is OK here
  PRIVATE(this)->menuitems.append(-1); // fake menu item
  PRIVATE(this)->radiogroups.append(id); // register id in array
  return id;
} // newRadioGroup()

/*!
*/

int
SoWinPopupMenu::getRadioGroup(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->radiogroups.getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if (PRIVATE(this)->menuitems[i] == itemid)
      return PRIVATE(this)->radiogroups[i];
  }
  return -1;
} // getRadioGroup()

/*!
*/

int
SoWinPopupMenu::getRadioGroupSize(
  int groupid)
{
  int count = 0;
  const int numItems = PRIVATE(this)->radiogroups.getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if (PRIVATE(this)->radiogroups[i] == groupid &&
         PRIVATE(this)->menuitems[i] != -1)
      count++;
  }
  return count;
} // getRadioGroupSize()

/*!
*/

void
SoWinPopupMenu::addRadioGroupItem(
  int groupid,
  int itemid)
{
  PRIVATE(this)->menuitems.append(itemid);
  PRIVATE(this)->radiogroups.append(groupid);
} // addRadioGroupItem()

/*!
*/

void
SoWinPopupMenu::removeRadioGroupItem(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->menuitems.getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if (PRIVATE(this)->menuitems[i] == itemid) {
      PRIVATE(this)->menuitems.remove(i);
      PRIVATE(this)->radiogroups.remove(i);
      return;
    }
  }
} // removeRadioGroupItem()

// *************************************************************************

void
SoWinPopupMenu::setRadioGroupMarkedItem(int itemid)
{
  const int idx = PRIVATE(this)->menuitems.find(itemid);
  assert(idx != -1);
  const int groupid = PRIVATE(this)->radiogroups[idx];

  if (groupid == -1) {
    SoDebugError::post("SoWinPopupMenu::setRadioGroupMarkedItem",
                       "item not in a radio group");
    return;
  }

  const int numItems = PRIVATE(this)->menuitems.getLength();
  for (int i = 0; i < numItems; i++) {
    if (i == idx) { continue; }
    if (PRIVATE(this)->radiogroups[i] == groupid) {
      const int item = PRIVATE(this)->menuitems[i];
      if (item != -1) { // FIXME: should this be an assert? 20050622 mortene.
        if ( this->getMenuItemMarked(item) ) {
          this->setMenuItemMarked(item, FALSE);
        }
      }
    }
  }
}

int
SoWinPopupMenu::getRadioGroupMarkedItem(int groupid)
{
  const int numItems = PRIVATE(this)->menuitems.getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if (PRIVATE(this)->radiogroups[i] == groupid) {
      int item = PRIVATE(this)->menuitems[i];
      if (item != -1 && this->getMenuItemMarked(item))
        return item;
    }
  }
  return -1;
}

// *************************************************************************

/*!
*/

void
SoWinPopupMenu::addMenuSelectionCallback(
  SoWinMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
    PRIVATE(this)->callbacks = new SbPList;
    PRIVATE(this)->userdata = new SbPList;
  }
  PRIVATE(this)->callbacks->append((void *) callback);
  PRIVATE(this)->userdata->append(user);
} // addMenuSelectionCallback()

/*!
*/

void
SoWinPopupMenu::removeMenuSelectionCallback(
  SoWinMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
#if SOWIN_DEBUG
    SoDebugError::postInfo("SoWinPopupMenu::RemoveMenuSelectionCallback",
      "no callbacks at all");
#endif // SOWIN_DEBUG
    return;
  }
  int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    if ((*PRIVATE(this)->callbacks)[i] == (void *) callback &&
         (*PRIVATE(this)->userdata)[i] == user) {
      PRIVATE(this)->callbacks->remove(i);
      PRIVATE(this)->userdata->remove(i);
      i--;
      numCallbacks--;
    }
  }
} // removeMenuSelectionCallback()

/*!
*/

void
SoWinPopupMenu::invokeMenuSelection(
  int itemid)
{
  if (PRIVATE(this)->callbacks == NULL)
    return;
  const int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    SoWinMenuSelectionCallback * cb = (SoWinMenuSelectionCallback *)
      (*PRIVATE(this)->callbacks)[i];
    (*cb)(itemid, (*PRIVATE(this)->userdata)[i]);
  }
} // invokeMenuSelection()

// *************************************************************************

