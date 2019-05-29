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
  \class SbBox3i32 Inventor/SbBox3i32.h
*/
// FIXME: class doc missing.  -mortene.

// *************************************************************************

#include <Inventor/SbBox3i32.h>

#include <limits>

#include <Inventor/SbBox3s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbBox3d.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds()
*/

SbBox3i32 &
SbBox3i32::setBounds(const SbBox3s & box)
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

  \sa getBounds()
*/

SbBox3i32 &
SbBox3i32::setBounds(const SbBox3f & box)
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

  \sa getBounds()
*/

SbBox3i32 &
SbBox3i32::setBounds(const SbBox3d & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

void
SbBox3i32::extendBy(const SbVec3i32 & point)
{
  if (isEmpty()) {
    minpt = maxpt = point;
  } else {
    this->minpt.setValue(SbMin(static_cast<int32_t>(point[0]), static_cast<int32_t>(this->minpt[0])),
                      SbMin(static_cast<int32_t>(point[1]), static_cast<int32_t>(this->minpt[1])),
                      SbMin(static_cast<int32_t>(point[2]), static_cast<int32_t>(this->minpt[2])));
    this->maxpt.setValue(SbMax(static_cast<int32_t>(point[0]), static_cast<int32_t>(this->maxpt[0])),
                      SbMax(static_cast<int32_t>(point[1]), static_cast<int32_t>(this->maxpt[1])),
                      SbMax(static_cast<int32_t>(point[2]), static_cast<int32_t>(this->maxpt[2])));
  }
}

void
SbBox3i32::extendBy(const SbBox3i32 & bb)
{
  if (!bb.isEmpty()) {
    extendBy(bb.getMin());
    extendBy(bb.getMax());
  }
}

void
SbBox3i32::extendBy(const SbVec3f & pt)
{
  SbVec3i32 ptmax(pt), ptmin(pt);
  if (float(ptmax[0]) < pt[0]) ++ptmax[0];
  if (float(ptmax[1]) < pt[1]) ++ptmax[1];
  if (float(ptmax[2]) < pt[2]) ++ptmax[2];
  extendBy(ptmin);
  extendBy(ptmax);
}

void
SbBox3i32::transform(const SbMatrix & m)
{
#if COIN_DEBUG
  if (this->isEmpty()) {
    SoDebugError::postWarning("SbBox3i32::transform",
                              "The box is not valid!");
    return;
  }
#endif // COIN_DEBUG

  SbVec3f dst;
  SbVec3f points[2] = {SbVec3f(this->minpt), SbVec3f(this->maxpt)};
  SbVec3f corner;
  SbBox3i32 newbox;

  //transform all the corners and include them into the new box.
  for (int i = 0; i < 8; ++i) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0], points[(i&2)>>1][1], points[i&1][2]);
    m.multVecMatrix(corner, dst);
    newbox.extendBy(dst);
  }
  this->setBounds(newbox.minpt, newbox.maxpt);
}

void
SbBox3i32::makeEmpty(void)
{
  minpt.setValue(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());
  maxpt.setValue(-std::numeric_limits<int32_t>::max(), -std::numeric_limits<int32_t>::max(), -std::numeric_limits<int32_t>::max());
}

SbBool
SbBox3i32::intersect(const SbVec3i32 & pt) const
{
  return !(pt[0] < minpt[0] || pt[0] > maxpt[0] ||
           pt[1] < minpt[1] || pt[1] > maxpt[1] ||
           pt[2] < minpt[2] || pt[2] > maxpt[2]);
}

SbBool
SbBox3i32::intersect(const SbBox3i32 & bb) const
{
  return !((bb.maxpt[0] < minpt[0]) || (bb.minpt[0] > maxpt[0]) ||
           (bb.maxpt[1] < minpt[1]) || (bb.minpt[1] > maxpt[1]) ||
           (bb.maxpt[2] < minpt[2]) || (bb.minpt[2] > maxpt[2]));
}

SbBool
SbBox3i32::intersect(const SbVec3f & pt) const
{
  SbVec3i32 ptmax(pt), ptmin(pt);
  if (float(ptmax[0]) < pt[0]) ++ptmax[0];
  if (float(ptmax[1]) < pt[1]) ++ptmax[1];
  if (float(ptmax[2]) < pt[2]) ++ptmax[2];
  return !((ptmin[0] < minpt[0]) || (ptmax[0] > maxpt[0]) ||
           (ptmin[1] < minpt[1]) || (ptmax[1] > maxpt[1]) ||
           (ptmin[2] < minpt[2]) || (ptmax[2] > maxpt[2]));
}

SbBool
SbBox3i32::outside(const SbMatrix & MVP, int & cullbits) const
{
  // FIXME: this function is untested (code written by
  // pederb). 20000615 mortene.

  int i;
  SbVec3f tmp;
  SbVec3f clip[8];
  for (i = 0; i < 8; ++i) {
    tmp[0] = i & 4 ? float(minpt[0]) : float(maxpt[0]);
    tmp[1] = i & 2 ? float(minpt[1]) : float(maxpt[1]);
    tmp[2] = i & 1 ? float(minpt[2]) : float(maxpt[2]);
    MVP.multVecMatrix(tmp, clip[i]);
  }
  for (int j = 0; j < 3; j++) {
    if (cullbits & (1<<j)) {
      int inside = 0;
      int outsideneg = 0;
      int outsidepos = 0;
      for (i = 0; i < 8; i++) {
        float val = clip[i][j];
        if (val < -1.0f) outsideneg++;
        else if (val > 1.0f) outsidepos++;
        else inside++;
      }
      if (outsidepos == 8) return TRUE;
      if (outsideneg == 8) return TRUE;
      if (inside == 8) cullbits ^= (1<<j);
    }
  }
  return FALSE;
}

SbVec3f
SbBox3i32::getClosestPoint(const SbVec3f & pt) const
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

void
SbBox3i32::getSpan(const SbVec3f & dir, float & dmin, float & dmax) const
{
  float dist, mindist = std::numeric_limits<float>::max(), maxdist = -std::numeric_limits<float>::max();
  SbVec3f points[2] = { SbVec3f(this->minpt), SbVec3f(this->maxpt) };
  SbVec3f corner;
  SbVec3f normdir(dir);
  if (normdir.normalize() == 0.0f) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbBox3f::getSpan",
                              "The direction is a null vector.");
#endif // COIN_DEBUG

    dmin = dmax = 0.0f;
    return;
  }

  for (int i = 0; i < 8; ++i) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0], points[(i&2)>>1][1], points[i&1][2]);

    // a = dir
    // b = corner-[0, 0, 0]
    // v = dir/|dir|
    //
    // The dot product formula (1) gives the formula we use (2).
    //
    // (1)  a * b = |a|*|b| * cos(angle(a, b))
    //
    // (2)  distance = v * b
    //
    // This dot product (2) gives the distance from origo to the plane
    // defined by normdir and corner.
    dist=normdir.dot(corner);
    if (dist < mindist) mindist = dist;
    if (dist > maxdist) maxdist = dist;
  }
  dmin = mindist;
  dmax = maxdist;
}


#ifdef COIN_TEST_SUITE
BOOST_AUTO_TEST_CASE(checkSize) {
  SbVec3i32 min(1,2,3);
  SbVec3i32 max(3,4,5);

  SbVec3i32 diff = max - min;

  
  SbBox3i32 box(min, max);
  SbVec3i32 res = box.getSize();

  BOOST_CHECK_MESSAGE(box.getSize() == diff,
                      "Box has incorrect size");

}
#endif //COIN_TEST_SUITE
