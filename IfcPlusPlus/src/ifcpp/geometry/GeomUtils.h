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

#include <array>
#include <random>
#include <vector>

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingException.h>
#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "IncludeCarveHeaders.h"
#include "GeometrySettings.h"

typedef std::array<double, 2> array2d;

namespace GeomUtils
{
	inline double length2(const carve::geom::vector<3>& p0, const carve::geom::vector<3>& p1)
	{
		double dx = p1.x - p0.x;
		double dy = p1.y - p0.y;
		double dz = p1.z - p0.z;
		return dx * dx + dy * dy + dz * dz;
	}

	inline void safeNormalize(carve::geom::vector<3>& vec, double eps)
	{
		double len = vec.length2();
		if (len > eps*eps)
		{
			vec.normalize();
		}
	}

	inline void safeNormalize(glm::dvec3& vec, double eps)
	{
		double len = glm::length2(vec);
		if (len > eps*eps)
		{
			vec = glm::normalize(vec);
		}
	}

	enum ProjectionPlane { UNDEFINED, XY_PLANE, YZ_PLANE, XZ_PLANE };

	class Ray
	{
	public:
		void setRay(Ray* other)
		{
			origin = other->origin;
			direction = other->direction;
		}
		void setRay(const Ray& other)
		{
			origin = other.origin;
			direction = other.direction;
		}
		glm::dvec3 origin{ 0.0, 0.0, 0.0 };
		glm::dvec3 direction{ 0.0, 0.0, 1.0 };
	};

	struct Plane
	{
		Plane()
		{
		}
		Plane(const glm::dvec3& _position, const glm::dvec3& _normal)
		{
			m_position = _position;
			m_normal = _normal;
#ifdef _DEBUG
			if (std::abs(glm::length(m_normal) - 1.0) > EPS_DEFAULT)
			{
				std::cout << "Plane::normal not normalized" << std::endl;
			}
#endif
		}

		const glm::dvec3& getPosition() const { return m_position; }
		const glm::dvec3& getNormal() const { return m_normal; }
		const void setNormal(glm::dvec3& norm) { m_normal = norm; }
		const void setPosition(glm::dvec3& pos) { m_position = pos; }

		void setPlane(const glm::dvec3& _position, const glm::dvec3& _normal)
		{
			m_position = _position;
			m_normal = _normal;

#ifdef _DEBUG
			if (std::abs(glm::length(m_normal) - 1.0) > EPS_DEFAULT)
			{
				std::cout << "Plane::normal not normalized" << std::endl;
			}
#endif
		}

		// distance point to plane
		double distancePointPlane(const glm::dvec3& point) const
		{
#ifdef _DEBUG
			double length = glm::length(m_normal);
			if (std::abs(length - 1.0) > EPS_M8)
			{
				std::cout << " not normalized";
			}

			if (false)
			{
				glm::dvec3 normal(1, 0, 0);
				glm::dvec3 position(1, 0, 0);
				glm::dvec3 point(1, 5, 10);
				double D = dot(normal, (position));
				double dist = dot(normal, point - position);


				glm::dvec3 normal2(1, 0, 0);
				glm::dvec3 position2(0, 5, 8);
				glm::dvec3 point2(1, 0, 0);
				double dist2 = dot(normal2, point2 - position2);

				glm::dvec3 normal3(1, 0, 0);
				glm::dvec3 position3(1, 2, 3);
				glm::dvec3 point3(0, 9, 7);
				double dist3 = dot(normal3, point3 - position3);

				double a = normal.x;
				double b = normal.y;
				double c = normal.z;
				double d = D;
				double x1 = point.x;
				double y1 = point.y;
				double z1 = point.z;
				double Distance = std::abs(a * x1 + b * y1 + c * z1 + d) / (sqrt(a * a + b * b + c * c));
				std::cout << Distance;
			}

#endif

			return std::abs(dot(m_normal, point - m_position));
		}

		bool isEqualToPlane(const Plane& other, bool allowOppositeNormals, double epsDistance, double epsAngle ) const
		{
			double distanceTriangleProcessedPlane = other.distancePointPlane(m_position);
			if (std::abs(distanceTriangleProcessedPlane) < epsDistance)
			{
				double angle = glm::angle(m_normal, other.m_normal);
				if (std::abs(angle) < epsAngle)
				{
					return true;
				}

				if (allowOppositeNormals)
				{
					if (std::abs(angle - M_PI) < epsAngle)
					{
						return true;
					}
				}
			}
			return false;
		}

	protected:
		glm::dvec3 m_position{ 0.0, 0.0, 0.0 };
		glm::dvec3 m_normal{ 0.0, 0.0, 1.0 };
	};

	static double distancePointPlane(const vec3& point, const vec3& planeNormal, const vec3& planePosition)
	{
#ifdef _DEBUG
		double length2 = planeNormal.length2();
		if (std::abs(length2 - 1.0) > 0.1)
		{
			std::cout << "plane normal not normalized";
		}
#endif
		return dot(planeNormal, point - planePosition);
	}

	static bool intersectRayPlane(glm::dvec3 const& orig, glm::dvec3 const& dir, glm::dvec3 const& planeOrig, glm::dvec3 const& planeNormal, double& intersectionDistance)
	{
		double d = glm::dot(dir, planeNormal);
		double Epsilon = std::numeric_limits<double>::epsilon();

		if (glm::abs(d) > Epsilon)  // if dir and planeNormal are not perpendicular
		{
			double const tmp_intersectionDistance = glm::dot(planeOrig - orig, planeNormal) / d;
			//if( tmp_intersectionDistance > 0.0 ) { // allow only intersections
			intersectionDistance = tmp_intersectionDistance;
			return true;
			//}
		}

		return false;
	}

	/** polygon operations */
	inline vec3 computePolygonCentroid(const std::vector<vec3>& polygon)
	{
		vec3 polygon_centroid(carve::geom::VECTOR(0, 0, 0));
		for (std::vector<vec3>::const_iterator it = polygon.begin(); it != polygon.end(); ++it)
		{
			const vec3& vertex_current = (*it);
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)(polygon.size());
		return polygon_centroid;
	}
	inline vec2 computePolygonCentroid(const std::vector<vec2>& polygon)
	{
		vec2 polygon_centroid(carve::geom::VECTOR(0, 0));
		for (std::vector<vec2>::const_iterator it = polygon.begin(); it != polygon.end(); ++it)
		{
			const vec2& vertex_current = (*it);
			polygon_centroid += vertex_current;
		}
		polygon_centroid /= (double)(polygon.size());
		return polygon_centroid;
	}

	static carve::geom::vector<3> computeNormal(const carve::geom::vector<3>& a, const carve::geom::vector<3>& b, const carve::geom::vector<3>& c, double eps )
	{
		carve::geom::vector<3> AB(b - a);
		carve::geom::vector<3> AC(c - a);
		carve::geom::vector<3> normal = cross(AB, AC);

		double length = normal.length();
		if (length <= eps)
		{
			return normal;
		}

		normal /= length;
		return normal;
	}

	static glm::dvec3 computeNormal(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, double eps)
	{
		glm::dvec3 AB(b - a);
		glm::dvec3 AC(c - a);
		glm::dvec3 normal = glm::cross(AB, AC);

		double length = glm::length(normal);
		if (length <= eps)
		{
			return normal;
		}

		normal /= length;
		return normal;
	}

	inline vec3 computePolygonNormal(const std::vector<vec3>& polygon, double eps)
	{
		vec3 polygon_normal(carve::geom::VECTOR(0, 0, 0));
		if (polygon.size() < 2)
		{
			return polygon_normal;
		}
		else if (polygon.size() == 3)
		{
			return computeNormal(polygon[0], polygon[1], polygon[2], eps);
		}

		bool last_loop = false;
		for (std::vector<vec3>::const_iterator it = polygon.begin();; )
		{
			const vec3& vertex_current = (*it);
			++it;
			if (it == polygon.end())
			{
				it = polygon.begin();
				last_loop = true;
			}
			const vec3& vertex_next = (*it);
			polygon_normal[0] += (vertex_current.y - vertex_next.y) * (vertex_current.z + vertex_next.z);
			polygon_normal[1] += (vertex_current.z - vertex_next.z) * (vertex_current.x + vertex_next.x);
			polygon_normal[2] += (vertex_current.x - vertex_next.x) * (vertex_current.y + vertex_next.y);
			if (last_loop)
			{
				break;
			}
		}
		polygon_normal.normalize();

		if (polygon_normal.length2() < 0.1)
		{
			// generate random triangles, compute normal vector, then interpolate polygon normal vector with area and edge length as weight factor
			std::vector<std::pair<double, vec3> > vectorOfWeightsAndNormals;
			double sumOfWeights = 0;
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution0(0, polygon.size() - 1);
			std::uniform_int_distribution<int> distribution1(0, polygon.size() - 1);
			std::uniform_int_distribution<int> distribution2(0, polygon.size() - 1);
			for (size_t ii = 0; ii < polygon.size(); ++ii)
			{
				int idx0 = distribution0(generator);
				int idx1 = distribution0(generator);
				int idx2 = distribution0(generator);

				if (idx0 == idx1 || idx0 == idx2 || idx1 == idx2)
				{
					for (size_t jj = 0; jj < polygon.size() * 10; ++jj)
					{
						std::default_random_engine generator1;
						std::uniform_int_distribution<int> distribution0_1(0, polygon.size() - 1);

						size_t idx0 = distribution0_1(generator1);
						size_t idx1 = distribution0_1(generator1);
						size_t idx2 = distribution0_1(generator1);

						if (idx0 != idx1 && idx0 != idx2 && idx1 != idx2)
						{
							break;
						}
					}
				}


				// ensure order: idx0 < idx1 < idx2
				if (idx1 < idx0)
				{
					std::swap(idx0, idx1);
				}
				if (idx2 < idx0)
				{
					std::swap(idx0, idx2);
				}
				if (idx2 < idx1)
				{
					std::swap(idx1, idx2);
				}

				if (idx0 > idx1 || idx2 < idx1 || idx2 < idx0)
				{
					std::cout << "order failed";
				}

				const vec3& a = polygon[idx0];
				const vec3& b = polygon[idx1];
				const vec3& c = polygon[idx2];
				carve::geom::vector<3> AB(b - a);
				carve::geom::vector<3> AC(c - a);
				carve::geom::vector<3> triangleNormal = cross(AB, AC);

				double length = triangleNormal.length();
				if (length > eps)
				{
					triangleNormal /= length;
					double area = 0.5 * length;
					double edgeLength = AB.length() + AC.length() + (b - c).length();
					double weight = area + edgeLength;
					sumOfWeights += weight;
					vectorOfWeightsAndNormals.push_back({ weight, triangleNormal });
				}
			}

			polygon_normal = carve::geom::VECTOR(0, 0, 0);
			for (size_t ii = 0; ii < vectorOfWeightsAndNormals.size(); ++ii)
			{
				auto weightAndNormal = vectorOfWeightsAndNormals[ii];
				double weight = weightAndNormal.first;
				vec3& normal = weightAndNormal.second;
				polygon_normal += normal * (weight / sumOfWeights);
			}
		}
#ifdef _DEBUG
		if (polygon_normal.length2() < 0.1)
		{
			//glm::vec4 color(0.1, 0.3, 0.3, 1.0);
			//GeomDebugDump::dumpPolyline(polygon, color, 1.0, true);
		}
#endif
		return polygon_normal;
	}

	inline vec3 computePolygonNormal(const std::vector<carve::mesh::Vertex<3>* >& polygon)
	{
		vec3 polygon_normal(carve::geom::VECTOR(0, 0, 0));
		if (polygon.size() < 2)
		{
			return polygon_normal;
		}
		bool last_loop = false;
		for (std::vector<carve::mesh::Vertex<3>* >::const_iterator it = polygon.begin();; )
		{
			const carve::geom::vector<3>& vertex_current = (*it)->v;
			++it;
			if (it == polygon.end())
			{
				it = polygon.begin();
				last_loop = true;
			}
			const vec3& vertex_next = (*it)->v;
			polygon_normal[0] += (vertex_current.y - vertex_next.y) * (vertex_current.z + vertex_next.z);
			polygon_normal[1] += (vertex_current.z - vertex_next.z) * (vertex_current.x + vertex_next.x);
			polygon_normal[2] += (vertex_current.x - vertex_next.x) * (vertex_current.y + vertex_next.y);
			if (last_loop)
			{
				break;
			}
		}
		polygon_normal.normalize();
		return polygon_normal;
	}

	inline vec3 computePolygon2DNormal(const std::vector<vec2>& polygon)
	{
		const int num_points = polygon.size();
		vec3 polygon_normal(carve::geom::VECTOR(0, 0, 0));
		for (int k = 0; k < num_points; ++k)
		{
			const vec2& vertex_current = polygon[k];
			const vec2& vertex_next = polygon[(k + 1) % num_points];
			polygon_normal[2] += (vertex_current.x - vertex_next.x) * (vertex_current.y + vertex_next.y);
		}
		polygon_normal.normalize();
		return polygon_normal;
	}

	static glm::dvec3 computePolygon2DNormal(const std::vector<std::array<double, 2> >& polygon, double eps)
	{
		const size_t num_points = polygon.size();
		glm::dvec3 polygon_normal(glm::dvec3(0, 0, 0));
		for (int k = 0; k < num_points; ++k)
		{
			const std::array<double, 2>& vertex_current = polygon[k];
			const std::array<double, 2>& vertex_next = polygon[(k + 1) % num_points];
			polygon_normal[2] += (vertex_current[0] - vertex_next[0]) * (vertex_current[1] + vertex_next[1]);
		}
		safeNormalize(polygon_normal, eps);
		return polygon_normal;
	}

	inline bool isQuadConvex(const std::array<double, 2>& _a, const std::array<double, 2>& _b, const std::array<double, 2>& _c, const std::array<double, 2>& _d)
	{
		carve::geom::vector<2> a = carve::geom::VECTOR(_a[0], _a[1]);
		carve::geom::vector<2> b = carve::geom::VECTOR(_b[0], _b[1]);
		carve::geom::vector<2> c = carve::geom::VECTOR(_c[0], _c[1]);
		carve::geom::vector<2> d = carve::geom::VECTOR(_d[0], _d[1]);
		double s_1 = carve::geom2d::orient2d(a, c, b);
		double s_2 = carve::geom2d::orient2d(a, c, d);
		if ((s_1 < 0.0 && s_2 < 0.0) || (s_1 > 0.0 && s_2 > 0.0))
		{
			return false;
		}

		s_1 = carve::geom2d::orient2d(b, d, a);
		s_2 = carve::geom2d::orient2d(b, d, c);
		if ((s_1 < 0.0 && s_2 < 0.0) || (s_1 > 0.0 && s_2 > 0.0))
		{
			return false;
		}

		return true;
	}

	//\brief closePolygon: if last point in polygon is not equal to first point, add first point to close polygon
	static void closePolygon(std::vector<vec2>& polygon2d, double eps)
	{
		if (polygon2d.size() > 1)
		{
			auto& ptFirst = polygon2d.front();
			auto& ptLast = polygon2d.back();
			double dx = ptLast.x - ptFirst.x;
			double dy = ptLast.y - ptFirst.y;

			if (dx * dx + dy * dy > eps)
			{
				polygon2d.push_back(ptFirst);
			}
		}
	}

	//\brief closePolygon: if last point in polygon is not equal to first point, add first point to close polygon
	static void closePolygon(std::vector<vec3>& polygon3d, double eps)
	{
		if (polygon3d.size() > 1)
		{
			auto& ptFirst = polygon3d.front();
			auto& ptLast = polygon3d.back();
			double dx = ptLast.x - ptFirst.x;
			double dy = ptLast.y - ptFirst.y;
			double dz = ptLast.z - ptFirst.z;

			if (dx * dx + dy * dy + dz * dz > eps)
			{
				polygon3d.push_back(ptFirst);
			}
		}
	}

	//\brief unClosePolygon: if last point in polygon is equal to first point, remove last point
	static void unClosePolygon(std::vector<vec2>& polygon2d, double eps)
	{
		if (polygon2d.size() > 1)
		{
			auto& ptFirst = polygon2d.front();
			auto& ptLast = polygon2d.back();
			double dx = ptLast.x - ptFirst.x;
			double dy = ptLast.y - ptFirst.y;

			if (dx * dx + dy * dy < eps)
			{
				polygon2d.pop_back();
			}
		}
	}

	//\brief unClosePolygon: if last point in polygon is equal to first point, remove last point
	static void unClosePolygon(std::vector<vec3>& polygon3d, double eps)
	{
		for( size_t ii = 0; ii < polygon3d.size(); ++ii)
		{
			if (polygon3d.size() > 1)
			{
				vec3& ptFirst = polygon3d.front();
				vec3& ptLast = polygon3d.back();
				double dx = ptLast.x - ptFirst.x;
				
				if (std::abs(dx) < eps)
				{
					double dy = ptLast.y - ptFirst.y;
					if (std::abs(dy) < eps)
					{
						//if (dx * dx + dy * dy + dz * dz < eps)
						double dz = ptLast.z - ptFirst.z;
						if (std::abs(dz) < eps)
						{
							polygon3d.pop_back();
							continue;
						}
					}
				}
			}
			break;
		}
	}

	static void polygonBbox(const std::vector<vec3>& loop, carve::geom::aabb<3>& bbox)
	{
		bbox.fit(loop.begin(), loop.end());
	}


	static void polygonBbox(const std::vector<std::vector<vec3 > >& poly, vec3& min, vec3& max)
	{
		bool first = true;
		for (const auto& loop : poly)
		{
			for (const auto& point : loop)
			{
				if (first)
				{
					min = carve::geom::VECTOR(point.x, point.y, point.z);
					max = carve::geom::VECTOR(point.x, point.y, point.z);
					first = false;
				}
				else
				{
					min = carve::geom::VECTOR(std::min(min.x, point.x), std::min(min.y, point.y), std::min(min.z, point.z));
					max = carve::geom::VECTOR(std::max(max.x, point.x), std::max(max.y, point.y), std::min(min.z, point.z));
				}
			}
		}
	}

	static void polygonBbox(std::vector<std::vector<std::array<double, 2> > >& poly, vec2& min, vec2& max)
	{
		bool first = true;
		for (const auto& loop : poly)
		{
			for (const auto& point : loop)
			{
				if (first)
				{
					min = carve::geom::VECTOR(point[0], point[1]);
					max = carve::geom::VECTOR(point[0], point[1]);
					first = false;
				}
				else
				{
					min = carve::geom::VECTOR(std::min(min.x, point[0]), std::min(min.y, point[1]));
					max = carve::geom::VECTOR(std::max(max.x, point[0]), std::max(max.y, point[1]));
				}
			}
		}
	}

	static void polygonBbox(std::vector<std::vector<std::array<double, 2> > >& poly, glm::dvec2& min, glm::dvec2& max)
	{
		bool first = true;
		for (const auto& loop : poly)
		{
			for (const auto& point : loop)
			{
				if (first)
				{
					min = { point[0], point[1] };
					max = { point[0], point[1] };
					first = false;
				}
				else
				{
					min = { std::min(min.x, point[0]), std::min(min.y, point[1]) };
					max = { std::max(max.x, point[0]), std::max(max.y, point[1]) };
				}
			}
		}
	}

	static std::vector<glm::dvec2> vecArray2poly2(std::vector<std::array<double, 2> >& polyIn)
	{
		std::vector<glm::dvec2> result;
		for (const auto& point : polyIn)
		{
			result.push_back({ point[0], point[1] });
		}
		return result;
	}

	static void poly2VecToArray2(const std::vector<carve::geom::vector<2> >& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& point : polyIn)
		{
			polyOut.push_back({ point[0], point[1] });
		}
	}

	static void poly2VecToArray2(const std::vector<glm::dvec2>& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& point : polyIn)
		{
			polyOut.push_back({ point[0], point[1] });
		}
	}

	static void poly2VecToFlatArray2(const std::vector<std::vector<vec2> >& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back({ point[0], point[1] });
			}
		}
	}

	static void poly2VecToFlatArray2(const std::vector<std::vector<std::array<double, 2> > >& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back({ point[0], point[1] });
			}
		}
	}

	static void poly2VecToFlatVec(const std::vector<std::vector<glm::dvec2> >& polyIn, std::vector<glm::dvec2>& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back({ point[0], point[1] });
			}
		}
	}


	static void polygons2flatVec(const std::vector<vec2>& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& point : polyIn)
		{
			polyOut.push_back({ point[0], point[1] });
		}
	}

	static void polygons2flatVec(const std::vector<std::vector<vec2> >& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back({ point[0], point[1] });
			}
		}
	}

	static void polygons2flatVec(const std::vector<std::vector<std::array<double, 2> > >& polyIn, std::vector<std::array<double, 2> >& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back({ point[0], point[1] });
			}
		}
	}

	static void polygons2flatVec(const std::vector<std::vector<vec2> >& polyIn, std::vector<vec2>& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back(carve::geom::VECTOR(point.x, point.y));
			}
		}
	}

	static void polygons2flatVec(const std::vector<std::vector<vec3> >& polyIn, std::vector<vec3>& polyOut)
	{
		for (const auto& vec : polyIn)
		{
			for (const auto& point : vec)
			{
				polyOut.push_back(carve::geom::VECTOR(point.x, point.y, point.z));
			}
		}
	}

	static double signedArea(const std::vector<std::array<double, 2> >& points)
	{
		size_t l = points.size();
		double A = 0.0;
		if (l > 2)
		{
			for (size_t i = 0; i < l - 1; i++)
			{
				A += (points[i + 1][1] + points[i][1]) * (points[i + 1][0] - points[i][0]);
			}
		}
		A += (points[0][1] + points[l - 1][1]) * (points[0][0] - points[l - 1][0]);

		return A / 2.0;
	}

	static double signedArea(const std::vector<carve::geom::vector<2> >& points)
	{
		size_t l = points.size();
		double A = 0.0;
		if (l > 2)
		{
			for (size_t i = 0; i < l - 1; i++)
			{
				A += (points[i + 1].y + points[i].y) * (points[i + 1].x - points[i].x);
			}
		}
		A += (points[0].y + points[l - 1].y) * (points[0].x - points[l - 1].x);

		return A / 2.0;
	}

	static double computePolygonArea(const std::vector<vec3>& points, double eps)
	{
		if (points.size() == 3)
		{
			const carve::geom::vector<3>& v1 = points[0];
			const carve::geom::vector<3>& v2 = points[1];
			const carve::geom::vector<3>& v3 = points[2];
			carve::geom::vector<3> side1 = v2 - v1;
			carve::geom::vector<3> side2 = v3 - v2;
			carve::geom::vector<3> c = cross(side1, side2);
			double len_square = c.length2();
			if (len_square > 1.e-50)  // #define DBL_MIN          2.2250738585072014e-308 // min positive value
			{
				double area = sqrt(len_square) * 0.5;
				return area;
			}
#if defined(_DEBUG) || defined(_DEBUG_RELEASE)
			else
			{
				if (len_square > 1.e-150)
				{
					double area = sqrt(len_square) * 0.5;
					std::cout << "computePolygonArea: check division by " << len_square << std::endl;
				}
			}
#endif
		}
		else
		{
			// continue for n points
			size_t n = points.size();
			carve::geom::vector<3> normal;
			carve::geom::vector<3> a;
			carve::geom::vector<3> b = points[n - 2];
			carve::geom::vector<3> c = points[n - 1];
			carve::geom::vector<3> s;

			for (int i = 0; i < n; ++i)
			{
				a = b;
				b = c;
				c = points[i];

				normal.x += b.y * (c.z - a.z);
				normal.y += b.z * (c.x - a.x);
				normal.z += b.x * (c.y - a.y);

				s += c;
			}

			double len_square = normal.length2();// glm::length(normal);
			if (len_square < 1.e-50)
			{
				return 0.0;
			}

			double area = sqrt(len_square) * 0.5;
			return std::abs(area);
		}

		return 0.0;
	}

	inline bool checkOpenPolygonConvexity(const std::vector<vec2>& polygon, double eps)
	{
		if (polygon.size() < 3)
		{
			return true;
		}
		const int num_points = polygon.size();
		double zcrossproduct_previous = 0;
		for (int k = 0; k < num_points - 2; ++k)
		{
			const vec2& vertex_current = polygon[k];
			const vec2& vertex_next1 = polygon[k + 1];
			const vec2& vertex_next2 = polygon[k + 2];

			double dx1 = vertex_next1.x - vertex_current.x;
			double dy1 = vertex_next1.y - vertex_current.y;

			double dx2 = vertex_next2.x - vertex_next1.x;
			double dy2 = vertex_next2.y - vertex_next1.y;

			double zcrossproduct = dx1 * dy2 - dy1 * dx2;

			if (k > 0)
			{
				if (std::abs(zcrossproduct) > eps)
				{
					if (std::abs(zcrossproduct_previous) > eps)
					{
						if (zcrossproduct * zcrossproduct_previous < 0)
						{
							// there is a change in direction -> not convex
							return false;
						}
					}
				}
			}
			zcrossproduct_previous = zcrossproduct;
		}
		return true;
	}

	inline void appendPointsToCurve(const std::vector<vec2>& points_vec, std::vector<vec3>& target_vec, double eps)
	{
		bool omit_first = false;
		if (target_vec.size() > 0)
		{
			const vec3& last_point = target_vec.back();
			const vec2& first_point_current_segment = points_vec.front();
			if (std::abs(last_point.x - first_point_current_segment.x) < eps)
			{
				if (std::abs(last_point.y - first_point_current_segment.y) < eps)
				{
					omit_first = true;
				}
			}
		}

		if (omit_first)
		{
			for (size_t i = 1; i < points_vec.size(); ++i)
			{
				const vec2& pt = points_vec[i];
				target_vec.push_back(carve::geom::VECTOR(pt.x, pt.y, 0));
			}
		}
		else
		{
			for (size_t i = 0; i < points_vec.size(); ++i)
			{
				const vec2& pt = points_vec[i];
				target_vec.push_back(carve::geom::VECTOR(pt.x, pt.y, 0));
			}
		}
		// TODO: handle all segments separately: std::vector<std::vector<vec3> >& target_vec
	}
	inline void appendPointsToCurve(const std::vector<vec3>& points_vec_src, std::vector<vec3>& target_vec, double epsilonMergePoints)
	{
		if (points_vec_src.size() == 0)
		{
			return;
		}

		// sometimes, sense agreement is not given correctly. try to correct sense of segment if necessary
		std::vector<vec3> points_vec(points_vec_src);
		if (target_vec.size() > 0 && points_vec.size() > 1)
		{
			vec3 first_target_point = target_vec.front();
			vec3 last_target_point = target_vec.back();

			vec3 first_segment_point = points_vec.front();
			vec3 last_segment_point = points_vec.back();
			double d0 = (last_target_point - first_segment_point).length2();
			if (d0 < epsilonMergePoints * epsilonMergePoints)
			{
				// segment order is as expected, nothing to do
			}
			else
			{
				double d1 = (last_target_point - last_segment_point).length2();
				if (d1 < epsilonMergePoints * epsilonMergePoints)
				{
					// current segment seems to be in wrong order
					std::reverse(points_vec.begin(), points_vec.end());
				}
				else
				{
					// maybe the current segment fits to the beginning of the target vector
					double d2 = (first_target_point - first_segment_point).length2();
					if (d2 < epsilonMergePoints * epsilonMergePoints)
					{
						std::reverse(target_vec.begin(), target_vec.end());
					}
					else
					{
						double d3 = (first_target_point - last_segment_point).length2();
						if (d3 < epsilonMergePoints * epsilonMergePoints)
						{
							std::reverse(target_vec.begin(), target_vec.end());
							std::reverse(points_vec.begin(), points_vec.end());
						}
						else
						{
							if (d1 < d0 && d1 < d2 && d1 < d3)
							{
								std::reverse(points_vec.begin(), points_vec.end());
							}
							else if (d2 < d0 && d2 < d1 && d2 < d3)
							{
								std::reverse(target_vec.begin(), target_vec.end());
							}
							else if (d3 < d0 && d3 < d1 && d3 < d2)
							{
								std::reverse(target_vec.begin(), target_vec.end());
								std::reverse(points_vec.begin(), points_vec.end());
							}
						}
					}
				}
			}
		}

		bool omit_first = false;
		if (target_vec.size() > 0)
		{
			vec3 last_point = target_vec.back();
			vec3 first_point_current_segment = points_vec.front();
			if ((last_point - first_point_current_segment).length() < 0.000001)
			{
				omit_first = true;
			}
		}

		if (omit_first)
		{
			target_vec.insert(target_vec.end(), points_vec.begin() + 1, points_vec.end());
		}
		else
		{
			target_vec.insert(target_vec.end(), points_vec.begin(), points_vec.end());
		}
		// TODO: handle all segments separately: std::vector<std::vector<vec3> >& target_vec
	}
	inline void addArcWithEndPoint(std::vector<vec2>& coords, double radius, double start_angle, double opening_angle, double x_center, double y_center, int num_segments)
	{
		if (num_segments < 3)
		{
			num_segments = 3;
		}

		if (num_segments > 100)
		{
			num_segments = 100;
		}

		double angle = start_angle;
		double angle_delta = opening_angle / (double)(num_segments - 1);
		for (int i = 0; i < num_segments; ++i)
		{
			coords.push_back(carve::geom::VECTOR(radius * cos(angle) + x_center, radius * sin(angle) + y_center));
			angle += angle_delta;
		}
	}

	inline bool LineToLineIntersectionHelper(vec2& v1, vec2& v2, vec2& v3, vec2& v4, double& r, double& s)
	{
		// check if lines are parallel
		const vec2 vertex1to2 = v2 - v1;
		const vec2 vertex3to4 = v4 - v3;
		if (vertex1to2.y / vertex1to2.x != vertex3to4.y / vertex3to4.x)
		{
			const double d = vertex1to2.x * vertex3to4.y - vertex1to2.y * vertex3to4.x;
			if (d != 0)
			{
				const vec2 vertex3to1 = v1 - v3;
				r = (vertex3to1.y * vertex3to4.x - vertex3to1.x * vertex3to4.y) / d;
				s = (vertex3to1.y * vertex1to2.x - vertex3to1.x * vertex1to2.y) / d;
				return true;
			}
		}
		return false;

	}
	inline bool LineToLineIntersectionHelper(vec3& v1, vec3& v2, vec3& v3, vec3& v4, double& r, double& s)
	{
		// check if lines are parallel
		const vec3 vertex1to2 = v2 - v1;
		const vec3 vertex3to4 = v4 - v3;
		if (vertex1to2.y / vertex1to2.x != vertex3to4.y / vertex3to4.x)
		{
			const double d = vertex1to2.x * vertex3to4.y - vertex1to2.y * vertex3to4.x;
			if (d != 0)
			{
				const vec3 vertex3to1 = v1 - v3;
				r = (vertex3to1.y * vertex3to4.x - vertex3to1.x * vertex3to4.y) / d;
				s = (vertex3to1.y * vertex1to2.x - vertex3to1.x * vertex1to2.y) / d;
				return true;
			}
		}
		return false;

	}

	inline bool LineSegmentToLineIntersection(vec2& v1, vec2& v2, vec2& v3, vec2& v4, std::vector<vec2>& result)
	{
		double r, s;
		if (LineToLineIntersectionHelper(v1, v2, v3, v4, r, s))
		{
			if (r >= 0 && r <= 1)
			{
				result.push_back(v1 + (v2 - v1) * r);
				return true;
			}
		}
		return false;
	}
	inline bool LineSegmentToLineSegmentIntersection(vec2& v1, vec2& v2, vec2& v3, vec2& v4, std::vector<vec2>& result)
	{
		double r, s;
		if (LineToLineIntersectionHelper(v1, v2, v3, v4, r, s))
		{
			if (r >= 0 && r <= 1)
			{
				if (s >= 0 && s <= 1)
				{
					result.push_back(v1 + (v2 - v1) * r);
					return true;
				}
			}
		}
		return false;
	}
	inline bool LineSegmentToLineSegmentIntersection(vec3& v1, vec3& v2, vec3& v3, vec3& v4, std::vector<vec3>& result)
	{
		double r, s;
		if (LineToLineIntersectionHelper(v1, v2, v3, v4, r, s))
		{
			if (r >= 0 && r <= 1)
			{
				if (s >= 0 && s <= 1)
				{
					result.push_back(v1 + (v2 - v1) * r);
					return true;
				}
			}
		}
		return false;
	}

	inline void closestPointOnLine(const vec3& point, const vec3& line_origin, const vec3& line_direction, vec3& closest)
	{
		const double denom = point.x * line_direction.x + point.y * line_direction.y + point.z * line_direction.z - line_direction.x * line_origin.x - line_direction.y * line_origin.y - line_direction.z * line_origin.z;
		const double numer = line_direction.x * line_direction.x + line_direction.y * line_direction.y + line_direction.z * line_direction.z;
		if (numer == 0.0)
		{
			throw BuildingException("Line is degenerated: the line's direction vector is a null vector!", __FUNC__);
		}
		const double lambda = denom / numer;
		closest = carve::geom::VECTOR(line_origin.x + lambda * line_direction.x, line_origin.y + lambda * line_direction.y, line_origin.z + lambda * line_direction.z);
	}
	inline void closestPointOnLine(const vec2& point, const vec2& line_origin, const vec2& line_direction, vec2& closest)
	{
		const double denom = point.x * line_direction.x + point.y * line_direction.y + -line_direction.x * line_origin.x - line_direction.y * line_origin.y;
		const double numer = line_direction.x * line_direction.x + line_direction.y * line_direction.y;
		if (numer == 0.0)
		{
			throw BuildingException("Line is degenerated: the line's direction vector is a null vector!", __FUNC__);
		}
		const double lambda = denom / numer;
		closest = carve::geom::VECTOR(line_origin.x + lambda * line_direction.x, line_origin.y + lambda * line_direction.y);
	}
	inline double distancePoint2Line(const vec3& point, const vec3& line_p0, const vec3& line_p1)
	{
		// d = |(point - line_p0)x(point - line_p1)| / |line_p1 - line_p0|
		return carve::geom::cross((point - line_p0), (point - line_p1)).length() / (line_p1 - line_p0).length();
	}
	inline double distancePoint2LineUnitDirection(const vec3& point, const vec3& line_pt, const vec3& line_direction_normalized)
	{
		// d = |line_direction_normalized x ( point - line_pt )|
		return carve::geom::cross((point - line_pt), (line_direction_normalized)).length();
	}
	template<unsigned int ndim>
	double Point2LineSegmentDistance2(const carve::geom::linesegment<ndim>& l, const carve::geom::vector<ndim>& v, carve::geom::vector<ndim>& closest_point)
	{
		carve::geom::vector<ndim> linesegment_delta = l.v2 - l.v1;
		double t = dot(v - l.v1, linesegment_delta) / dot(linesegment_delta, linesegment_delta);
		if (t <= 0.0)
		{
			t = 0.0;
		}
		if (t >= 1.0)
		{
			t = 1.0;
		}
		closest_point = linesegment_delta * t + l.v1;
		return (v - closest_point).length2();
	}

	static bool isPointOnLineSegment(const carve::geom::vector<3>& linesegment_start, const carve::geom::vector<3>& linesegment_delta, const carve::geom::vector<3>& point, double eps)
	{
		double t = dot(point - linesegment_start, linesegment_delta) / dot(linesegment_delta, linesegment_delta);

		if (t < eps || t > 1.0 - eps)
		{
			return false;
		}

		carve::geom::vector<3> closest_point = linesegment_delta * t + linesegment_start;
		double distance2 = (point - closest_point).length2();

		if (distance2 < eps * eps)
		{
			return true;
		}
		return false;
	}

	static bool isPointOnLineSegment(const glm::dvec3& linesegment_start, const glm::dvec3& linesegment_delta, const glm::dvec3& point, double eps)
	{
		double t = dot(point - linesegment_start, linesegment_delta) / dot(linesegment_delta, linesegment_delta);

		if (t < eps || t > 1.0 - eps)
		{
			return false;
		}

		glm::dvec3 closest_point = linesegment_delta * t + linesegment_start;
		double distance2 = glm::length2(point - closest_point);

		if (distance2 < eps * eps)
		{
			return true;
		}
		return false;
	}

	static bool isPointOnLineSegment(const carve::geom::vector<3>& linesegment_start, const carve::geom::vector<3>& linesegment_delta, double dotLineSegDelta, const carve::geom::vector<3>& point, double& t, double eps)
	{
		t = dot(point - linesegment_start, linesegment_delta) / dotLineSegDelta;

		if (t < eps || t > 1.0 - eps)
		{
			return false;
		}

		carve::geom::vector<3> closest_point = linesegment_delta * t + linesegment_start;
		double distance2 = (point - closest_point).length2();

		if (distance2 < eps * eps)
		{
			return true;
		}
		return false;
	}

	inline bool isMatrixIdentity(const carve::math::Matrix& mat, double eps = 0.00001)
	{
		if (std::abs(mat._11 - 1.0) > eps)  return false;
		if (std::abs(mat._22 - 1.0) > eps)  return false;
		if (std::abs(mat._33 - 1.0) > eps)  return false;
		if (std::abs(mat._44 - 1.0) > eps)  return false;

		if (std::abs(mat._12) > eps)  return false;
		if (std::abs(mat._13) > eps)  return false;
		if (std::abs(mat._14) > eps)  return false;

		if (std::abs(mat._21) > eps)  return false;
		if (std::abs(mat._23) > eps)  return false;
		if (std::abs(mat._24) > eps)  return false;

		if (std::abs(mat._31) > eps)  return false;
		if (std::abs(mat._32) > eps)  return false;
		if (std::abs(mat._34) > eps)  return false;

		if (std::abs(mat._41) > eps)  return false;
		if (std::abs(mat._42) > eps)  return false;
		if (std::abs(mat._43) > eps)  return false;
		return true;
	}
	inline bool isMatrixEqual(const carve::math::Matrix& mat1, const carve::math::Matrix& mat2, double maxDelta = 0.00001)
	{
		if (std::abs(mat1._11 - mat2._11) > maxDelta)  return false;
		if (std::abs(mat1._12 - mat2._12) > maxDelta)  return false;
		if (std::abs(mat1._13 - mat2._13) > maxDelta)  return false;
		if (std::abs(mat1._14 - mat2._14) > maxDelta)  return false;

		if (std::abs(mat1._21 - mat2._21) > maxDelta)  return false;
		if (std::abs(mat1._22 - mat2._22) > maxDelta)  return false;
		if (std::abs(mat1._23 - mat2._23) > maxDelta)  return false;
		if (std::abs(mat1._24 - mat2._24) > maxDelta)  return false;

		if (std::abs(mat1._31 - mat2._31) > maxDelta)  return false;
		if (std::abs(mat1._32 - mat2._32) > maxDelta)  return false;
		if (std::abs(mat1._33 - mat2._33) > maxDelta)  return false;
		if (std::abs(mat1._34 - mat2._34) > maxDelta)  return false;

		if (std::abs(mat1._41 - mat2._41) > maxDelta)  return false;
		if (std::abs(mat1._42 - mat2._42) > maxDelta)  return false;
		if (std::abs(mat1._43 - mat2._43) > maxDelta)  return false;
		if (std::abs(mat1._44 - mat2._44) > maxDelta)  return false;
		return true;
	}
	inline void makeLookAt(const vec3& eye, const vec3& center, const vec3& up, carve::math::Matrix& resulting_matrix)
	{
		vec3 zaxis = (center - eye).normalize();
		vec3 xaxis = cross(up, zaxis).normalize();
		vec3 yaxis = cross(zaxis, xaxis);

		resulting_matrix = carve::math::Matrix(
			xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			0, 0, 0, 1);
	}
	inline bool bisectingPlane(const vec3& v1, const vec3& v2, const vec3& v3, vec3& normal, double eps)
	{
		bool valid = false;
		vec3 v21 = v2 - v1;
		vec3 v32 = v3 - v2;
		double len21_square = v21.length2();
		double len32_square = v32.length2();

		if (len21_square <= eps * len32_square)
		{
			if (len32_square == 0.0)
			{
				// all three points lie ontop of one-another
				normal = carve::geom::VECTOR(0.0, 0.0, 0.0);
				valid = false;
			}
			else
			{
				// return a normalized copy of v32 as bisector
				len32_square = 1.0 / len32_square;
				normal = v32 * len32_square;
				normal.normalize();
				valid = true;
			}

		}
		else
		{
			valid = true;
			if (len32_square <= eps * len21_square)
			{
				// return v21 as bisector
				v21.normalize();
				normal = v21;
			}
			else
			{
				v21.normalize();
				v32.normalize();

				double dot_product = dot(v32, v21);
				double dot_product_abs = std::abs(dot_product);

				if (dot_product_abs > (1.0 + eps) || dot_product_abs < (1.0 - eps))
				{
					normal = (v32 + v21) * dot_product - v32 - v21;
					normal.normalize();
				}
				else
				{
					// dot == 1 or -1, points are colinear
					normal = -v21;
				}
			}
		}
		return valid;
	}
	inline void convertPlane2Matrix(const vec3& plane_normal, const vec3& plane_position,
		const vec3& local_z, carve::math::Matrix& resulting_matrix)
	{
		vec3 local_normal(plane_normal);
		local_normal.normalize();
		vec3 local_z_new(local_z);
		//local_z_new.normalize();

		vec3 local_y = cross(local_normal, local_z_new);
		local_y.normalize();
		local_z_new = cross(local_y, local_normal);
		local_z_new.normalize();

		resulting_matrix = carve::math::Matrix(
			local_normal.x, local_y.x, local_z_new.x, plane_position.x,
			local_normal.y, local_y.y, local_z_new.y, plane_position.y,
			local_normal.z, local_y.z, local_z_new.z, plane_position.z,
			0, 0, 0, 1);

	}

	/** MeshSet and Polyhedron operations */
	inline void applyTransform(shared_ptr<carve::input::PolyhedronData>& poly_data, const carve::math::Matrix& matrix)
	{
		for (size_t ii = 0; ii < poly_data->points.size(); ++ii)
		{
			vec3& point = poly_data->points[ii];
			point = matrix * point;
		}
	}
	inline void applyTranslate(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const vec3& pos, double eps)
	{
		for (size_t i = 0; i < meshset->vertex_storage.size(); ++i)
		{
			vec3& point = meshset->vertex_storage[i].v;
			point = point + pos;
		}
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			meshset->meshes[i]->recalc(eps);
		}
	}
	inline void applyTransform(shared_ptr<carve::mesh::MeshSet<3> >& meshset, const carve::math::Matrix& matrix, double eps)
	{
		for (size_t i = 0; i < meshset->vertex_storage.size(); ++i)
		{
			vec3& point = meshset->vertex_storage[i].v;
			point = matrix * point;
		}
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			meshset->meshes[i]->recalc(eps);
		}
	}
	inline void applyTransform(carve::geom::aabb<3>& aabb, const carve::math::Matrix& matrix)
	{
		vec3& pos = aabb.pos;
		vec3& extent = aabb.extent;

		pos = matrix * pos;
		extent = matrix * extent;
	}

	/** matrix operations */
	inline bool computeInverse(const carve::math::Matrix& matrix_a, carve::math::Matrix& matrix_inv, const double eps = 0.01)
	{
		double inv[16], det;
		double m[16] = {
			matrix_a._11, matrix_a._12, matrix_a._13, matrix_a._14,
			matrix_a._21, matrix_a._22, matrix_a._23, matrix_a._24,
			matrix_a._31, matrix_a._32, matrix_a._33, matrix_a._34,
			matrix_a._41, matrix_a._42, matrix_a._43, matrix_a._44 };

		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (std::abs(det) < eps)
		{
			return false;
		}

		for (size_t i = 0; i < 16; i++)
		{
			matrix_inv.v[i] = inv[i] / det;
		}

		return true;

	}
	inline bool checkMatricesIdentical(const carve::math::Matrix& A, const carve::math::Matrix& B, double tolerance = 0.000001)
	{
		for (size_t i = 0; i < 16; ++i)
		{
			double delta = A.v[i] - B.v[i];
			if (std::abs(delta) > tolerance) return false;
		}
		return true;
	}
	static void simplifyPolygon(std::vector<std::array<double, 2> >& polygon, bool mergeAlignedEdges, double epsMergePoints, double epsAlignedEdgesAngle)
	{
		if (polygon.size() > 2)
		{
			for (size_t iiRound = 0; iiRound < polygon.size(); ++iiRound)
			{
				bool removedPoint = false;
				for (size_t ii = 2; ii < polygon.size(); ++ii)
				{
					const std::array<double, 2>& p0 = polygon[ii - 2];
					const std::array<double, 2>& p1 = polygon[ii - 1];
					const std::array<double, 2>& p2 = polygon[ii - 0];

					// TODO: check if we need to normalize
					const double dx1 = p1[0] - p0[0];
					const double dx2 = p2[0] - p1[0];
					const double dy1 = p1[1] - p0[1];
					const double dy2 = p2[1] - p1[1];

					if (std::abs(dx1) < epsMergePoints && std::abs(dy1) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}
					if (std::abs(dx2) < epsMergePoints && std::abs(dy2) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}

					if (mergeAlignedEdges)
					{
						double scalar = dx1 * dx2 + dy1 * dy2;
						double checkAlignment = scalar * scalar - (dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2);
						if (std::abs(checkAlignment) < epsAlignedEdgesAngle)
						{
							polygon.erase(polygon.begin() + ii - 1);
							removedPoint = true;
							break;
						}
					}
				}

				if (!removedPoint)
				{
					break;
				}
			}
		}
	}

	static void simplifyPolygon(std::vector<vec2>& polygon, bool mergeAlignedEdges, double epsMergePoints, double epsAlignedEdgesAngle)
	{
		if (polygon.size() > 2)
		{
			for (size_t iiRound = 0; iiRound < polygon.size(); ++iiRound)
			{
				bool removedPoint = false;
				for (size_t ii = 0; ii < polygon.size(); ++ii)
				{
					const vec2& p0 = polygon[ii];
					const vec2& p1 = polygon[(ii + 1) % polygon.size()];
					const vec2& p2 = polygon[(ii + 2) % polygon.size()];
					vec2 p10 = p1 - p0;
					vec2 p12 = p1 - p2;

					if (std::abs(p10.x) < epsMergePoints && std::abs(p10.y) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}
					if (std::abs(p12.x) < epsMergePoints && std::abs(p12.y) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}

					if (mergeAlignedEdges)
					{
						p10.normalize();
						p12.normalize();
						double dotProduct = dot(p10, p12);
						double angle = std::acos(dotProduct);

						if (std::abs(angle) < epsAlignedEdgesAngle)
						{
							polygon.erase(polygon.begin() + ii - 1);
							removedPoint = true;
							break;
						}
					}
				}

				if (!removedPoint)
				{
					break;
				}
			}
		}
	}

	static void simplifyPolygon(std::vector<vec3>& polygon, double epsMergePoints, double epsAlignedEdgesAngle)
	{
		if (polygon.size() > 2)
		{
			for (size_t iiRound = 0; iiRound < polygon.size(); ++iiRound)
			{
				bool removedPoint = false;
				for (size_t ii = 2; ii < polygon.size(); ++ii)
				{
					const vec3& p0 = polygon[ii - 2];
					const vec3& p1 = polygon[ii - 1];
					const vec3& p2 = polygon[ii - 0];
					double dx1 = p1.x - p0.x;
					double dx2 = p2.x - p1.x;
					double dy1 = p1.y - p0.y;
					double dy2 = p2.y - p1.y;
					double dz1 = p1.z - p0.z;
					double dz2 = p2.z - p1.z;

					if (std::abs(dx1) < epsMergePoints && std::abs(dy1) < epsMergePoints && std::abs(dz1) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}

					if (std::abs(dx2) < epsMergePoints && std::abs(dy2) < epsMergePoints && std::abs(dz2) < epsMergePoints)
					{
						polygon.erase(polygon.begin() + ii - 1);
						removedPoint = true;
						break;
					}

					if (epsAlignedEdgesAngle > 0)
					{
						vec3 p1p0 = p1 - p0;
						vec3 p1p2 = p1 - p2;
						safeNormalize(p1p0, epsMergePoints);
						safeNormalize(p1p2, epsMergePoints);
						dx1 = p1p0.x;
						dx2 = p1p2.x;
						dy1 = p1p0.y;
						dy2 = p1p2.y;
						dz1 = p1p0.z;
						dz2 = p1p2.z;

						double scalar = dx1 * dx2 + dy1 * dy2 + dz1 * dz2;
						double check = scalar * scalar - (dx1 * dx1 + dy1 * dy1 + dz1 * dz1) * (dx2 * dx2 + dy2 * dy2 + dz2 * dz2);

						if (std::abs(check) < epsAlignedEdgesAngle)
						{
							polygon.erase(polygon.begin() + ii - 1);
							removedPoint = true;
							break;
						}
					}
				}

				if (!removedPoint)
				{
					break;
				}
			}
		}
	}

	static void simplifyPolygon(std::vector<std::vector<std::array<double, 2> > >& vecOfPolygons, bool mergeAlignedEdges, double epsMergePoints, double epsAlignedEdgesAngle)
	{
		for (size_t ii = 0; ii < vecOfPolygons.size(); ++ii)
		{
			std::vector<std::array<double, 2> >& polygon = vecOfPolygons[ii];
			simplifyPolygon(polygon, mergeAlignedEdges, epsMergePoints, epsAlignedEdgesAngle);
		}
	}

	static void simplifyPolygon(std::vector<std::vector<vec2> >& vecOfPolygons, bool mergeAlignedEdges, double epsMergePoints, double epsAlignedEdgesAngle)
	{
		for (size_t ii = 0; ii < vecOfPolygons.size(); ++ii)
		{
			std::vector<vec2>& polygon = vecOfPolygons[ii];
			simplifyPolygon(polygon, mergeAlignedEdges, epsMergePoints, epsAlignedEdgesAngle);
		}
	}

	inline void removeDuplicates(std::vector<vec3>& loop, double eps)
	{
		if (loop.size() > 1)
		{
			auto it_loop = loop.begin();
			double previous_x = (*it_loop).x;
			double previous_y = (*it_loop).y;
			double previous_z = (*it_loop).z;
			++it_loop;

			while (it_loop != loop.end())
			{
				vec3& current_point = *it_loop;
				if (std::abs(current_point.x - previous_x) < eps)
				{
					if (std::abs(current_point.y - previous_y) < eps)
					{
						if (std::abs(current_point.z - previous_z) < eps)
						{
							previous_x = current_point.x;
							previous_y = current_point.y;
							previous_z = current_point.z;
							it_loop = loop.erase(it_loop);
							continue;
						}
					}
				}
				previous_x = current_point.x;
				previous_y = current_point.y;
				previous_z = current_point.z;
				++it_loop;
			}
		}
	}

	inline void removeDuplicates(std::vector<vec2>& loop, double eps)
	{
		if (loop.size() > 1)
		{
			auto it_loop = loop.begin();
			double previous_x = (*it_loop).x;
			double previous_y = (*it_loop).y;
			++it_loop;

			while (it_loop != loop.end())
			{
				vec2& current_point = *it_loop;
				if (std::abs(current_point.x - previous_x) < eps)
				{
					if (std::abs(current_point.y - previous_y) < eps)
					{
						previous_x = current_point.x;
						previous_y = current_point.y;
						it_loop = loop.erase(it_loop);
						continue;
					}
				}
				previous_x = current_point.x;
				previous_y = current_point.y;
				++it_loop;
			}
		}
	}

	inline void removeDuplicates(std::vector<std::vector<vec2> >& paths, double eps)
	{
		for (size_t ii = 0; ii < paths.size(); ++ii)
		{
			std::vector<vec2>& loop = paths[ii];
			removeDuplicates(loop, eps);
		}
	}
	inline void copyClosedLoopSkipDuplicates(const std::vector<vec2>& loop_in, std::vector<vec2>& loop_out, double eps)
	{
		loop_out.clear();
		if (loop_in.size() > 0)
		{
			loop_out.reserve(loop_in.size());
			vec2 previous_point = loop_in[0];
			loop_out.push_back(previous_point);

			if (loop_in.size() > 1)
			{
				for (size_t ii = 1; ii < loop_in.size(); ++ii)
				{
					const vec2& current_point = loop_in[ii];
					if (std::abs(current_point.x - previous_point.x) < eps)
					{
						if (std::abs(current_point.y - previous_point.y) < eps)
						{
							continue;
						}
					}
					loop_out.push_back(current_point);
					previous_point.x = current_point.x;
					previous_point.y = current_point.y;
				}

				// delete last point if equal to first
				while (loop_out.size() > 2)
				{
					vec2& first = loop_out.front();
					vec2& last = loop_out.back();

					if (std::abs(first.x - last.x) < eps)
					{
						if (std::abs(first.y - last.y) < eps)
						{
							loop_out.pop_back();
							continue;
						}
					}
					break;
				}
			}
		}
	}

	inline void removeLastIfEqualToFirst(std::vector<int>& loop)
	{
		if (loop.size() > 1)
		{
			for (size_t iiPoint = 0; iiPoint < loop.size(); ++iiPoint)
			{
				size_t idxFirst = loop.front();
				size_t idxLast = loop.back();
				if (idxFirst == idxLast)
				{
					loop.pop_back();
					if (loop.size() < 2)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	inline void intersectLoopWithPoints(std::vector<vec3>& loop, std::vector<carve::mesh::Vertex<3> >& vertices, double eps, size_t maxNumPoints)
	{
		for (size_t repeat = 0; repeat < loop.size(); ++repeat)
		{
			if (loop.size() > 1)
			{
				bool intersectionFound = false;
				size_t kkIntersect = 0;
				size_t iiIntersect = 0;
				for (size_t ii = 0; ii < loop.size(); ++ii)
				{
					const vec3& p0 = loop[ii];
					const vec3& p1 = loop[(ii + 1) % loop.size()];
					const carve::geom::vector<3> linesegment_delta = p1 - p0;
					double dotLineSegDelta = dot(linesegment_delta, linesegment_delta);
					bool pointAdded = false;

					for (size_t kk = 0; kk < vertices.size(); ++kk)
					{
						const vec3& v = vertices[kk].v;
						double t = -1;
						bool onSegment = isPointOnLineSegment(p0, linesegment_delta, dotLineSegDelta, v, t, eps);

						if (onSegment)
						{
							if (t > eps && t < 1.0 - eps)
							{
								intersectionFound = true;
								kkIntersect = kk;
								iiIntersect = ii;
								break;
							}
						}

						if (kk > maxNumPoints)
						{
							break;
						}
					}
					if (intersectionFound)
					{
						break;
					}

					if (ii > maxNumPoints)
					{
						break;
					}
				}

				if (intersectionFound)
				{
					auto it = loop.begin();
					std::advance(it, iiIntersect);
					const vec3& v = vertices[kkIntersect].v;
					loop.insert(it, v);
				}
				else
				{
					break;
				}


			}
			if (repeat > maxNumPoints)
			{
				break;
			}
		}
	}

	inline std::pair<vec3, vec3> shortestConnectionSegmentToSegment(const vec3 A, const vec3 B, const vec3 C, const vec3 D, double eps)
	{
		vec3 u = B - A;
		vec3 v = D - C;
		vec3 w = A - C;

		double    a = dot(u, u);         // always >= 0
		double    b = dot(u, v);
		double    c = dot(v, v);         // always >= 0
		double    d = dot(u, w);
		double    e = dot(v, w);
		double    sc, sN, sD = a * c - b * b;  // sc = sN / sD, sD >= 0
		double    tc, tN, tD = a * c - b * b;  // tc = tN / tD, tD >= 0
		double    tol = eps;

		// compute the line parameters of the two closest points
		if (sD < tol)
		{
			// the lines are almost parallel
			sN = 0.0;              // force using point A on segment AB to prevent possible division by 0.0 later
			sD = 1.0;
			tN = e;
			tD = c;
		}
		else
		{
			// get the closest points on the infinite lines
			sN = (b * e - c * d);
			tN = (a * e - b * d);
			if (sN < 0.0)
			{
				// sc < 0 => the s=0 edge is visible
				sN = 0.0;          // compute shortest connection of A to segment CD
				tN = e;
				tD = c;
			}
			else if (sN > sD) {    // sc > 1  => the s=1 edge is visible
				sN = sD;           // compute shortest connection of B to segment CD
				tN = e + b;
				tD = c;
			}
		}

		if (tN < 0.0)
		{
			// tc < 0 => the t=0 edge is visible
			tN = 0.0;
			// recompute sc for this edge
			if (-d < 0.0)          // compute shortest connection of C to segment AB
			{
				sN = 0.0;
			}
			else if (-d > a)
			{
				sN = sD;
			}
			else
			{
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD)
		{
			// tc > 1  => the t=1 edge is visible
			tN = tD;
			// recompute sc for this edge
			if ((-d + b) < 0.0)  // compute shortest connection of D to segment AB
			{
				sN = 0;
			}
			else if ((-d + b) > a)
			{
				sN = sD;
			}
			else
			{
				sN = (-d + b);
				sD = a;
			}
		}
		// finally do the division to get sc and tc
		sc = (fabs(sN) < tol ? 0.0 : sN / sD);
		tc = (fabs(tN) < tol ? 0.0 : tN / tD);

		vec3 P1 = A + (sc * u);
		vec3 P2 = C + (tc * v);

		return { P1, P2 };   // return the closest distance
	}

	inline bool pointInPolySimple(const std::vector<vec2>& polygon, const vec2& testPoint, double eps)
	{
		if (polygon.size() < 3)
		{
			return false;
		}

		int numIntersections = 0;
		int numVertices = polygon.size();

		for (int i = 0; i < numVertices; ++i)
		{
			int nextIndex = (i + 1) % numVertices;

			// Check if the ray from the test point intersects with the edge
			if ((polygon[i].y > testPoint.y) != (polygon[nextIndex].y > testPoint.y) &&
				(testPoint.x < (polygon[nextIndex].x - polygon[i].x) * (testPoint.y - polygon[i].y) / (polygon[nextIndex].y - polygon[i].y) + polygon[i].x))
			{
				++numIntersections;
			}
		}

		return numIntersections % 2 == 1;
	}

	enum PointPosition {
		INSIDE,
		ON_EDGE,
		OUTSIDE
	};

	inline PointPosition pointInPolySimple(const array2d& p, const std::vector<array2d>& vertices, double epsilon)
	{
		int numVertices = static_cast<int>(vertices.size());

		for (int i = 0, j = numVertices - 1; i < numVertices; j = i++) {
			// Check if the point is close enough to the current edge
			if (fabs((p[0] - vertices[i][0]) * (vertices[j][1] - vertices[i][1]) -
				(vertices[j][0] - vertices[i][0]) * (p[1] - vertices[i][1])) < epsilon) {
				return ON_EDGE; // Point is considered on the edge
			}

			// Check for intersection between the ray and the edge
			if ((vertices[i][1] > p[1]) != (vertices[j][1] > p[1]) &&
				p[0] < (vertices[j][0] - vertices[i][0]) * (p[1] - vertices[i][1]) / (vertices[j][1] - vertices[i][1]) + vertices[i][0]) {
				return INSIDE; // Point is inside the polygon
			}
		}

		return OUTSIDE; // Point is outside the polygon
		

		//int numIntersections = 0;
		//int numVertices = polygon.size();
		//if (numVertices < 3)
		//{
		//	return false;
		//}

		//for (int i = 0; i < numVertices; ++i)
		//{
		//	int nextIndex = (i + 1) % numVertices;
		//	const array2d& polygonPoint = polygon[i];
		//	const array2d& polygonPointNext = polygon[nextIndex];

		//	// Check if the ray from the test point intersects with the edge
		//	if ((polygonPoint[1] > testPoint[1]) != (polygonPointNext[1] > testPoint[1]) &&
		//		(testPoint[0] < (polygonPointNext[0] - polygonPoint[0]) * (testPoint[1] - polygonPoint[1]) / (polygonPointNext[1] - polygonPoint[1]) + polygon[i][0])) {
		//		++numIntersections;
		//	}
		//}

		//return numIntersections % 2 == 1;
	}

	inline bool isEnclosed(const std::vector<vec2>& loop1, const std::vector<vec2>& loop2, double eps)
	{
		bool all_points_inside = true;
		for (size_t ii = 0; ii < loop1.size(); ++ii)
		{
			const vec2& p1 = loop1[ii];
			if (!pointInPolySimple(loop2, p1, eps))
			{
				all_points_inside = false;
				break;
			}
		}
		return all_points_inside;
	}

	//\brief: finds the first occurrence of T in vector
	template<typename T, typename U>
	bool findFirstInVector(std::vector<shared_ptr<U> > vec, shared_ptr<T>& ptr)
	{
		typename std::vector<shared_ptr<U> >::iterator it_trim = vec.begin();
		for (; it_trim != vec.end(); ++it_trim)
		{
			shared_ptr<U> item = *(it_trim);
			ptr = dynamic_pointer_cast<T>(item);
			if (ptr)
			{
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool allPointersValid(const std::vector<shared_ptr<T> >& vec)
	{
		for (size_t ii = 0; ii < vec.size(); ++ii)
		{
			const shared_ptr<T>& ptr = vec[ii];
			if (!ptr)
			{
				return false;
			}
		}
		return true;
	}

	//\brief: collect connected edges and create face
	static carve::mesh::Face<3>* createFaceFromEdgeLoop(carve::mesh::Edge<3>* start, double eps)
	{
		carve::mesh::Edge<3>* e = start;
		std::vector<carve::mesh::Edge<3>*> loop_edges;
		do {
			if (e->rev != nullptr)
			{
				return nullptr;
			}
			loop_edges.push_back(e);
			e = e->perimNext();
		} while (e != start);

		const size_t N = loop_edges.size();
		for (size_t i = 0; i < N; ++i)
		{
			loop_edges[i]->rev = new carve::mesh::Edge<3>(loop_edges[i]->v2(), nullptr);
		}

		for (size_t i = 0; i < N; ++i)
		{
			carve::mesh::Edge<3>* openEdge = loop_edges[i];
			carve::mesh::Edge<3>* openEdgeNext = loop_edges[(i + 1) % N];
			carve::mesh::Edge<3>* e1 = openEdge->rev;
			carve::mesh::Edge<3>* e2 = openEdgeNext->rev;
			e1->prev = e2;
			e2->next = e1;

			e1->rev = openEdge;
			e2->rev = openEdgeNext;
		}

		carve::mesh::Face<3>* f = new carve::mesh::Face<3>(start->rev, eps);

		if (f->n_edges != N)
		{
			delete f;
			return nullptr;
		}

		return f;
	}

	//\brief: 
	static void closeMeshSet(carve::mesh::MeshSet<3>* meshset, double eps)
	{
		// try to fix open mesh
		for (size_t i = 0; i < meshset->meshes.size(); ++i)
		{
			carve::mesh::MeshSet<3>::mesh_t* mesh = meshset->meshes[i];
			const size_t numOpenEdgesInitial = mesh->open_edges.size();
			if (numOpenEdgesInitial == 0)
			{
				continue;
			}
			for (size_t kk = 0; kk < numOpenEdgesInitial; ++kk)
			{
				const size_t numOpenEdges = mesh->open_edges.size();
				if (numOpenEdges == 0)
				{
					break;
				}

				mesh->faces.reserve(numOpenEdges + 1);

				carve::mesh::Edge<3>* start = mesh->open_edges[0];

				carve::mesh::Edge<3>* openEdge1 = nullptr;
				carve::mesh::Edge<3>* openEdge2 = nullptr;
				std::vector<carve::mesh::Edge<3>*> edges_to_close;
				edges_to_close.resize(numOpenEdges);
				carve::mesh::Edge<3>* edge = start;
				size_t j = 0;
				size_t numOpenEdgesCurrentLoop = 0;
				do {
					edges_to_close[j++] = edge;

					carve::mesh::Edge<3>* currentEdge = edge;
					carve::mesh::Edge<3>* nextEdge = currentEdge->perimNext();
					++numOpenEdgesCurrentLoop;

					if (openEdge1 == nullptr)
					{
						// check if nextEdge is also an open edge
						for (size_t mm = 0; mm < mesh->open_edges.size(); ++mm)
						{
							carve::mesh::Edge<3>* e = mesh->open_edges[mm];
							if (e == nextEdge)
							{
								openEdge1 = currentEdge;
								openEdge2 = nextEdge;
								break;
							}
						}
					}
					edge = nextEdge;
				} while (edge != start);

				if (numOpenEdgesCurrentLoop == 3)
				{
					if (openEdge1 != nullptr)
					{
						// close with triangle
						carve::mesh::Face<3>* closingTriangle = createFaceFromEdgeLoop(openEdge1, eps);
						if (closingTriangle != nullptr)
						{
							closingTriangle->mesh = mesh;
							mesh->faces.push_back(closingTriangle);
						}
					}
				}
				else if (numOpenEdgesCurrentLoop > 3)
				{
					if (openEdge1 != nullptr && openEdge2 != nullptr)
					{
						// add triangle with 2 open edges and a new edge
						carve::mesh::Face<3>* triangle = new carve::mesh::Face<3>(openEdge1->v2(), openEdge1->v1(), openEdge2->v2(), eps);
						triangle->mesh = mesh;
						openEdge1->rev = triangle->edge;
						triangle->edge->rev = openEdge1;
						mesh->faces.push_back(triangle);

						carve::mesh::Edge<3>* e1 = openEdge1->rev;
						carve::mesh::Edge<3>* e2 = e1->prev;
						openEdge2->rev = e2;
						e2->rev = openEdge2;
						//e1->validateLoop();
					}
				}

				meshset->collectVertices();
				mesh->cacheEdges();
				mesh->calcOrientation();
			}
		}
	}

	static double triangleArea(const glm::dvec3& A, const glm::dvec3& B, const glm::dvec3& C)
	{
		glm::dvec3 AB(B - A);
		glm::dvec3 AC(C - A);
		glm::dvec3 crossProduct = glm::cross(AB, AC);
		return glm::length(crossProduct) * 0.5;
	}

	static double triangleArea(const vec3& A, const vec3& B, const vec3& C)
	{
		vec3 AB(B - A);
		vec3 AC(C - A);
		vec3 crossProduct = cross(AB, AC);
		return crossProduct.length() * 0.5;
	}

	static double distance2(const glm::dvec3& A, const glm::dvec3& B)
	{
		double dx = A.x - B.x;
		double dy = A.y - B.y;
		double dz = A.z - B.z;
		return dx * dx + dy * dy + dz * dz;
	}

	static bool pointOnEdge(const glm::dvec3& point, const glm::dvec3& edgePoint0, const glm::dvec3& edgePoint1, double eps)
	{
		glm::dvec3 pointOnEdge0 = glm::closestPointOnLine(point, edgePoint0, edgePoint1);
		double dist2 = GeomUtils::distance2(pointOnEdge0, point);
		if (dist2 < eps * 0.0001)
		{
			// edge is intersected
			return true;
		}
		return false;
	}

	static bool equals(const carve::geom::vector<3>& A, const carve::geom::vector<3>& B, double eps = 0)
	{
		return std::abs(A.x - B.x) <= eps && std::abs(A.y - B.y) <= eps && std::abs(A.z - B.z) <= eps;
	}

	static bool equals2d(const carve::geom::vector<2>& A, const carve::geom::vector<2>& B, double eps = 0)
	{
		return std::abs(A.x - B.x) <= eps && std::abs(A.y - B.y) <= eps;
	}

	enum RayTriangleIntersection { INTERSECT_EDGE, INTERSECT_INTERIOR, INTERSECT_NONE };
	static RayTriangleIntersection intersectRayTriangle(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection, const glm::dvec3& p0, const glm::dvec3& p1, const glm::dvec3& p2, double eps)
	{
		double t = 0;
		glm::dvec2 baryPosition(1, 1);
		bool intersects = glm::intersectRayTriangle(rayOrigin, rayDirection, p0, p1, p2, baryPosition, t);
		bool edgeIntersected = false;
		if (intersects && t >= 0)
		{
			edgeIntersected = true;
			// check if edge is intersected
			glm::dvec3 intersectionPoint = rayOrigin + rayDirection * t;

			if (pointOnEdge(intersectionPoint, p0, p1, eps))
			{
				return RayTriangleIntersection::INTERSECT_EDGE;
			}

			if (pointOnEdge(intersectionPoint, p0, p2, eps))
			{
				return RayTriangleIntersection::INTERSECT_EDGE;
			}

			if (pointOnEdge(intersectionPoint, p1, p2, eps))
			{
				return RayTriangleIntersection::INTERSECT_EDGE;
			}

			// edge is not intersected
			return RayTriangleIntersection::INTERSECT_INTERIOR;
		}
		return RayTriangleIntersection::INTERSECT_NONE;
	}

	class aabb {
	public:
		glm::dvec3 pos;
		glm::dvec3 extent;

		aabb()
		{
			pos = glm::dvec3(0, 0, 0);
			extent = glm::dvec3(0, 0, 0);
		}

		aabb(glm::dvec3 _pos, glm::dvec3 _extent)
		{
			pos = _pos;
			extent = _extent;
		}

		bool isEmpty(double eps)
		{
			if (std::abs(extent.x) > eps)
			{
				return false;
			}

			if (std::abs(extent.y) > eps)
			{
				return false;
			}

			if (std::abs(extent.z) > eps)
			{
				return false;
			}

			return true;
		}

		void unionAABB(const aabb& other)
		{
			double xmin = pos.x - extent.x;
			double ymin = pos.y - extent.y;
			double zmin = pos.z - extent.z;

			double xmax = pos.x + extent.x;
			double ymax = pos.y + extent.y;
			double zmax = pos.z + extent.z;

			double xmax_other = other.pos.x + other.extent.x;
			double ymax_other = other.pos.y + other.extent.y;
			double zmax_other = other.pos.z + other.extent.z;

			double xmin_other = other.pos.x - other.extent.x;
			double ymin_other = other.pos.y - other.extent.y;
			double zmin_other = other.pos.z - other.extent.z;

			double new_xmin = std::min(xmin, xmin_other);
			double new_ymin = std::min(ymin, ymin_other);
			double new_zmin = std::min(zmin, zmin_other);

			double new_xmax = std::max(xmax, xmax_other);
			double new_ymax = std::max(ymax, ymax_other);
			double new_zmax = std::max(zmax, zmax_other);

			pos.x = (new_xmax + new_xmin) * 0.5;
			pos.y = (new_ymax + new_ymin) * 0.5;
			pos.z = (new_zmax + new_zmin) * 0.5;

			extent.x = (new_xmax - new_xmin) * 0.5;
			extent.y = (new_ymax - new_ymin) * 0.5;
			extent.z = (new_zmax - new_zmin) * 0.5;
		}
	};
}
