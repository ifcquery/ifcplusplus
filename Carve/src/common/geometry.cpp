// Copyright 2006-2015 Tobias Sargeant (tobias.sargeant@gmail.com).
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#if defined(HAVE_CONFIG_H)
#  include <carve_config.h>
#endif

#include "geometry.hpp"
#include <carve/input.hpp>

carve::mesh::MeshSet<3> *makeCube(
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;

  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, +1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, -1.0, +1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, +1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, -1.0));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, -1.0, -1.0));

  data.addFace(0, 1, 2, 3);
  data.addFace(7, 6, 5, 4);
  data.addFace(0, 4, 5, 1);
  data.addFace(1, 5, 6, 2);
  data.addFace(2, 6, 7, 3);
  data.addFace(3, 7, 4, 0);

  return new carve::mesh::MeshSet<3>(data.points, data.getFaceCount(), data.faceIndices);
}

static bool _all(int /* x */, int /* y */, int /* z */) {
  return true;
}

carve::mesh::MeshSet<3> *makeSubdividedCube(
    int sub_x,
    int sub_y,
    int sub_z,
    bool (*inc)(int, int, int),
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;

  if (inc == NULL) inc = _all;

  data.reserveVertices((sub_x + 1) * (sub_y + 1) * (sub_z + 1));
  for (int _z = 0; _z < sub_z + 1; ++_z) {
    double z = 1.0 - 2.0 * _z / sub_z;
    for (int _y = 0; _y < sub_y + 1; ++_y) {
      double y = 1.0 - 2.0 * _y / sub_y;
      for (int _x = 0; _x < sub_x + 1; ++_x) {
        double x = 1.0 - 2.0 * _x / sub_x;
        data.addVertex(transform * carve::geom::VECTOR(x, y, z));
      }
    }
  }
#define OK(x, y, z) ((x) >= 0 && (x) < sub_x && (y) >= 0 && (y) < sub_y && (z) >= 0 && (z) < sub_z)
#define I(x, y, z) ((x) + (y) * (sub_x + 1) + (z) * ((sub_x + 1) * (sub_y + 1)))
#define FACE(a, b, c, d) data.addFace(idx[a], idx[b], idx[c], idx[d])
  for (int _z = 0; _z < sub_z; ++_z) {
    for (int _y = 0; _y < sub_y; ++_y) {
      for (int _x = 0; _x < sub_x; ++_x) {
        int idx[8] = {
          I(_x,   _y,   _z),
          I(_x+1, _y,   _z),
          I(_x+1, _y+1, _z),
          I(_x,   _y+1, _z),
          I(_x,   _y,   _z+1),
          I(_x+1, _y,   _z+1),
          I(_x+1, _y+1, _z+1),
          I(_x,   _y+1, _z+1)
        };
        if (!inc(_x, _y, _z)) continue;
        if (!OK(_x - 1, _y, _z) || !inc(_x - 1, _y, _z)) FACE(3, 7, 4, 0);
        if (!OK(_x + 1, _y, _z) || !inc(_x + 1, _y, _z)) FACE(1, 5, 6, 2);
        if (!OK(_x, _y - 1, _z) || !inc(_x, _y - 1, _z)) FACE(0, 4, 5, 1);
        if (!OK(_x, _y + 1, _z) || !inc(_x, _y + 1, _z)) FACE(2, 6, 7, 3);
        if (!OK(_x, _y, _z - 1) || !inc(_x, _y, _z - 1)) FACE(0, 1, 2, 3);
        if (!OK(_x, _y, _z + 1) || !inc(_x, _y, _z + 1)) FACE(7, 6, 5, 4);

      }
    }
  }

  return data.createMesh(carve::input::opts());
}

carve::mesh::MeshSet<3> *makeDoubleCube(
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, -1.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(+0.0, -1.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(+0.0, -1.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(-1.0, +0.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(-1.0, +0.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(+0.0, +0.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(+0.0, +0.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(+0.0, +1.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(+0.0, +1.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(+1.0, +0.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, +0.0, +0.5));

  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, -0.5));
  data.addVertex(transform * carve::geom::VECTOR(+1.0, +1.0, +0.5));

  data.addFace(0, 4, 6, 2);
  data.addFace(6, 8, 12, 10);
  data.addFace(1, 3, 7, 5);
  data.addFace(7, 11, 13, 9);

  data.addFace(1, 5, 4, 0);
  data.addFace(3, 1, 0, 2);
  data.addFace(7, 3, 2, 6);
  data.addFace(5, 7, 6, 4);

  data.addFace(11, 7, 6, 10);
  data.addFace(13, 11, 10, 12);
  data.addFace(9, 13, 12, 8);
  data.addFace(7, 9, 8, 6);

  return data.createMesh(carve::input::opts());
}

carve::mesh::MeshSet<3> *makeTorus(
    int slices,
    int rings,
    double rad1,
    double rad2,
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;
  data.reserveVertices(slices * rings);

  for (int i = 0; i < slices; i++) {
    double a1 = i * M_PI * 2.0 / slices;
    double dy = cos(a1);
    double dx = sin(a1);
    for (int j = 0; j < rings; j++) {
      double a2 = j * M_PI * 2.0 / rings;
      double x = dx * (rad1 + cos(a2) * rad2);
      double y = dy * (rad1 + cos(a2) * rad2);
      double z = sin(a2) * rad2;
      data.addVertex(transform * carve::geom::VECTOR(x, y, z));
    }
  }

#define V(i, j) ((i) * rings + (j))

  data.reserveFaces(slices * rings, 4);
  for (int i = 0; i < slices; i++) {
    int i2 = (i + 1) % slices;
    for (int j = 0; j < rings; j++) {
      int j2 = (j + 1) % rings;
      data.addFace(V(i, j), V(i, j2), V(i2, j2), V(i2, j));
    }
  }
#undef V

  return data.createMesh(carve::input::opts());
}

carve::mesh::MeshSet<3> *makeCylinder(
    int slices,
    double rad,
    double height,
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;
  data.reserveVertices(slices * 2 + 2);

  data.addVertex(transform * carve::geom::VECTOR(0, 0, +height/2));
  data.addVertex(transform * carve::geom::VECTOR(0, 0, -height/2));

  for (int i = 0; i < slices; i++) {
    double a1 = i * M_PI * 2.0 / slices;
    double y = cos(a1) * rad;
    double x = sin(a1) * rad;
    data.addVertex(transform * carve::geom::VECTOR(x, y, +height/2));
    data.addVertex(transform * carve::geom::VECTOR(x, y, -height/2));
  }

  data.reserveFaces(slices * 3, 4);
  for (int i = 0; i < slices; i++) {
    data.addFace(0,
                 2 + ((i+1) % slices) * 2,
                 2 + i * 2);
  }
  for (int i = 0; i < slices; i++) {
    data.addFace(2 + i * 2,
                 2 + ((i+1) % slices) * 2,
                 3 + ((i+1) % slices) * 2,
                 3 + i * 2);
  }
  for (int i = 0; i < slices; i++) {
    data.addFace(1,
                 3 + i * 2,
                 3 + ((i+1) % slices) * 2);
  }

  return data.createMesh(carve::input::opts());
}

carve::mesh::MeshSet<3> *makeCone(
    int slices,
    double rad,
    double height,
    const carve::math::Matrix &transform) {
  carve::input::PolyhedronData data;
  data.reserveVertices(slices + 2);

  data.addVertex(transform * carve::geom::VECTOR(0, 0, +height/2));
  data.addVertex(transform * carve::geom::VECTOR(0, 0, -height/2));

  for (int i = 0; i < slices; i++) {
    double a1 = i * M_PI * 2.0 / slices;
    double y = cos(a1) * rad;
    double x = sin(a1) * rad;
    data.addVertex(transform * carve::geom::VECTOR(x, y, -height/2));
  }
  data.reserveFaces(slices * 2, 3);
  for (int i = 0; i < slices; i++) {
    data.addFace(0,
                 2 + ((i+1) % slices),
                 2 + i);
  }
  for (int i = 0; i < slices; i++) {
    data.addFace(1,
                 2 + i,
                 2 + ((i+1) % slices));
  }

  return data.createMesh(carve::input::opts());
}
