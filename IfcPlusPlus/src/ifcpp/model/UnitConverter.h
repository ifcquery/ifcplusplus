/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include "shared_ptr.h"
#include "StatusCallback.h"
#include "IfcPPException.h"
#include "ifcpp/IFC4/include/IfcProject.h"
#include "ifcpp/IFC4/include/IfcSIPrefix.h"
#include "ifcpp/IFC4/include/IfcSIUnitName.h"

//\brief class to convert values from different units into meter and radian
class UnitConverter : public StatusCallback
{
public:
	enum AngularUnit { UNDEFINED, RADIANT, DEGREE, GON, CONVERSION_BASED };
	UnitConverter();
	virtual ~UnitConverter();
	void setIfcProject( shared_ptr<IfcProject> project);
	void setLengthInMeterFactor( double factor )
	{
		m_length_unit_factor = factor;
		m_length_unit_found = true;
	}
	double getLengthInMeterFactor()
	{
		if( !m_length_unit_found )
		{
			messageCallback( "No length unit definition found in model", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__ );
		}

		return m_length_unit_factor;
	}
	void setAngleUnit( AngularUnit unit )
	{
		m_angular_unit = unit;
		if( m_angular_unit == RADIANT )
		{
			m_plane_angle_factor = 1.0; // radian
		}
		else if( m_angular_unit == DEGREE )
		{
			m_plane_angle_factor = M_PI / 180.0; // 360°
		}
		else if( m_angular_unit == GON )
		{
			m_plane_angle_factor = M_PI / 200.0; // 400 gon
		}
		else
		{
			messageCallback( "Could not set angular unit", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__ );
		}
	}
	double getAngleInRadiantFactor()
	{
		if( m_angular_unit == UNDEFINED )
		{
			messageCallback( "No plane angle unit definition found in model", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__ );
		}
		return m_plane_angle_factor;
	}
	AngularUnit getAngularUnit() { return m_angular_unit; }
	shared_ptr<IfcSIPrefix>& getLoadedPrefix() { return m_loaded_prefix; }

	void resetUnitFactors();
	void resetComplete();

protected:
	std::map<int, double> m_prefix_map;
	shared_ptr<IfcSIPrefix>	m_loaded_prefix;
	double m_length_unit_factor;
	bool m_length_unit_found;
	double m_plane_angle_factor;
	AngularUnit	m_angular_unit;
};
