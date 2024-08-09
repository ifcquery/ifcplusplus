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

#include <IfcBSplineCurve.h>
#include <IfcBSplineSurface.h>
#include <IfcInteger.h>
#include <IfcParameterValue.h>
#include <IfcRationalBSplineCurveWithKnots.h>
#include <IfcRationalBSplineSurfaceWithKnots.h>
#include <IfcReal.h>

#include "PointConverter.h"
#include "IncludeCarveHeaders.h"

class SplineConverter : public StatusCallback
{
protected:
	shared_ptr<GeometrySettings> m_geom_settings;
	shared_ptr<PointConverter> m_point_converter;

public:
	static void computeKnotVector( const size_t numControlPoints, const size_t order, std::vector<double>& knotVector )
	{
		const size_t n_plus_order = numControlPoints + order;
		const size_t n_plus_1 = numControlPoints + 1;

		// example: order=2, numControlPoints=4,  n_plus_1=5,  n_plus_c=6
		//                       order                            nplus1    nplusc
		//  knot[0]   knot[1]    knot[2]    knot[3]    knot[4]    knot[5]   knot[6]
		//  0         0          1          2          3          4         4

		knotVector[0] = 0;  // start the knot vector with 0
		for( size_t ii = 1; ii < n_plus_order; ++ii )
		{
			if( ( ii >= order ) && ( ii < n_plus_1 ) )
			{
				knotVector[ii] = knotVector[ii - 1] + 1.0;
			}
			else
			{
				knotVector[ii] = knotVector[ii - 1];
			}
		}
	}

	static void computRationalBasisFunctions( const size_t order, const double t, const size_t numControlPoints, const std::vector<double>& knotVec,
		std::vector<double>& weights, std::vector<double>& basisFunc )
	{
		const size_t n_plus_order = numControlPoints + order;  // maximum number of knot values

		// first order nonrational basis function
		std::vector<double> temp;
		temp.resize( n_plus_order + 1 );
		for( size_t ii = 0; ii < n_plus_order - 1; ii++ )
		{
			if( ( t >= knotVec[ii] ) && ( t < knotVec[ii + 1] ) )
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
					basis_func_part1 = ( ( t - knotVec[ii] )*temp[ii] ) / ( knotVec[ii + kk - 1] - knotVec[ii] );
				}
				else
				{
					basis_func_part1 = 0;
				}

				// skip if the lower order basis function is zero
				if( temp[ii + 1] != 0 )
				{
					basis_func_part2 = ( ( knotVec[ii + kk] - t )*temp[ii + 1] ) / ( knotVec[ii + kk] - knotVec[ii + 1] );
				}
				else
				{
					basis_func_part2 = 0;
				}

				temp[ii] = basis_func_part1 + basis_func_part2;
			}
		}

		if( t == knotVec[n_plus_order - 1] )
		{
			// pick up last point
			temp[numControlPoints - 1] = 1;
		}

		if( weights.size() < numControlPoints+1 )
		{
			double resizeValue = 1.0;
			weights.resize(numControlPoints+1, resizeValue);
		}

		// compute sum for denominator of rational basis function
		double sum = 0.0;
		for( size_t ii = 0; ii < numControlPoints; ++ii )
		{
			sum = sum + temp[ii] * weights[ii + 1];
		}

		// compute the rational basis function
		for( size_t ii = 0; ii < numControlPoints; ii++ )
		{
			if( sum != 0.0 )
			{
				basisFunc[ii] = temp[ii] * weights[ii + 1] / sum;
			}
			else
			{
				basisFunc[ii] = 0;
			}
		}
	}

	static void computeRationalBSpline( const size_t order, const size_t numCurvePoints, const std::vector<vec3>& controlPoints, std::vector<double>& weights,
		std::vector<double>& knotVec, std::vector<double>& curvePoints )
	{
		if( knotVec.size() < 2)
		{
			return;
		}
		// order: order of the BSpline basis function
		std::vector<double> basis_func;  // basis function for parameter value t
		basis_func.resize( controlPoints.size() + 1, 0.0 );

		const size_t n_plus_order = controlPoints.size() + order; // number of knot values
		if( knotVec.size() != n_plus_order )
		{
			// generate a uniform open knot vector
			knotVec.resize( n_plus_order, 0.0 );
			computeKnotVector( controlPoints.size(), order, knotVec );
		}

		double t = knotVec[0]; // parameter value 0 <= t <= npts - k + 1
		double step = (knotVec[knotVec.size() - 1] - knotVec[0])/ ( (double)( numCurvePoints - 1 ) );

		std::vector<double> control_points_coords;
		for( size_t ii = 0; ii < controlPoints.size(); ++ii )
		{
			control_points_coords.push_back( controlPoints[ii].x );
			control_points_coords.push_back( controlPoints[ii].y );
			control_points_coords.push_back( controlPoints[ii].z );
		}

		size_t offset_i = 0;
		double temp;
		for( size_t ii = 0; ii < numCurvePoints; ++ii )
		{
			if( t > knotVec[knotVec.size() - 1] - 0.000001 )
			{
				t = knotVec[knotVec.size() - 1];
			}

			// generate the basis function for this value of t
			computRationalBasisFunctions( order, t, controlPoints.size(), knotVec, weights, basis_func );

			for( size_t jj = 0; jj < 3; ++jj )
			{
				curvePoints[offset_i + jj] = 0.;

				for( size_t kk = 0; kk < controlPoints.size(); ++kk )
				{
					// matrix multiplication
					temp = basis_func[kk] * control_points_coords[jj + kk * 3];
					curvePoints[offset_i + jj] = curvePoints[offset_i + jj] + temp;
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
		
		const std::vector<shared_ptr<IfcCartesianPoint> >&	control_points_ifc = bspline_curve->m_ControlPointsList;
		std::vector<vec3> controlPoints;
		m_point_converter->convertIfcCartesianPointVector( control_points_ifc, controlPoints );

		if( controlPoints.size() < 2 )
		{
			return;
		}

		const size_t numControlPoints = controlPoints.size();
		const int	degree = bspline_curve->m_Degree->m_value;
		const size_t order = degree + 1; // the order of the curve is the degree of the resulting polynomial + 1
		const size_t numCurvePoints = numControlPoints * m_geom_settings->getNumVerticesPerControlPoint();
		std::vector<double> knotVector;

		//	set weighting factors to 1.0 in case of homogeneous curve
		std::vector<double> weights;
		weights.resize( numControlPoints + 1, 1.0 );

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
				
				int num_multiply_knot_value = 1;
				if( ifc_knot_mult.size() == ifc_knots.size() )
				{
					num_multiply_knot_value = ifc_knot_mult[ii]->m_value;
				}
				
				for( int jj = 0; jj < num_multiply_knot_value; ++jj )
				{
					knotVector.push_back( knot_value );
				}
			}

#ifdef _DEBUG
			std::stringstream controlPointsStringStream;
			for( size_t i_weight = 0; i_weight < controlPoints.size(); ++i_weight )
			{
				controlPointsStringStream << controlPoints[i_weight].x << "  " << controlPoints[i_weight].y << "  " << controlPoints[i_weight].z << "\n";
			}
			std::string controlPointsString = controlPointsStringStream.str();

			// check knot multiplicities and degree
			const size_t n_plus_order = controlPoints.size() + order; // number of knot values
			int n = numControlPoints - 1;
			int k = order;
			size_t numKnots = n + k + 1;
			if( numKnots != knotVector.size() )
			{
				knotVector.clear();  // valid knot vector will be computed in computeRationalBSpline
				std::cout << "invalid knot vector/KnotMultiplicities" << std::endl;
			}
#endif

			shared_ptr<IfcRationalBSplineCurveWithKnots> r_bspline_curve_with_knots = dynamic_pointer_cast<IfcRationalBSplineCurveWithKnots>( bspline_curve_with_knots );
			if( r_bspline_curve_with_knots )
			{
				std::vector<shared_ptr<IfcReal> >& ifc_vec_weigths = r_bspline_curve_with_knots->m_WeightsData;
				weights.resize( ifc_vec_weigths.size() );
				for( size_t i_weight = 0; i_weight < ifc_vec_weigths.size(); ++i_weight )
				{
					weights[i_weight] = ifc_vec_weigths[i_weight]->m_value;
				}
			}
		}

		std::vector<double> curvePointsCoords;
		curvePointsCoords.resize( 3 * numCurvePoints, 0.0 );
		computeRationalBSpline( order, numCurvePoints, controlPoints, weights, knotVector, curvePointsCoords );

		if( target_vec.size() > 2 )
		{
			segment_start_points.push_back( carve::geom::VECTOR( curvePointsCoords[0], curvePointsCoords[1], curvePointsCoords[2] ) );
		}

		for( size_t ii = 0; ii < 3 * numCurvePoints; ii = ii + 3 )
		{
			target_vec.push_back( carve::geom::VECTOR( curvePointsCoords[ii], curvePointsCoords[ii + 1], curvePointsCoords[ii + 2] ) );
		}
#ifdef _DEBUG
		vec4 color(0.5, 0.6, 0.7, 1.0);
		GeomDebugDump::dumpPolyline(target_vec, color, 0, false, false);
#endif
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
				//std::vector<std::vector<double > >& weights = r_bspline_surface_with_knots->m_WeightsData;

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
