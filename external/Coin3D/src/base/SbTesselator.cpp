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
  \class SbTesselator SbTesselator.h Inventor/SbTesselator.h
  \brief The SbTesselator class is used to tessellate polygons into triangles.
  \ingroup base

  SbTesselator is used within Coin to split polygons into
  triangles. It handles concave polygons, does Delaunay triangulation
  and avoids generating self-intersecting triangles.

  Here's a simple example which shows how to tessellate a quad polygon
  with corners in <0, 0, 0>, <1, 0, 0>, <1, 1, 0> and <0, 1, 0>.

  \code

  // Callback function for the tessellator. Called once for each
  // generated triangle with the vertices.
  static void
  tess_cb(void * v0, void * v1, void * v2, void * cbdata)
  {
    SbVec3f * vtx0 = (SbVec3f *)v0;
    SbVec3f * vtx1 = (SbVec3f *)v1;
    SbVec3f * vtx2 = (SbVec3f *)v2;
    (void) fprintf(stdout, "triangle: <%f, %f, %f> <%f, %f, %f> <%f, %f, %f>\n",
      (*vtx0)[0], (*vtx0)[1], (*vtx0)[2],
      (*vtx1)[0], (*vtx1)[1], (*vtx1)[2],
      (*vtx2)[0], (*vtx2)[1], (*vtx2)[2]);

    // Do stuff with triangle here.
  }

  static SbVec3f vertices[] = {
    SbVec3f(1, 0, 0), SbVec3f(1, 1, 0),
    SbVec3f(0, 1, 0), SbVec3f(0, 0, 0)
  };

  SbTesselator mytessellator(tess_cb, NULL);
  mytessellator.beginPolygon();
  for (int i=0; i < 4; i++) {
    mytessellator.addVertex(vertices[i], &vertices[i]);
  }
  mytessellator.endPolygon();

  \endcode

  The call to SbTesselator::endPolygon() triggers the SbTesselator to
  spring into action, calling the tess_cb() function for each triangle
  it generates.

  The reason we use 2 arguments to SbTesselator::addVertex() and passes
  void pointers for the vertices to the callback function is to make it
  possible to have more complex structures than just the coordinates
  themselves (as in the example above), like material information,
  lighting information or whatever other attributes your vertices have.

  This class is not part of the original Open Inventor API.


  Another option for tessellating polygons is the tessellator of the
  GLU library. It has some features not part of SbTesselator (like
  handling hulls), but the GLU library is known to have bugs in
  various implementations and doesn't do Delaunay triangulation. If
  you however still prefer to use the GLU tessellator instead of this
  one, that can be forced by setting an environment variable:

  \code
  (void) coin_setenv("COIN_PREFER_GLU_TESSELLATOR", "1", 1);
  \endcode
*/

// *************************************************************************

/*! \file SbTesselator.h */
#include <Inventor/SbTesselator.h>

#include <cstdio>
#include <climits>
#include <cassert>
#include <cfloat>

#include <Inventor/C/base/heap.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/SbSphere.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  \typedef void SbTesselatorCB(void * v0, void * v1, void * v2, void * data)

  The type definition of the callback function which is called for each triangle
  returned by the tesselator.
*/

// *************************************************************************

class SbTesselator::PImpl {
public:

#if 1 // enable this to do double-precision internal calculations
  typedef double tessfloat_t;
#else // or else do single-precision calculations.
  // FIXME: we should normally want do to double-precision, but this
  // is kept around for now since there are suspected bugs in the
  // tessellation code which are easier to reproduce with
  // single-precision calculations.  -mortene.
  typedef float tessfloat_t;
#endif

  // this type is for internal real values which is forced by external
  // factors to be single-precision floats. (e.g. coming into the api
  // as an SbVec3f.)
  typedef float forcedfloat_t;

  struct Vertex {
    SbTesselator::PImpl * thisp;

    SbVec3f v;
    void * data;

    tessfloat_t weight;
    int dirtyweight;

    Vertex * prev, * next;
  };

  PImpl(void) : bsptree(256) { }
  cc_heap * heap;
  SbBSPTree bsptree;
  SbList <int> clippablelist;
  tessfloat_t epsilon;
  SbBox3f bbox;

  Vertex * newVertex(void);
  void cleanUp(void);

  int currVertex;
  SbList <struct Vertex *> vertexStorage;

  Vertex * headV, * tailV;
  int numVerts;
  SbVec3f polyNormal;
  int X,Y;
  int polyDir;
  void (*callback)(void * v0,void * v1,void * v2,void * data);
  void * callbackData;
  SbBool hasNormal;
  SbBool keepVertices;

  void emitTriangle(Vertex * v);
  void cutTriangle(Vertex * t);
  void calcPolygonNormal(void);

  SbBool circleCenter(const SbVec3f &a, const SbVec3f &b,
                      const SbVec3f &c, tessfloat_t &cx, tessfloat_t &cy);
  tessfloat_t circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c);
  tessfloat_t circleSize(Vertex * v);
  tessfloat_t dot2D(const SbVec3f &v1, const SbVec3f &v2);
  SbBool clippable(Vertex * v);
  SbBool isTriangle(Vertex * v);
  SbBool pointInTriangle(Vertex * p, Vertex * t);
  tessfloat_t area(Vertex * t);

  static tessfloat_t heap_evaluate(void * v);
  static int heap_compare(void * v0, void * v1);

  static bool point_on_edge(tessfloat_t x, tessfloat_t y,
                            const forcedfloat_t * v0,
                            const forcedfloat_t * v1,
                            int X, int Y, tessfloat_t eps);
};

#define PRIVATE(obj) ((obj)->pimpl)

// *************************************************************************

int
SbTesselator::PImpl::heap_compare(void * h0, void * h1)
{
  if (heap_evaluate(h0) > heap_evaluate(h1)) return -1;
  return 1;
}

SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::heap_evaluate(void * v)
{
  Vertex * vertex = static_cast<Vertex *>(v);
  if (vertex->dirtyweight) {
    vertex->dirtyweight = 0;
    if ((vertex->thisp->area(vertex) > vertex->thisp->epsilon) &&
        vertex->thisp->isTriangle(vertex) &&
        vertex->thisp->clippable(vertex)) {
#if 0 // testing code to avoid empty triangles
      vertex->weight = vertex->thisp->circleSize(vertex);
      Vertex *v2 = vertex->next;
      if (vertex->weight != FLT_MAX &&
          v2->thisp->keepVertices &&
          v2->thisp->numVerts > 4 &&
          fabs(v2->thisp->area(v2)) < PRIVATE(thisp)->epsilon) {
        vertex->weight = 0.0f; // cut immediately!
      }

#else
      vertex->weight = vertex->thisp->circleSize(vertex);
#endif
    }
    else
      vertex->weight = FLT_MAX;
  }
  return vertex->weight;
}

// *************************************************************************

/*!
  Initializes a tessellator. The \a callback argument specifies a
  function which will be called for each triangle returned by the
  tessellator. The callback function will get three pointers to each
  vertex and the \a userdata pointer. The vertex pointers are
  specified in the SbTesselator::addVertex() method.
*/
SbTesselator::SbTesselator(SbTesselatorCB * func, void * data)
{
  this->setCallback(func, data);
  PRIVATE(this)->headV = PRIVATE(this)->tailV = NULL;
  PRIVATE(this)->currVertex = 0;

  PRIVATE(this)->heap =
    cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb *>(PImpl::heap_compare), TRUE);
  PRIVATE(this)->epsilon = FLT_EPSILON;
}

/*!
  Destructor.
*/
SbTesselator::~SbTesselator()
{
  PRIVATE(this)->cleanUp();
  int i, n = PRIVATE(this)->vertexStorage.getLength();
  for (i = 0; i < n; i++) { delete PRIVATE(this)->vertexStorage[i]; }

  cc_heap_destruct(PRIVATE(this)->heap);
}

// *************************************************************************

/*!
  Initializes new polygon.

  You can explicitly set the polygon normal if you know what it
  is. Otherwise it will be calculated internally.

  If \a keepVerts is \c TRUE, all vertices will be included in the
  returned triangles, even though this might lead to triangles without
  area.
*/
void
SbTesselator::beginPolygon(SbBool keepVerts, const SbVec3f &normal)
{
  PRIVATE(this)->cleanUp();
  PRIVATE(this)->keepVertices = keepVerts;
  if (normal != SbVec3f(0.0f, 0.0f, 0.0f)) {
    PRIVATE(this)->polyNormal = normal;
    PRIVATE(this)->hasNormal = TRUE;
  }
  else {
    PRIVATE(this)->hasNormal = FALSE;
  }
  PRIVATE(this)->headV = PRIVATE(this)->tailV = NULL;
  PRIVATE(this)->numVerts = 0;
  PRIVATE(this)->bbox.makeEmpty();
}

/*!
  Adds a new vertex to the polygon. \a data will be returned as a
  vertex in the callback-function.
*/
void
SbTesselator::addVertex(const SbVec3f &v,void *data)
{
  if (PRIVATE(this)->tailV &&
      !PRIVATE(this)->keepVertices &&
      v == PRIVATE(this)->tailV->v)
    return;

  PRIVATE(this)->bbox.extendBy(v);

  PImpl::Vertex *newv = PRIVATE(this)->newVertex();
  newv->v = v;
  newv->data = data;
  newv->next = NULL;
  newv->dirtyweight = 1;
  newv->weight = FLT_MAX;
  newv->prev = PRIVATE(this)->tailV;
  newv->thisp = &(PRIVATE(this).get());
  if (!PRIVATE(this)->headV) PRIVATE(this)->headV = newv;
  if (PRIVATE(this)->tailV) PRIVATE(this)->tailV->next = newv;
  PRIVATE(this)->tailV = newv;
  PRIVATE(this)->numVerts++;
}

/*!
  Signals the tessellator to begin tessellating. The callback function
  specified in the constructor (or set using the
  SbTesselator::setCallback() method) will be called for each triangle
  before returning.
*/
void
SbTesselator::endPolygon(void)
{
  // projection enums
  enum { OXY, OXZ, OYZ };

  // check for special case when last point equals the first point
  if (!PRIVATE(this)->keepVertices && PRIVATE(this)->numVerts >= 3) {
    PImpl::Vertex * first = PRIVATE(this)->headV;
    PImpl::Vertex * last = PRIVATE(this)->tailV;
    if (first->v == last->v) {
      PImpl::Vertex * newlast = last->prev;
      newlast->next = NULL;
      // don't delete old tail. We have some special memory handling
      // in this class
      PRIVATE(this)->tailV = newlast;
      PRIVATE(this)->numVerts--;
    }
  }

  PImpl::Vertex *v;

  if (PRIVATE(this)->numVerts > 3) {
    PRIVATE(this)->calcPolygonNormal();

    // Find best projection plane
    int projection;
    if (fabs(PRIVATE(this)->polyNormal[0]) > fabs(PRIVATE(this)->polyNormal[1]))
      if (fabs(PRIVATE(this)->polyNormal[0]) > fabs(PRIVATE(this)->polyNormal[2]))
        projection=OYZ;
      else projection=OXY;
    else
      if (fabs(PRIVATE(this)->polyNormal[1]) > fabs(PRIVATE(this)->polyNormal[2]))
        projection=OXZ;
      else projection=OXY;

    switch (projection) {
    case OYZ:
      PRIVATE(this)->X=1;
      PRIVATE(this)->Y=2;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[0]/fabs(PRIVATE(this)->polyNormal[0]));
      break;
    case OXY:
      PRIVATE(this)->X=0;
      PRIVATE(this)->Y=1;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[2]/fabs(PRIVATE(this)->polyNormal[2]));
      break;
    case OXZ:
      PRIVATE(this)->X=2;
      PRIVATE(this)->Y=0;
      PRIVATE(this)->polyDir=static_cast<int>(PRIVATE(this)->polyNormal[1]/fabs(PRIVATE(this)->polyNormal[1]));
      break;
    }

    // find epsilon based on bbox
    SbVec3f d;
    PRIVATE(this)->bbox.getSize(d[0],d[1],d[2]);
    PRIVATE(this)->epsilon = SbMin(d[PRIVATE(this)->X], d[PRIVATE(this)->Y]) * FLT_EPSILON * FLT_EPSILON;

    //Make loop
    PRIVATE(this)->tailV->next = PRIVATE(this)->headV;
    PRIVATE(this)->headV->prev = PRIVATE(this)->tailV;

    // add all vertices to heap.
    cc_heap_clear(PRIVATE(this)->heap);
    PRIVATE(this)->bsptree.clear(256);

    // use two loops to add points to bsptree and heap, since the heap
    // requires that the bsptree is fully set up to evaluate
    // correctly.
    v = PRIVATE(this)->headV;
    do {
      PRIVATE(this)->bsptree.addPoint(SbVec3f(v->v[PRIVATE(this)->X],
                                              v->v[PRIVATE(this)->Y],
                                              0.0f), v);
      v = v->next;
    } while (v != PRIVATE(this)->headV);

    do {
      cc_heap_add(PRIVATE(this)->heap, v);
      v = v->next;
    } while (v != PRIVATE(this)->headV);

    while (PRIVATE(this)->numVerts > 4) {
      v = static_cast<PImpl::Vertex *>(cc_heap_get_top(PRIVATE(this)->heap));
      if (PImpl::heap_evaluate(v) == FLT_MAX) break;
      cc_heap_remove(PRIVATE(this)->heap, v->next);
      PRIVATE(this)->bsptree.removePoint(SbVec3f(v->next->v[PRIVATE(this)->X],
                                                 v->next->v[PRIVATE(this)->Y],
                                                 0.0f));
      PRIVATE(this)->emitTriangle(v); // will remove v->next
      PRIVATE(this)->numVerts--;

      v->prev->dirtyweight = 1;
      v->dirtyweight = 1;

      (void) cc_heap_remove(PRIVATE(this)->heap, v->prev);
      (void) cc_heap_remove(PRIVATE(this)->heap, v);

      cc_heap_add(PRIVATE(this)->heap, v->prev);
      cc_heap_add(PRIVATE(this)->heap, v);
    }

    // remember that headV and tailV are not valid anymore!

    //
    // must handle special case when only four vertices remain
    //
    if (PRIVATE(this)->numVerts == 4) {
      SbTesselator::PImpl::tessfloat_t v0 = SbMax(PImpl::heap_evaluate(v), PImpl::heap_evaluate(v->next->next));
      SbTesselator::PImpl::tessfloat_t v1 = SbMax(PImpl::heap_evaluate(v->next), PImpl::heap_evaluate(v->prev));

      // abort if vertices should not be kept
      if (v0 == v1 && v0 == FLT_MAX && !PRIVATE(this)->keepVertices) return;

      if (v0 < v1) {
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->emitTriangle(v);
      }
      else {
        v = v->next;
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->emitTriangle(v);
      }
      PRIVATE(this)->numVerts -= 2;
    }

    // Emit the empty triangles that might lay around
    if (PRIVATE(this)->keepVertices) {
      while (PRIVATE(this)->numVerts>=3) {
        PRIVATE(this)->emitTriangle(v);
        PRIVATE(this)->numVerts--;
      }
    }
  }
  else if (PRIVATE(this)->numVerts==3) {   //only one triangle
    PRIVATE(this)->emitTriangle(PRIVATE(this)->headV);
  }
}

// *************************************************************************

/*!
  Sets the callback function for this tessellator.
*/
void
SbTesselator::setCallback(SbTesselatorCB * func, void *data)
{
  PRIVATE(this)->callback = func;
  PRIVATE(this)->callbackData = data;
}

// *************************************************************************

bool
SbTesselator::PImpl::point_on_edge(tessfloat_t x, tessfloat_t y,
                                   const forcedfloat_t * v0,
                                   const forcedfloat_t * v1,
                                   int X, int Y, tessfloat_t eps)
{
  if (x < v0[X] && x < v1[X]) return false;
  if (x > v0[X] && x > v1[X]) return false;
  if (y < v0[Y] && y < v1[Y]) return false;
  if (y > v0[Y] && y > v1[Y]) return false;

  if (v0[X] == v1[X]) {
    if (fabs(x-v0[X]) <= eps) return true;
    return false;
  }

  tessfloat_t ny = v0[Y] + (x-v0[X])*(v1[Y]-v0[Y])/(v1[X]-v0[X]);

  if (fabs(y-ny) <= eps) {
    return true;
  }
  return false;
}

//
// PRIVATE FUNCTIONS:
//

//
// Checks if the point p lies inside the triangle
// starting with t.
// Algorithm from comp.graphics.algorithms FAQ
//
SbBool
SbTesselator::PImpl::pointInTriangle(Vertex *p, Vertex *t)
{
  tessfloat_t x,y;
  SbBool tst = FALSE;

  x = p->v[X];
  y = p->v[Y];

  const forcedfloat_t * v1 = t->v.getValue();
  const forcedfloat_t * v2 = t->next->next->v.getValue();

  if ((((v1[Y] <= y) && (y < v2[Y])) || ((v2[Y] <= y)  && (y < v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) / (v2[Y] - v1[Y]) + v1[X])) {
    tst = !tst;
  }

  v2 = v1;
  v1 = t->next->v.getValue();

  if ((((v1[Y] <= y) && (y < v2[Y])) || ((v2[Y] <= y)  && (y < v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) / (v2[Y] - v1[Y]) + v1[X])) {
    tst = !tst;
  }

  v2 = v1;
  v1 = t->next->next->v.getValue();

  if ((((v1[Y] <= y) && (y < v2[Y])) || ((v2[Y] <= y)  && (y < v1[Y]))) &&
      (x < (v2[X] - v1[X]) * (y - v1[Y]) / (v2[Y] - v1[Y]) + v1[X])) {
    tst = !tst;
  }

  // the pointInTriangle test might fail for vertices that are on one
  // of the triangle edges. Do a point_on_edge test for all three
  // edges to handle this case. Example model that fails without this
  // test:
  //
  //  ShapeHints { faceType UNKNOWN_FACE_TYPE vertexOrdering CLOCKWISE }
  //  IndexedFaceSet {
  //    vertexProperty
  //    VertexProperty {
  //      vertex [ -0.3 0.05 0.0,
  //                0.3   0.05  0.0,
  //                0.3   0.1   0.0,
  //                0.5   0.0   0.0,
  //                0.3  -0.1   0.0,
  //                0.3  -0.05  0.0,
  //               -0.3  -0.05  0.0,
  //               -0.3  -0.1   0.0,
  //               -0.5   0.0   0.0,
  //               -0.3   0.1   0.0 ]
  //    }
  //    coordIndex [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1]
  if (!tst) {
    if (point_on_edge(x, y, t->v.getValue(),
                      t->next->v.getValue(), X, Y, this->epsilon)) {
      return TRUE;
    }
    if (point_on_edge(x, y, t->next->v.getValue(),
                      t->next->next->v.getValue(), X, Y, this->epsilon)) {
      return TRUE;
    }
    if (point_on_edge(x, y, t->next->next->v.getValue(),
                      t->v.getValue(), X, Y, this->epsilon)) {
      return TRUE;
    }
  }
  return tst;
}


//
// Check if v points to a legal triangle (normal is right way)
// (i.e convex or concave corner)
//
SbBool
SbTesselator::PImpl::isTriangle(Vertex * v)
{
  return (((v->next->v[X]-v->v[X]) * (v->next->next->v[Y]-v->v[Y]) -
           (v->next->v[Y]-v->v[Y]) * (v->next->next->v[X]-v->v[X])) *
          this->polyDir > 0.0) ? TRUE : FALSE;
}

//
// Check if there are no intersection to the triangle
// pointed to by v. (no other vertices are inside the triangle)
//
SbBool
SbTesselator::PImpl::clippable(Vertex * v)
{
  SbBox3f boundingbox;
  boundingbox.makeEmpty();
  boundingbox.extendBy(SbVec3f(v->v[X], v->v[Y], 0.0f));
  boundingbox.extendBy(SbVec3f(v->next->v[X], v->next->v[Y], 0.0f));
  boundingbox.extendBy(SbVec3f(v->next->next->v[X], v->next->next->v[Y], 0.0f));

  SbSphere sphere;
  sphere.circumscribe(boundingbox);

  SbList <int> & l = this->clippablelist;
  l.truncate(0);
  this->bsptree.findPoints(sphere, l);
  for (int i = 0; i < l.getLength(); i++) {
    Vertex * vtx = static_cast<Vertex*>(this->bsptree.getUserData(l[i]));
    if (vtx != v && vtx != v->next && vtx != v->next->next) {
      if (pointInTriangle(vtx, v)) { return FALSE; }
    }
  }
  return TRUE;
}



//
// Call the callback-function for the triangle starting with t
//
void
SbTesselator::PImpl::emitTriangle(Vertex * t)
{
  assert(t);
  assert(t->next);
  assert(t->next->next);
  assert(this->callback);

  this->callback(t->data, t->next->data, t->next->next->data,
                 this->callbackData);
  this->cutTriangle(t);
}

// Cuts t->next out of the triangle vertex list.
//
// FIXME: bad design, this should have been a method on
// Vertex. 20031007 mortene.
void
SbTesselator::PImpl::cutTriangle(Vertex * t)
{
  t->next->next->prev = t;
  t->next = t->next->next;
}

//
// Return the area of the triangle starting with v
//
SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::area(Vertex * v)
{
  return static_cast<tessfloat_t>(fabs(((v->next->v[X]-v->v[X])*(v->next->next->v[Y]-v->v[Y])-
                     (v->next->v[Y]-v->v[Y])*(v->next->next->v[X]-v->v[X]))));
}

//
// Returns the center of the circle through points a, b, c.
//
SbBool
SbTesselator::PImpl::circleCenter(const SbVec3f &a, const SbVec3f &b,
                                  const SbVec3f &c, tessfloat_t &cx, tessfloat_t &cy)
{
  tessfloat_t d1, d2, d3, c1, c2, c3;
  SbVec3f tmp1, tmp2;

  tmp1 = b - a;
  tmp2 = c - a;
  d1 = dot2D(tmp1, tmp2);

  tmp1 = b - c;
  tmp2 = a - c;
  d2 = dot2D(tmp1, tmp2);

  tmp1 = a - b;
  tmp2 = c - b;
  d3 = dot2D(tmp1, tmp2);

  c1 = d2 * d3;
  c2 = d3 * d1;
  c3 = d1 * d2;

  SbVec3f tmp4(c);
  tmp1 = a;
  tmp2 = b;
  tmp1 *= float(c2+c3);
  tmp2 *= float(c3+c1);
  tmp4 *= float(c1+c2);
  tmp4 += tmp1;
  tmp4 += tmp2;

  tessfloat_t div = 2.0f*(c1+c2+c3);
  if (div != 0.0f) {
    tessfloat_t val = 1.0f / div;
    cx = tmp4[X] * val;
    cy = tmp4[Y] * val;
    return TRUE;
  }
  return FALSE;
}

//
// Returns the square of the radius of the circle through a, b, c
//
SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::circleSize(const SbVec3f &a, const SbVec3f &b, const SbVec3f &c)
{
  tessfloat_t cx, cy;
  if (circleCenter(a, b, c, cx, cy)) {
    tessfloat_t t1, t2;
    t1 = a[X] - cx;
    t2 = a[Y] - cy;
    return t1*t1+t2*t2;
  }
  return FLT_MAX;
}

SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::circleSize(Vertex *v)
{
  return circleSize(v->v, v->next->v, v->next->next->v);
}

SbTesselator::PImpl::tessfloat_t
SbTesselator::PImpl::dot2D(const SbVec3f &v1, const SbVec3f &v2)
{
  return v1[X] * v2[X] + v1[Y] * v2[Y];
}

void
SbTesselator::PImpl::calcPolygonNormal()
{
  assert(this->numVerts > 3);

  this->polyNormal.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f vert1, vert2;
  Vertex *currvertex = this->headV;
  vert2 = currvertex->v;

  while (currvertex->next != NULL && currvertex != tailV) {
    vert1 = vert2;
    vert2 = currvertex->next->v;
    this->polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
    this->polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
    this->polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);
    currvertex = currvertex->next;
  }
  vert1 = vert2;
  vert2 = headV->v;
  polyNormal[0] += (vert1[1] - vert2[1]) * (vert1[2] + vert2[2]);
  polyNormal[1] += (vert1[2] - vert2[2]) * (vert1[0] + vert2[0]);
  polyNormal[2] += (vert1[0] - vert2[0]) * (vert1[1] + vert2[1]);

  if (polyNormal.normalize() == 0.0f) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbTesselator::calcPolygonNormal",
                              "Polygon has no normal.");
#endif // COIN_DEBUG
  }
}

//
// makes sure Vertexes are not allocated and deallocated
// all the time, by storing them in a growable array. This
// way, the Vertexes will not be deleted until the tessellator
// is destructed, and Vertexes can be reused.
//
SbTesselator::PImpl::Vertex *
SbTesselator::PImpl::newVertex()
{
  assert(this->currVertex <= this->vertexStorage.getLength());
  if (this->currVertex == this->vertexStorage.getLength()) {
    Vertex * v = new Vertex;
    this->vertexStorage.append(v);
  }
  return this->vertexStorage[currVertex++];
}

void
SbTesselator::PImpl::cleanUp()
{
  this->headV = this->tailV = NULL;
  this->currVertex = 0;
  this->numVerts = 0;
}

// *************************************************************************

#undef PRIVATE
