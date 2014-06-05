// Begin License:
// Copyright (C) 2006-2011 Tobias Sargeant (tobias.sargeant@gmail.com).
// All rights reserved.
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// This file may be used under the terms of the GNU General Public
// License version 2.0 as published by the Free Software Foundation
// and appearing in the file LICENSE.GPL2 included in the packaging of
// this file.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// End:


#pragma once

#include <carve/carve.hpp>

#include <carve/vector.hpp>
#include <carve/mesh.hpp>

#include <carve/octree_decl.hpp>
#include <carve/octree_impl.hpp>

#include "rgb.hpp"

void drawMeshSetWireframe(carve::mesh::MeshSet<3> *poly, int group, bool draw_normal, bool draw_edgeconn);
void drawMeshSet(carve::mesh::MeshSet<3> *poly, float r, float g, float b, float a, int group = -1);

void drawColourPoly(const carve::geom3d::Vector &normal, std::vector<std::pair<carve::geom3d::Vector, cRGBA> > &v);
void drawColourFace(carve::mesh::Face<3> *face, const std::vector<cRGBA> &vc);
void drawFace(carve::mesh::Face<3> *face, cRGBA fc);

void installDebugHooks();
void drawTri(const carve::geom::tri<3> &);
void drawSphere(const carve::geom::sphere<3> &);
void drawCube(const carve::geom3d::Vector &, const carve::geom3d::Vector &);
void drawSphere(const carve::geom3d::Vector &, double);
void drawOctree(const carve::csg::Octree &);

extern carve::geom3d::Vector g_translation;
extern double g_scale;
