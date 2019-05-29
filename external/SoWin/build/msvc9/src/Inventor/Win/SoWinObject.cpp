// src\Inventor\Win\SoWinObject.cpp.  Generated from SoGuiObject.cpp.in by configure.

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

// Pulls in __COIN_SOWIN__ define used below.
#include <Inventor/Win/SoWinBasic.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <sowindefs.h>
#include <Inventor/Win/SoWinObject.h>
#include <Inventor/Win/devices/SoWinDevice.h>
#include <Inventor/Win/SoWinComponent.h>

/*!
  \class SoWinObject Inventor/Win/SoWinObject.h
  \brief The SoWinObject class is the common superclass for all SoWin component classes.
  \ingroup misc

  The purpose of making this class the superclass of all SoWin
  device, component and viewer classes is to be able to do run-time
  type checking of the SoWin objects.


  You can place the macro SOWIN_OBJECT_HEADER(classname,parentname)
  within a class definition header for SoWin extension components to
  automatically make the necessary definitions for setting up a
  run-time type system for your extension classes:

  \code
  #ifndef MYSPECIALVIEWER_H
  #define MYSPECIALVIEWER_H

  class MySpecialViewer : public SoWinExaminerViewer {
    SOWIN_OBJECT_HEADER(MySpecialViewer, SoWinExaminerViewer);
    // [rest of class definition follows]
  };

  #endif // !MYSPECIALVIEWER_H
  \endcode

  Then put the SOWIN_OBJECT_SOURCE(classname) macro within the
  actual implementation sourcecode file to include the necessary
  code for the run-time type system:

  \code
  #include <MySpecialViewer.h>

  SOQT_OBJECT_SOURCE(MySpecialViewer);

  // [rest of class implementation]
  \endcode


  See also the documentation of the SoType class in Coin or Inventor.
 */

// *************************************************************************

/*!
  \fn SoType SoWinObject::getTypeId

  Returns the type identification of an object derived from a class
  inheriting SoWinObject.  This is used for run-time type checking
  and "downward" casting.

  Usage example:

  \code
  void foo(SoWinViewer * comp)
  {
    if (comp->getTypeId() == SoWinExaminerViewer::getClassTypeId()) {
      // safe downward cast, knows the type
      SoWinExaminerViewer * exviewer = (SoWinExaminerViewer *)comp;
      /// [then something] ///
    }
    else if (comp->getTypeId().isOfType(SoWinFlyViewer::getClassTypeId())) {
      // safe downward cast, knows the type
      SoWinFlyViewer * flyviewer = (SoWinFlyViewer *)comp;
      // then something else
    }
  }
  \endcode
*/
// FIXME: add doc above to explain how external developers can use the
// type system for their own extension classes. 20020502 mortene.

// This is a private variable.
SoType SoWinObject::classTypeId SOWIN_STATIC_SOTYPE_INIT;

/*!
  Sets up initialization for data common to all instances of this
  class, submitting necessary information to the internal SoWin type
  system.
*/
void
SoWinObject::initClass(void)
{
  assert(SoWinObject::classTypeId == SoType::badType());
  SoWinObject::classTypeId =
    SoType::createType(SoType::badType(), "SoWinObject");
}

/*!
  Returns \c TRUE if the type of this object is either of the same
  type or inherited from \a type.
*/
SbBool
SoWinObject::isOfType(SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  This static method returns the SoType object associated with
  objects of this class.
*/
SoType
SoWinObject::getClassTypeId(void)
{
  return SoWinObject::classTypeId;
}

/*!
  Initialize the type system of SoWinObject, all SoWin device
  classes and all SoWin components (including viewers).
*/
void
SoWinObject::init(void)
{
  SoWinObject::initClass();
  SoWinDevice::initClasses();
  SoWinComponent::initClasses();
}
