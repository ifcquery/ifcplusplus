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

#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>

#include <ifcpp/IFC4/include/IfcBSplineCurve.h>
#include <ifcpp/IFC4/include/IfcBSplineSurface.h>
#include <ifcpp/IFC4/include/IfcInteger.h>
#include <ifcpp/IFC4/include/IfcParameterValue.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineCurveWithKnots.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h>
#include <ifcpp/IFC4/include/IfcReal.h>

#include "PointConverter.h"
#include "IncludeCarveHeaders.h"

class SplineConverter : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings> m_geom_settings;
	shared_ptr<PointConverter> m_point_converter;

public:
	static void computeKnotVector( const size_t num_ctrl_points, const size_t order, std::vector<double>& knot_vec )
	{
		const size_t n_plus_order = num_ctrl_points + order;
		const size_t n_plus_1 = num_ctrl_points + 1;

		// example: order=2, num_ctrl_points=4,  nplus1=5,  nplusc=6
		//                       order                            nplus1    nplusc
		//  knot[0]   knot[1]    knot[2]    knot[3]    knot[4]    knot[5]   knot[6]
		//  0         0          1          2          3          4         4

		knot_vec[0] = 0;  // start the knot vector with 0
		for( size_t ii = 1; ii < n_plus_order; ++ii )
		{
			if( ( ii >= order ) && ( ii < n_plus_1 ) )
			{
				knot_vec[ii] = knot_vec[ii - 1] + 1.0;
			}
			else
			{
				knot_vec[ii] = knot_vec[ii - 1];
			}
		}
	}

	static void computRationalBasisFunctions(
		const size_t order,
		const double t,
		const size_t num_control_points,
		const std::vector<double>& knot_vec,
		const std::vector<double>& weights,
		std::vector<double>& basis_func )
	{
		const size_t n_plus_order = num_control_points + order;  // maximum number of knot values

		// first order nonrational basis function
		std::vector<double> temp;
		temp.resize( n_plus_order + 1 );
		for( size_t ii = 0; ii < n_plus_order - 1; ii++ )
		{
			if( ( t >= knot_vec[ii] ) && ( t < knot_vec[ii + 1] ) )
			{
				temp[ii] = 1;
			}
			else
			{
				temp[ii] = 0;
			}
		}

		// higher order nonrational basis function
		double basis_func_part1; // first term of the basis function recursion relation
		double basis_func_part2; // second term of the basis function recursion relation
		for( size_t kk = 2; kk <= order; ++kk )
		{
			for( size_t ii = 0; ii <= n_plus_order - kk + 1; ++ii )
			{
				// skip if the lower order basis function is zero
				if( temp[ii] != 0 )
				{
					basis_func_part1 = ( ( t - knot_vec[ii] )*temp[ii] ) / ( knot_vec[ii + kk - 1] - knot_vec[ii] );
				}
				else
				{
					basis_func_part1 = 0;
				}

				// skip if the lower order basis function is zero
				if( temp[ii + 1] != 0 )
				{
					basis_func_part2 = ( ( knot_vec[ii + kk] - t )*temp[ii + 1] ) / ( knot_vec[ii + kk] - knot_vec[ii + 1] );
				}
				else
				{
					basis_func_part2 = 0;
				}

				temp[ii] = basis_func_part1 + basis_func_part2;
			}
		}

		if( t == knot_vec[n_plus_order - 1] )
		{
			// pick up last point
			temp[num_control_points - 1] = 1;
		}

		// compute sum for denominator of rational basis function
		double sum = 0.0;
		for( size_t ii = 0; ii < num_control_points; ++ii )
		{
			sum = sum + temp[ii] * weights[ii + 1];
		}

		// compute the rational basis function
		for( size_t ii = 0; ii < num_control_points; ii++ )
		{
			if( sum != 0.0 )
			{
				basis_func[ii] = temp[ii] * weights[ii + 1] / sum;
			}
			else
			{
				basis_func[ii] = 0;
			}
		}
	}

	static void computeRationalBSpline(
		const size_t order,
		const size_t num_curve_pts,
		const std::vector<vec3>& control_points,
		const std::vector<double>& weights,
		std::vector<double>& knot_vec,
		std::vector<double>& curve_points )
	{
		// order: order of the BSpline basis function
		std::vector<double> basis_func;  // basis function for parameter value t
		basis_func.resize( control_points.size() + 1, 0.0 );

		if( knot_vec.size() == 0 )
		{
			// generate a uniform open knot vector
			const size_t n_plus_order = control_points.size() + order; // number of knot values
			knot_vec.resize( n_plus_order, 0.0 );
			computeKnotVector( control_points.size(), order, knot_vec );
		}

		double t = 0; // parameter value 0 <= t <= npts - k + 1
		double step = knot_vec[knot_vec.size() - 1] / ( (double)( num_curve_pts - 1 ) );

		std::vector<double> control_points_coords;
		for( size_t ii = 0; ii < control_points.size(); ++ii )
		{
			control_points_coords.push_back( control_points[ii].x );
			control_points_coords.push_back( control_points[ii].y );
			control_points_coords.push_back( control_points[ii].z );
		}

		size_t offset_i = 0;
		double temp;
		for( size_t ii = 0; ii < num_curve_pts; ++ii )
		{
			if( t > knot_vec[knot_vec.size() - 1] - 0.000001 )
			{
				t = knot_vec[knot_vec.size() - 1];
			}

			// generate the basis function for this value of t
			computRationalBasisFunctions( order, t, control_points.size(), knot_vec, weights, basis_func );

			for( size_t jj = 0; jj < 3; ++jj )
			{
				curve_points[offset_i + jj] = 0.;

				for( size_t kk = 0; kk < control_points.size(); ++kk )
				{
					// matrix multiplication
					temp = basis_func[kk] * control_points_coords[jj + kk * 3];
					curve_points[offset_i + jj] = curve_points[offset_i + jj] + temp;
				}
			}

			offset_i = offset_i + 3;
			t = t + step;
		}
	}

	SplineConverter( shared_ptr<GeometrySettings>& geom_settings, shared_ptr<PointConverter>& pt_converter )
		: m_geom_settings( geom_settings ), m_point_converter( pt_converter )
	{
	}

	virtual ~SplineConverter()
	{
	}

	void convertBSplineCurve( const shared_ptr<IfcBSplineCurve>& bspline_curve, std::vector<vec3>& target_vec, std::vector<vec3>& segment_start_points ) const
	{
		if( !bspline_curve )
		{
			return;
		}
		if( !bspline_curve->m_Degree )
		{
			return;
		}
		const int											degree = bspline_curve->m_Degree->m_value;
		const std::vector<shared_ptr<IfcCartesianPoint> >&	control_points = bspline_curve->m_ControlPointsList;
		//const shared_ptr<IfcBSplineCurveForm>&				curve_form = bspline_curve->m_CurveForm;
		//const LogicalEnum									closed_curve = bspline_curve->m_ClosedCurve;
		//const LogicalEnum									self_intersect = bspline_curve->m_ClosedCurve;

		std::vector<vec3> vec_control_points;
		m_point_converter->convertIfcCartesianPointVector( control_points, vec_control_points );

		if( vec_control_points.size() < 2 )
		{
			return;
		}

		std::vector<double> vec_weights;
		std::vector<double> curve_points_coords;

		const size_t num_control_pts = vec_control_points.size();
		const size_t order = degree + 1; // the order of the curve is the degree of the resulting polynomial + s1
		const size_t num_curve_pts = num_control_pts * m_geom_settings->getNumVerticesPerControlPoint();
		std::vector<double> knot_vec;

		//	set weighting factors to 1.0 in case of homogenious curve
		vec_weights.resize( num_control_pts + 1, 1.0 );

		shared_ptr<IfcBSplineCurveWithKnots> bspline_curve_with_knots = dynamic_pointer_cast<IfcBSplineCurveWithKnots>( bspline_curve );
		if( bspline_curve_with_knots )
		{
			std::vector<shared_ptr<IfcInteger> >&			ifc_knot_mult = bspline_curve_with_knots->m_KnotMultiplicities;
			std::vector<shared_ptr<IfcParameterValue> >&	ifc_knots = bspline_curve_with_knots->m_Knots;
			//shared_ptr<IfcKnotType>&						ifc_knot_spec = bspline_curve_with_knots->m_KnotSpec;

			for( size_t ii = 0; ii < ifc_knots.size(); ++ii )
			{
				shared_ptr<IfcParameterValue>& knot_parameter = ifc_knots[ii];
				double knot_value = knot_parameter->m_value;
				knot_vec.push_back( knot_value );

				if( ifc_knot_mult.size() == ifc_knots.size() )
				{
					int num_multiply_knot_value = ifc_knot_mult[ii]->m_value;
					for( int jj = 0; jj < num_multiply_knot_value; ++jj )
					{
						knot_vec.push_back( knot_value );
					}
				}
			}

			// TODO: check knot multiplicities and degree

			shared_ptr<IfcRationalBSplineCurveWithKnots> r_bspline_curve_with_knots = dynamic_pointer_cast<IfcRationalBSplineCurveWithKnots>( bspline_curve_with_knots );
			if( r_bspline_curve_with_knots )
			{
				std::vector<shared_ptr<IfcReal> >& ifc_vec_weigths = r_bspline_curve_with_knots->m_WeightsData;
				vec_weights.resize( ifc_vec_weigths.size() );
				for( size_t i_weight = 0; i_weight < ifc_vec_weigths.size(); ++i_weight )
				{
					vec_weights[i_weight] = ifc_vec_weigths[i_weight]->m_value;
				}
			}
		}

		curve_points_coords.resize( 3 * num_curve_pts, 0.0 );
		computeRationalBSpline( order, num_curve_pts, vec_control_points, vec_weights, knot_vec, curve_points_coords );

		if( target_vec.size() > 2 )
		{
			segment_start_points.push_back( carve::geom::VECTOR( curve_points_coords[0], curve_points_coords[1], curve_points_coords[2] ) );
		}

		for( size_t ii = 0; ii < 3 * num_curve_pts; ii = ii + 3 )
		{
			target_vec.push_back( carve::geom::VECTOR( curve_points_coords[ii], curve_points_coords[ii + 1], curve_points_coords[ii + 2] ) );
		}
	}



	void convertIfcBSplineSurface( const shared_ptr<IfcBSplineSurface>& ifc_bspline_surface, shared_ptr<carve::input::PolylineSetData>& polyline_data )
	{
		// IfcBSplineSurface -----------------------------------------------------------
		// attributes:
		//int degree_u = ifc_bspline_surface->m_UDegree;
		//int degree_v = ifc_bspline_surface->m_VDegree;
		std::vector<std::vector<shared_ptr<IfcCartesianPoint> > >& ifc_control_points = ifc_bspline_surface->m_ControlPointsList;
		shared_ptr<IfcBSplineSurfaceForm> surface_form = ifc_bspline_surface->m_SurfaceForm;
		//LogicalEnum u_closed = ifc_bspline_surface->m_UClosed;
		//LogicalEnum v_closed = ifc_bspline_surface->m_VClosed;
		//LogicalEnum	self_intersect = ifc_bspline_surface->m_SelfIntersect;

		shared_ptr<IfcBSplineSurfaceWithKnots> bspline_surface_with_knots = dynamic_pointer_cast<IfcBSplineSurfaceWithKnots>( ifc_bspline_surface );
		if( bspline_surface_with_knots )
		{
			//std::vector<int >& u_mult = bspline_surface_with_knots->m_UMultiplicities;
			//std::vector<int >& v_mult = bspline_surface_with_knots->m_VMultiplicities;
			//std::vector<shared_ptr<IfcParameterValue> >& ifc_u_knots = bspline_surface_with_knots->m_UKnots;
			//std::vector<shared_ptr<IfcParameterValue> >& ifc_v_knots = bspline_surface_with_knots->m_VKnots;

			//shared_ptr<IfcKnotType>& knot_spec = bspline_surface_with_knots->m_KnotSpec;


			shared_ptr<IfcRationalBSplineSurfaceWithKnots> r_bspline_surface_with_knots = dynamic_pointer_cast<IfcRationalBSplineSurfaceWithKnots>( ifc_bspline_surface );
			if( r_bspline_surface_with_knots )
			{
				//std::vector<std::vector<double > >& vec_weights = r_bspline_surface_with_knots->m_WeightsData;

			}
		}

		std::vector<vec3> control_point_array;

		m_point_converter->convertIfcCartesianPointVector2D( ifc_control_points, control_point_array );

		//unsigned int numPathU = 10;
		//unsigned int numPathV = 10;

		const size_t eta = ifc_control_points.size();
		if( eta < 2 )
		{
			return;
		}
		//const size_t zeta = ifc_control_points[0].size();

		//const int num_points_per_section = eta*zeta;
		// TODO: implement
#ifdef _DEBUG
		std::cout << "IfcBSplineSurface not yet implemented" << std::endl;
#endif
	}
};
