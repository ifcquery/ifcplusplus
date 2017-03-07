/* -*-c++-*- IFC++ www.ifcquery.com
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

#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include "IfcPPBasicTypes.h"
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
