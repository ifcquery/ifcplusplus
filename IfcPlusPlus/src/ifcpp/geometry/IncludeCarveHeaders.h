/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#pragma warning (disable: 4267 4334 4244 4100 4996 4589 4456 4457 4458 4589 4018 4211)
#ifndef CARVE_USE_EXACT_PREDICATES
	#define CARVE_USE_EXACT_PREDICATES
#endif
#ifndef CARVE_SYSTEM_BOOST
	#define CARVE_SYSTEM_BOOST
#endif

#include <vector>
#include <algorithm>
#include <carve/carve.hpp>
#include <carve/matrix.hpp>
#include <carve/geom2d.hpp>
#include <carve/geom3d.hpp>
#include <carve/poly.hpp>
#include <carve/polyhedron_base.hpp>
#include <carve/mesh.hpp>
#include <carve/mesh_simplify.hpp>
#include <carve/input.hpp>

#ifndef CARVE_CSG_TRIANGULATOR_INCLUDE
	#define CARVE_CSG_TRIANGULATOR_INCLUDE
	#include <carve/csg_triangulator.hpp>
#endif

#ifndef CARVE_VEC2_VEC3_DEFINED
#define CARVE_VEC2_VEC3_DEFINED
typedef carve::geom::vector<2> vec2;
typedef carve::geom::vector<3> vec3;
#endif

namespace carve{
	namespace geom2d{
		int lineSegmentPolyIntersections( const std::vector<vec2>& points, carve::geom2d::LineSegment2 line, std::vector<carve::geom2d::PolyIntersectionInfo> &out );
	}
}
