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

#include "GeomUtils.h"

class PointProjector
{
public:
	struct RestOfPoint
	{
		RestOfPoint(double _second, double _third)
		{
			multimap.insert({ _second, _third });
		}

		std::multimap<double, double> multimap;
	};

	GeomUtils::ProjectionPlane m_projectionTarget;
	GeomUtils::Plane m_plane;
	std::map<double, RestOfPoint> m_mapRestore;


	PointProjector() {}
	PointProjector(const vec3& faceNormal, const vec3& pointOnFace )
	{
		m_projectionTarget = GeomUtils::UNDEFINED;

		double nx = std::abs(faceNormal.x);
		double ny = std::abs(faceNormal.y);
		double nz = std::abs(faceNormal.z);
		if( nz > nx && nz >= ny )
		{
			m_projectionTarget = GeomUtils::XY_PLANE;
		}
		else if( nx >= ny && nx >= nz )
		{
			m_projectionTarget = GeomUtils::YZ_PLANE;
		}
		else if( ny > nx && ny >= nz )
		{
			m_projectionTarget = GeomUtils::XZ_PLANE;
		}
		else
		{
			std::cout << "unable to project to plane: nx" << nx << " ny " << ny << " nz " << nz << std::endl;
		}

		m_plane.setPlane( glm::dvec3( pointOnFace.x, pointOnFace.y, pointOnFace.z ), glm::dvec3( faceNormal.x, faceNormal.y, faceNormal.z ) );
	}

	bool isEqual(const PointProjector& other, double epsAngle, double epsDistance )
	{
		bool allowOppositeNormals = true;
		return other.m_plane.isEqualToPlane(m_plane, allowOppositeNormals, epsDistance, epsAngle);
	}

	glm::dvec3 unprojectPointyByPlane(double x, double y) const
	{
		double t = 0;
		if( m_projectionTarget == GeomUtils::XY_PLANE )
		{
			// restore z coordinate by intersecting with m_plane
			glm::dvec3 ray_origin(x, y, 0);
			glm::dvec3 ray_direction(0, 0, 1);

			bool intersect = GeomUtils::intersectRayPlane(ray_origin, ray_direction, m_plane.getPosition(), m_plane.getNormal(), t);
			if( std::abs(t) < EPS_DEFAULT )
			{
				intersect = true;
			}
			if( intersect )
			{
				glm::dvec3 v = ray_origin + ray_direction * t;

#ifdef _DEBUG
				double distance_check = m_plane.distancePointPlane(v);
				if( std::abs(distance_check) > EPS_DEFAULT )
				{
					std::cout << "distance_check > 0: " << distance_check << std::endl;
				}
				distance_check = m_plane.distancePointPlane(glm::dvec3(x, y, v.z));
				if( std::abs(distance_check) > EPS_DEFAULT )
				{
					std::cout << "distance_check > 0: " << distance_check << std::endl;
				}

#endif

				return glm::dvec3(x, y, v.z);
			}
		}
		else if( m_projectionTarget == GeomUtils::YZ_PLANE )
		{
			// restore x coordinate by intersecting with m_plane
			glm::dvec3 ray_origin(0, x, y);
			glm::dvec3 ray_direction(1, 0, 0);

			bool intersect = GeomUtils::intersectRayPlane(ray_origin, ray_direction, m_plane.getPosition(), m_plane.getNormal(), t);
			if( std::abs(t) < EPS_DEFAULT )
			{
				intersect = true;
			}
			if( intersect )
			{
				glm::dvec3 v = ray_origin + ray_direction * t;
#ifdef _DEBUG
				double distance_check = m_plane.distancePointPlane(v);
				if( std::abs(distance_check) > EPS_DEFAULT )
				{
					std::cout << "distance_check > 0: " << distance_check << std::endl;
				}
#endif
				return glm::dvec3(v.x, x, y);
			}
		}
		else if( m_projectionTarget == GeomUtils::XZ_PLANE )
		{
			// restore y coordinate by intersecting with m_plane
			glm::dvec3 ray_origin(x, 0, y);
			glm::dvec3 ray_direction(0, 1, 0);

			bool intersect = GeomUtils::intersectRayPlane(ray_origin, ray_direction, m_plane.getPosition(), m_plane.getNormal(), t);
			if( std::abs(t) < EPS_DEFAULT )
			{
				intersect = true;
			}
			if( intersect )
			{
				glm::dvec3 v = ray_origin + ray_direction * t;
#ifdef _DEBUG
				double distance_check = m_plane.distancePointPlane(v);
				if( std::abs(distance_check) > EPS_DEFAULT )
				{
					std::cout << "distance_check > 0: " << distance_check << std::endl;
				}
#endif
				return glm::dvec3(x, v.y, y);
			}
		}
		std::cout << "could not unproject point" << std::endl;
		return glm::dvec3(0, 0, 0);
	}

	glm::dvec3 unprojectPoint(double x, double y) const
	{
		glm::dvec3 unprojectedByPlane = unprojectPointyByPlane(x, y);
		return unprojectedByPlane;

//			double x = point2D[0];
//			double y = point2D[1];
//
//			auto it_find_first = m_mapRestore.lower_bound(x);
//			if( it_find_first == m_mapRestore.end() )
//			{
//				it_find_first = m_mapRestore.upper_bound(x);
//				if( it_find_first == m_mapRestore.end() )
//				{
//					return unprojectedByPlane;
//				}
//			}
//			auto it_best_x = it_find_first;
//
//			double minDeltaX = DBL_MAX;
//			for( ; it_find_first != m_mapRestore.end(); ++it_find_first )
//			{
//				double x_it = it_find_first->first;
//				double delt = std::abs(x_it - x);
//				if( delt > EPS_M12 )
//				{
//					break;
//				}
//
//				if( delt < minDeltaX )
//				{
//					it_best_x = it_find_first;
//					minDeltaX = delt;
//				}
//			}
//
//			if( it_best_x == m_mapRestore.end() )
//			{
//				return unprojectedByPlane;
//			}
//			RestOfPoint& map23 = it_best_x->second;
//
//			if( m_projectionTarget == GeomUtils::XY_PLANE )
//			{
//				// x and y are used, z is saved for restore
//				auto it_find_second = map23.multimap.find(y);
//
//				if( it_find_second != map23.multimap.end() )
//				{
//					double restoredCoord = it_find_second->second;
//#ifdef _DEBUG
//					glm::dvec3 restoredPoint(x, y, restoredCoord);
//					double dx = unprojectedByPlane.x - restoredPoint.x;
//					double dy = unprojectedByPlane.y - restoredPoint.y;
//					double dz = unprojectedByPlane.z - restoredPoint.z;
//					double dist = glm::distance(unprojectedByPlane, restoredPoint);
//					if( dist > EPS_M5 )
//					{
//						boostPoint2D unprojectedByPlanePlane = projectPoint(unprojectedByPlane);
//						boostPoint2D unprojectedByPlaneMap = projectPoint(restoredPoint);
//						std::cout << "difference in unprojected point: " << dist << std::endl;
//
//					}
//#endif
//					return glm::dvec3(x, y, restoredCoord);
//				}
//			}
//			else if( m_projectionTarget == GeomUtils::YZ_PLANE )
//			{
//				// y and z are used, x is saved for restore
//				auto it_find_second = map23.multimap.find(y);
//				if( it_find_second != map23.multimap.end() )
//				{
//					double restoredCoord = it_find_second->second;
//#ifdef _DEBUG
//					double dist = glm::distance(unprojectedByPlane, glm::dvec3(restoredCoord, x, y));
//					if( dist > EPS_M6 * 2.5 )
//					{
//						std::cout << "difference in unprojected point: " << dist << std::endl;
//					}
//#endif
//					return glm::dvec3(restoredCoord, x, y);
//				}
//			}
//			else if( m_projectionTarget == GeomUtils::XZ_PLANE )
//			{
//				// x and z are used, y is saved for restore
//				auto it_find_second = map23.multimap.lower_bound(y);
//				double minDelta = DBL_MAX;
//				double restoredY = unprojectedByPlane.y;
//
//				for( ; it_find_second != map23.multimap.end(); ++it_find_second )
//				{
//					double y_it = it_find_second->first;
//					if( std::abs(y_it - y) > EPS_M12 )
//					{
//						break;
//					}
//					double restoredCoord = it_find_second->second;
//					double delt = std::abs(restoredCoord - unprojectedByPlane.y);
//
//					if( delt < minDelta )
//					{
//						restoredY = restoredCoord;
//						minDelta = delt;
//					}
//				}
//#ifdef _DEBUG
//				double dx = unprojectedByPlane.x - x;
//				double dy = unprojectedByPlane.y - restoredY;
//				double dz = unprojectedByPlane.z - y;
//				double dist = glm::distance(unprojectedByPlane, glm::dvec3(x, restoredY, y));
//				if( dist > EPS_M6 * 5 )
//				{
//					std::cout << "difference in unprojected point. dy=" << dy << std::endl;
//				}
//				assert(std::abs(dx) < EPS_M12);
//				assert(std::abs(dz) < EPS_M12);
//#endif
//				return glm::dvec3(x, restoredY, y);
//
//			}
//
//			return unprojectedByPlane;
	}
};
