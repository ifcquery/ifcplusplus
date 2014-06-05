/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#include <ifcpp/IFC4/include/IfcSectionedSpine.h>
#include <ifcpp/IFC4/include/IfcCompositeCurve.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineCurveWithKnots.h>
#include <ifcpp/IFC4/include/IfcProfileDef.h>

#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcArbitraryClosedProfileDef.h>
#include <ifcpp/IFC4/include/IfcPolyline.h>
#include <ifcpp/IFC4/include/IfcParameterValue.h>
#include <ifcpp/IFC4/include/IfcCartesianPoint.h>
#include <ifcpp/IFC4/include/IfcLengthMeasure.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcArbitraryProfileDefWithVoids.h>
#include <ifcpp/IFC4/include/IfcRationalBSplineSurfaceWithKnots.h>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/model/shared_ptr.h>
#include "ProfileConverter.h"
#include "CurveConverter.h"
#include "RepresentationConverter.h"

void RepresentationConverter::convertIfcSectionedSpine( const shared_ptr<IfcSectionedSpine>& spine, shared_ptr<ItemData> item_data, std::stringstream& strs_err )
{
	const shared_ptr<IfcCompositeCurve> spine_curve = spine->m_SpineCurve;
	if( !spine_curve )
	{
		return;
	}
	const std::vector<shared_ptr<IfcProfileDef> >& vec_cross_sections = spine->m_CrossSections;
	const std::vector<shared_ptr<IfcAxis2Placement3D> >& vec_cross_section_positions = spine->m_CrossSectionPositions;

	std::vector<shared_ptr<IfcProfileDef> >::iterator it_cross_sections;

	unsigned int num_cross_sections = vec_cross_sections.size();
	if( vec_cross_section_positions.size() < num_cross_sections )
	{
		num_cross_sections = vec_cross_section_positions.size();
	}

	std::vector<shared_ptr<IfcCompositeCurveSegment> > segements = spine_curve->m_Segments;
	int num_segments = segements.size();
	if( vec_cross_section_positions.size() < num_segments+1 )
	{
		num_segments = vec_cross_section_positions.size()-1;
	}

	std::vector<carve::geom::vector<3> > curve_polygon;
	std::vector<carve::geom::vector<3> > segment_start_points;
	//CurveConverter cconv( m_unit_converter );
	m_curve_converter->convertIfcCurve( spine_curve, curve_polygon, segment_start_points );

	std::cout << "IfcSectionedSpine not implemented." << std::endl;
}
