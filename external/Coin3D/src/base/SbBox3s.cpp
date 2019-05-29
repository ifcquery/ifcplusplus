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
  \class SbBox3s SbBox3s.h Inventor/SbBox3s.h
  \brief The SbBox3s class is a 3 dimensional box with short
  integer coordinates.
  \ingroup base

  This box class is used by other classes in Coin for data
  exchange. It provides storage for two box corners with short integer
  coordinates.

  \sa SbBox2s, SbBox2f, SbBox2d, SbBox3f, SbBox3d, SbXfBox3f.
  \since Coin 2.0
  \since TGS Inventor ?.?
*/


#include <Inventor/SbBox3s.h>

#include <limits>
#include <cassert>

#include <Inventor/SbBox3i32.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbBox3d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \fn SbBox3s::SbBox3s(void)
  The default constructor makes an empty box.
*/

/*!
  \fn SbBox3s::SbBox3s(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax)

  Constructs a box with the given corner coordinates.

  \a xmin should be less than \a xmax, \a ymin should be less than \a
  ymax, and \a zmin should be less than \a zmax if you want to make a
  valid box.
*/

/*!
  \fn SbBox3s::SbBox3s(const SbVec3s & minvec, const SbVec3s & maxvec)

  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
*/

/*!
  \fn SbBox3s & SbBox3s::setBounds(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax)

  Reset the boundaries of the box.

  \a xmin should be less than \a xmax, \a ymin should be less than \a
  ymax, and \a zmin should be less than \a xmax if you want to make a
  valid box.

  Returns reference to self.

  \sa getBounds().  
*/

/*!
  \fn SbBox3s & SbBox3s::setBounds(const SbVec3s & minvec, const SbVec3s & maxvec)

  Reset the boundaries of the box with the given corners.

  The coordinates of \a minvec should be less than the coordinates of
  \a maxvec if you want to make a valid box.

  Returns reference to self.

  \sa getBounds().
*/

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds().
*/

SbBox3s &
SbBox3s::setBounds(const SbBox3i32 & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds().
*/

SbBox3s &
SbBox3s::setBounds(const SbBox3f & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds().
*/

SbBox3s &
SbBox3s::setBounds(const SbBox3d & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
*/
void
SbBox3s::makeEmpty(void)
{
  this->minpt.setValue(std::numeric_limits<short>::max(), std::numeric_limits<short>::max(), std::numeric_limits<short>::max());
  this->maxpt.setValue(-std::numeric_limits<short>::max(), -std::numeric_limits<short>::max(), -std::numeric_limits<short>::max());
}

/*!
  \fn const SbVec3s & SbBox3s::getMin(void) const

  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/

/*!
  \fn const SbVec3s & SbBox3s::getMax(void) const

  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already within it.
 */
void
SbBox3s::extendBy(const SbVec3s & point)
{
  // The explicit casts are done to humour the HPUX aCC compiler,
  // which will otherwise say ``Template deduction failed to find a
  // match for the call to 'SbMin'''. mortene.
  this->minpt.setValue(SbMin(static_cast<short>(point[0]), static_cast<short>(this->minpt[0])),
                       SbMin(static_cast<short>(point[1]), static_cast<short>(this->minpt[1])),
                       SbMin(static_cast<short>(point[2]), static_cast<short>(this->minpt[2])));
  this->maxpt.setValue(SbMax(static_cast<short>(point[0]), static_cast<short>(this->maxpt[0])),
                       SbMax(static_cast<short>(point[1]), static_cast<short>(this->maxpt[1])),
                       SbMax(static_cast<short>(point[2]), static_cast<short>(this->maxpt[2])));
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
 */
void
SbBox3s::extendBy(const SbBox3s & box)
{
  if (box.isEmpty()) { return; }

  this->extendBy(box.getMin());
  this->extendBy(box.getMax());
}

/*!
  Check if the given point lies within the boundaries of this box.
 */
SbBool
SbBox3s::intersect(const SbVec3s & point) const
{
  if((point[0] >= this->minpt[0]) && (point[0] <= this->maxpt[0]) &&
     (point[1] >= this->minpt[1]) && (point[1] <= this->maxpt[1]) &&
     (point[2] >= this->minpt[2]) && (point[2] <= this->maxpt[2])) return TRUE;
  return FALSE;
}

/*!
  Check if \a box lies wholly or partly within the boundaries
  of this box.
 */
SbBool
SbBox3s::intersect(const SbBox3s & box) const
{
  if((box.getMax()[0] < this->getMin()[0]) ||
     (box.getMax()[1] < this->getMin()[1]) ||
     (box.getMax()[2] < this->getMin()[2]) ||
     (box.getMin()[0] > this->getMax()[0]) ||
     (box.getMin()[1] > this->getMax()[1]) ||
     (box.getMin()[2] > this->getMax()[2])) return FALSE;
  return TRUE;
}

/*!
*/

SbVec3f
SbBox3s::getClosestPoint(const SbVec3f & pt) const
{
  SbVec3f closest = pt;

  SbVec3f center = this->getCenter();
  float devx = closest[0] - center[0];
  float devy = closest[1] - center[1];
  float devz = closest[2] - center[2];
  float halfwidth = float(this->maxpt[0] - this->minpt[0]) / 2.0f;
  float halfheight = float(this->maxpt[1] - this->minpt[1]) / 2.0f;
  float halfdepth = float(this->maxpt[2] - this->minpt[2]) / 2.0f;

  // Move point to be on the nearest plane of the box.
  if ((fabs(devx) > fabs(devy)) && (fabs(devx) > fabs(devz)))
    closest[0] = center[0] + halfwidth * ((devx < 0.0f) ? -1.0f : 1.0f);
  else if (fabs(devy) > fabs(devz))
    closest[1] = center[1] + halfheight * ((devy < 0.0f) ? -1.0f : 1.0f);
  else
    closest[2] = center[2] + halfdepth * ((devz < 0.0f) ? -1.0f : 1.0f);

  // Clamp to be inside box.
  closest[0] = SbMin(SbMax(closest[0], float(minpt[0])), float(maxpt[0]));
  closest[1] = SbMin(SbMax(closest[1], float(minpt[1])), float(maxpt[1]));
  closest[2] = SbMin(SbMax(closest[2], float(minpt[2])), float(maxpt[2]));

  return closest;
}

/*!
  \fn void SbBox3s::getBounds(short & xmin, short & ymin, short & zmin, short & xmax, short & ymax, short & zmax) const

  Returns the box boundary coordinates.

  \sa setBounds(), getMin(), getMax().
*/

/*!
  \fn void SbBox3s::getBounds(SbVec3s & minvec, SbVec3s & maxvec) const

  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/

/*!
  \fn void SbBox3s::getOrigin(short & originX, short & originY, short & originZ) const

  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/

/*!
  \fn void SbBox3s::getSize(short & sizeX, short & sizeY, short & sizeZ) const

  Returns width and height of box.
*/

/*!
  \fn int operator == (const SbBox3s & b1, const SbBox3s & b2)
  \relates SbBox3s

  Check \a b1 and \a b2 for equality.
*/

/*!
  \fn int operator != (const SbBox3s & b1, const SbBox3s & b2)
  \relates SbBox3s

  Check \a b1 and \a b2 for inequality.
*/

/*!
  \fn SbBool SbBox3s::hasVolume(void) const
*/

#ifdef COIN_TEST_SUITE
BOOST_AUTO_TEST_CASE(checkSize) {
  SbVec3s min(1,2,3);
  SbVec3s max(3,4,5);

  SbVec3s diff = max - min;

  
  SbBox3s box(min, max);

  BOOST_CHECK_MESSAGE(box.getSize() == diff,
                      "Box has incorrect size");

}
#endif //COIN_TEST_SUITE
