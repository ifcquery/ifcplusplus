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

#pragma once

#include <map>
#include "shared_ptr.h"
#include "ifcpp/IFC4/include/IfcProject.h"
#include "ifcpp/IFC4/include/IfcSIPrefix.h"

//\brief class to convert values from different units into meter and radian

class UnitConverter
{
public:
	UnitConverter();
	~UnitConverter();
	void setIfcProject( shared_ptr<IfcProject> project);

	double getLengthInMeterFactor()
	{
		return m_length_unit_factor;
	}

	double getAngleInRadianFactor()
	{
		return m_plane_angle_factor;
	}
	shared_ptr<IfcSIPrefix>& getLoadedPrefix() { return m_loaded_prefix; }

	std::map<int, double> m_prefix_map;
	shared_ptr<IfcSIPrefix>	m_loaded_prefix;
	double m_length_unit_factor;
	double m_plane_angle_factor;
};
