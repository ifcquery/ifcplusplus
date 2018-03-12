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

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcCurve.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcLoop.h>
#include <ifcpp/IFC4/include/IfcPointOnCurve.h>
#include <ifcpp/IFC4/include/IfcPointOnSurface.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcReal.h>
#include <ifcpp/IFC4/include/IfcTrimmingSelect.h>
#include <ifcpp/IFC4/include/IfcVertex.h>
#include <ifcpp/IFC4/include/IfcVertexPoint.h>

#include "GeomUtilsOCC.h"

//\brief class to convert IFC point representations to OCC
class PointConverterOCC
{
public:
	static bool convertIfcCartesianPoint( const shared_ptr<IfcCartesianPoint>& ifc_point, gp_Vec& point, double length_factor )
	{
		if( !ifc_point )
		{
			return false;
		}
		std::vector<shared_ptr<IfcLengthMeasure> >& coords1 = ifc_point->m_Coordinates;
		if( coords1.size() > 2 )
		{
#ifdef ROUND_IFC_COORDINATES
			double x = round( coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double y = round( coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double z = round( coords1[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords1[0]->m_value*length_factor;
			double y = coords1[1]->m_value*length_factor;
			double z = coords1[2]->m_value*length_factor;
#endif
			point = gp_Vec( x, y, z );
			return true;
		}
		else if( coords1.size() > 1 )
		{
#ifdef ROUND_IFC_COORDINATES
			double x = round( coords1[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
			double y = round( coords1[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
			double x = coords1[0]->m_value*length_factor;
			double y = coords1[1]->m_value*length_factor;
#endif
			point = gp_Vec( x, y, 0.0 );
			return true;
		}
		return false;
	}

	static bool convertIfcDirection( const shared_ptr<IfcDirection>& ifc_direction, gp_Vec& direction_vector )
	{
		if( !ifc_direction )
		{
			return false;
		}

		const std::vector<shared_ptr<IfcReal> >& direction_ratios = ifc_direction->m_DirectionRatios;
		if( !GeomUtilsOCC::allPointersValid( direction_ratios ) )
		{
			return false;
		}
		if( direction_ratios.size() < 2 )
		{
			return false;
		}
		
		if( direction_ratios.size() > 2 )
		{
				direction_vector = gp_Vec( direction_ratios[0]->m_value, direction_ratios[1]->m_value, direction_ratios[2]->m_value );
		}
		else
		{
			direction_vector = gp_Vec( direction_ratios[0]->m_value, direction_ratios[1]->m_value, 0 );
		}
		direction_vector.Normalize();
		return true;
	}

	static void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, TopoDS_Wire& target_wire, double length_factor )
	{
		const size_t num_points = points.size();
		std::vector<gp_Vec> vec_vertices;
		for( size_t i_point = 0; i_point < num_points; ++i_point )
		{
			const shared_ptr<IfcCartesianPoint>& ifc_cartesian_point = points[i_point];
			if( !ifc_cartesian_point )
			{
				continue;
			}

			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = ifc_cartesian_point->m_Coordinates;
			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
				double z = coords[2]->m_value*length_factor;
#endif
				vec_vertices.push_back( gp_Vec( x, y, z ) );
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
#endif
				vec_vertices.push_back( gp_Vec( x, y, 0.0 ) );
			}
			else
			{
#ifdef _DEBUG
				std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
#endif
			}
		}

		GeomUtilsOCC::createWireFromPoints( vec_vertices, target_wire, false );
	}
	static void convertIfcCartesianPointVector( const std::vector<shared_ptr<IfcCartesianPoint> >& points, std::vector<gp_Vec>& loop, double length_factor )
	{
		const size_t num_points = points.size();
		for( size_t i_point = 0; i_point < num_points; ++i_point )
		{
			const shared_ptr<IfcCartesianPoint>& ifc_cartesian_point = points[i_point];
			if( !ifc_cartesian_point )
			{
				continue;
			}

			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = ifc_cartesian_point->m_Coordinates;
			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
				double z = coords[2]->m_value*length_factor;
#endif
				loop.push_back( gp_Vec( x, y, z ) );
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				double x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				double y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				double x = coords[0]->m_value*length_factor;
				double y = coords[1]->m_value*length_factor;
#endif
				loop.push_back( gp_Vec( x, y, 0.0 ) );
			}
			else
			{
#ifdef _DEBUG
				std::cout << "convertIfcCartesianPointVector: ifc_pt->m_Coordinates.size() != 2" << std::endl;
#endif
			}
		}
	}
	static void convertIfcCartesianPointVector2D( const std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& vec_points_in, std::vector<gp_Vec>& vertices, double length_factor )
	{
		for( size_t ii = 0; ii < vec_points_in.size(); ++ii )
		{
			const std::vector<shared_ptr<IfcCartesianPoint> >& points_inner = vec_points_in[ii];
			for( size_t jj = 0; jj < points_inner.size(); ++jj )
			{
				const shared_ptr<IfcCartesianPoint>& cartesian_point = points_inner[jj];

				if( !cartesian_point )
				{
					continue;
				}

				const std::vector<shared_ptr<IfcLengthMeasure> >& coords = cartesian_point->m_Coordinates;
				if( coords.size() > 2 )
				{
					vertices.push_back( gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, coords[2]->m_value*length_factor ) );
				}
				else if( coords.size() > 1 )
				{
					vertices.push_back( gp_Vec( coords[0]->m_value*length_factor, coords[1]->m_value*length_factor, 0.0 ) );
				}
			}
		}
	}
	static void convertIfcCartesianPointVectorSkipDuplicates( const std::vector<shared_ptr<IfcCartesianPoint> >& vec_ifc_points, TopoDS_Wire& wire, bool close_wire_with_first_point, double length_factor )
	{
		std::vector<gp_Vec> vec_points;
		double previous_x = 0;
		double previous_y = 0;
		double previous_z = 0;
		for( size_t ii = 0; ii < vec_ifc_points.size(); ++ii )
		{
			const shared_ptr<IfcCartesianPoint>& cartesian_point = vec_ifc_points[ii];
			double x = 0.0, y = 0.0, z = 0.0;
			const std::vector<shared_ptr<IfcLengthMeasure> >& coords = cartesian_point->m_Coordinates;

			if( coords.size() > 2 )
			{
#ifdef ROUND_IFC_COORDINATES
				x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				z = round( coords[2]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				x = coords[0]->m_value*length_factor;
				y = coords[1]->m_value*length_factor;
				z = coords[2]->m_value*length_factor;
#endif
			}
			else if( coords.size() > 1 )
			{
#ifdef ROUND_IFC_COORDINATES
				x = round( coords[0]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
				y = round( coords[1]->m_value*length_factor*ROUND_IFC_COORDINATES_UP )*ROUND_IFC_COORDINATES_DOWN;
#else
				x = coords[0]->m_value*length_factor;
				y = coords[1]->m_value*length_factor;
#endif
			}

			// skip duplicate vertices
			if( ii > 0 )
			{
				if( std::abs( x - previous_x ) < 0.00000001 )
				{
					if( std::abs( y - previous_y ) < 0.00000001 )
					{
						if( std::abs( z - previous_z ) < 0.00000001 )
						{
							continue;
						}
					}
				}
			}
			vec_points.push_back( gp_Vec( x, y, z ) );
			previous_x = x;
			previous_y = y;
			previous_z = z;
		}
		GeomUtilsOCC::createWireFromPoints( vec_points, wire, close_wire_with_first_point );
	}
	static bool convertIfcVertex( const shared_ptr<IfcVertex>& vertex, gp_Vec& point_result, const double length_factor )
	{
		shared_ptr<IfcVertexPoint> vertex_point = dynamic_pointer_cast<IfcVertexPoint>( vertex );
		if( vertex_point )
		{
			if( vertex_point->m_VertexGeometry )
			{
				const shared_ptr<IfcPoint>& vertex_point_geometry = vertex_point->m_VertexGeometry;
				// ENTITY IfcPoint ABSTRACT SUPERTYPE OF(ONEOF(IfcCartesianPoint, IfcPointOnCurve, IfcPointOnSurface))
				shared_ptr<IfcCartesianPoint> cartesian_point = dynamic_pointer_cast<IfcCartesianPoint>( vertex_point_geometry );
				if( cartesian_point )
				{
					convertIfcCartesianPoint( cartesian_point, point_result, length_factor );
					return true;
				}

				shared_ptr<IfcPointOnCurve> point_on_curve = dynamic_pointer_cast<IfcPointOnCurve>(vertex_point_geometry);
				if( point_on_curve )
				{
					std::cout << "IfcVertexPoint as IfcPointOnCurve not implemented" << std::endl;
					return false;
				}

				shared_ptr<IfcPointOnSurface> point_on_surface = dynamic_pointer_cast<IfcPointOnSurface>(vertex_point_geometry);
				if( point_on_surface )
				{
					std::cout << "IfcVertexPoint as IfcPointOnSurface not implemented" << std::endl;
					return false;
				}
			}
		}
		return false;
	}
};
