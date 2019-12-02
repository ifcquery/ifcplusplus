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

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include "ifcpp/IFC4/include/IfcConversionBasedUnit.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcMeasureWithUnit.h"
#include "ifcpp/IFC4/include/IfcPlaneAngleMeasure.h"
#include "ifcpp/IFC4/include/IfcProject.h"
#include "ifcpp/IFC4/include/IfcRatioMeasure.h"
#include "ifcpp/IFC4/include/IfcUnit.h"
#include "ifcpp/IFC4/include/IfcSIUnit.h"
#include "ifcpp/IFC4/include/IfcSIUnitName.h"
#include "ifcpp/IFC4/include/IfcSIPrefix.h"
#include "ifcpp/IFC4/include/IfcUnitAssignment.h"
#include "ifcpp/IFC4/include/IfcUnitEnum.h"
#include "ifcpp/IFC4/include/IfcValue.h"

#include "ifcpp/reader/ReaderUtil.h"
#include "UnitConverter.h"

UnitConverter::UnitConverter()
{
	resetComplete();
	m_prefix_map[IfcSIPrefix::ENUM_EXA]		= 1E18;
	m_prefix_map[IfcSIPrefix::ENUM_PETA]	= 1E15;
	m_prefix_map[IfcSIPrefix::ENUM_TERA]	= 1E12;
	m_prefix_map[IfcSIPrefix::ENUM_GIGA]	= 1E9;
	m_prefix_map[IfcSIPrefix::ENUM_MEGA]	= 1E6;
	m_prefix_map[IfcSIPrefix::ENUM_KILO]	= 1E3;
	m_prefix_map[IfcSIPrefix::ENUM_HECTO]	= 1E2;
	m_prefix_map[IfcSIPrefix::ENUM_DECA]	= 1E1;
	m_prefix_map[IfcSIPrefix::ENUM_DECI]	= 1E-1;
	m_prefix_map[IfcSIPrefix::ENUM_CENTI]	= 1E-2;
	m_prefix_map[IfcSIPrefix::ENUM_MILLI]	= 1E-3;
	m_prefix_map[IfcSIPrefix::ENUM_MICRO]	= 1E-6;
	m_prefix_map[IfcSIPrefix::ENUM_NANO]	= 1E-9;
	m_prefix_map[IfcSIPrefix::ENUM_PICO]	= 1E-12;
	m_prefix_map[IfcSIPrefix::ENUM_FEMTO]	= 1E-15;
	m_prefix_map[IfcSIPrefix::ENUM_ATTO]	= 1E-18;
	m_plane_angle_factor = M_PI/180.0;
}

void UnitConverter::resetUnitFactors()
{
	m_loaded_prefix.reset();
	m_length_unit_factor = 1.0;
	m_plane_angle_factor = M_PI / 180.0;
	if( m_angular_unit == RADIANT )
	{
		m_plane_angle_factor = 1.0; // radian
	}
	else if( m_angular_unit == DEGREE )
	{
		m_plane_angle_factor = M_PI/180.0; // 360°
	}
	else if( m_angular_unit == GON )
	{
		m_plane_angle_factor = M_PI/200.0; // 400 gon
	}
}
void UnitConverter::resetComplete()
{
	m_angular_unit = UNDEFINED;
	m_length_unit_found = false;
	resetUnitFactors();
}

void UnitConverter::setAngleUnit(AngularUnit unit)
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
		messageCallback("Could not set angular unit", StatusCallback::MESSAGE_TYPE_WARNING, __FUNC__);
	}
}

void UnitConverter::setIfcProject( shared_ptr<IfcProject> project )
{
	resetComplete();

	if( !project->m_UnitsInContext )
	{
		messageCallback( "IfcProject.UnitsInContext not defined", StatusCallback::MESSAGE_TYPE_WARNING, "" );
		return;
	}

	shared_ptr<IfcUnitAssignment> unit_assignment = project->m_UnitsInContext;
	std::vector<shared_ptr<IfcUnit> >& vec_units = unit_assignment->m_Units;
	for(auto unit : vec_units)
	{
			if( !unit )
		{
			continue;
		}
		shared_ptr<IfcSIUnit> si_unit = dynamic_pointer_cast<IfcSIUnit>( unit );
		if( si_unit )
		{
			//	example: #10 = IFCSIUNIT( *, .LENGTHUNIT., $, .METRE. );
			//  shared_ptr<IfcDimensionalExponents>	m_Dimensions;
			//  shared_ptr<IfcUnitEnum>				m_UnitType;
			//	shared_ptr<IfcSIPrefix>				m_Prefix;					//optional
			//	shared_ptr<IfcSIUnitName>			m_Name;

			shared_ptr<IfcUnitEnum> unit_type = si_unit->m_UnitType;
			shared_ptr<IfcSIUnitName> unit_name = si_unit->m_Name;

			if( unit_type )
			{
				if( unit_type->m_enum == IfcUnitEnum::ENUM_LENGTHUNIT )
				{
					if( unit_name )
					{
						if( unit_name->m_enum == IfcSIUnitName::ENUM_METRE )
						{
							m_length_unit_factor = 1.0;
							m_length_unit_found = true;
						}
					}

					if( si_unit->m_Prefix )
					{
						m_loaded_prefix = si_unit->m_Prefix;
						if( m_prefix_map.find( si_unit->m_Prefix->m_enum ) != m_prefix_map.end() )
						{
							m_length_unit_factor = m_prefix_map[si_unit->m_Prefix->m_enum];
							m_length_unit_found = true;
						}
					}
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_AREAUNIT )
				{
					// TODO: implement
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_VOLUMEUNIT )
				{
					// TODO: implement
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_PLANEANGLEUNIT )
				{
					if( unit_name )
					{
						if( unit_name->m_enum == IfcSIUnitName::ENUM_RADIAN )
						{
							m_angular_unit = RADIANT;
							m_plane_angle_factor = 1.0;
						}
					}
				}
			}
			continue;
		}


		shared_ptr<IfcConversionBasedUnit> conversion_based_unit = dynamic_pointer_cast<IfcConversionBasedUnit>( unit );
		if( conversion_based_unit )
		{
			shared_ptr<IfcMeasureWithUnit> conversion_factor = conversion_based_unit->m_ConversionFactor;
			if( conversion_factor )
			{
				shared_ptr<IfcUnit> unit_component = conversion_factor->m_UnitComponent;
				if( unit_component )
				{
					shared_ptr<IfcSIUnit> unit_component_si = dynamic_pointer_cast<IfcSIUnit>( unit_component );
					if( unit_component_si )
					{
						shared_ptr<IfcUnitEnum> type = unit_component_si->m_UnitType;
						if( type )
						{
							if( type->m_enum == IfcUnitEnum::ENUM_LENGTHUNIT )
							{
								shared_ptr<IfcValue> length_value_select = conversion_factor->m_ValueComponent;
								if( length_value_select )
								{
									shared_ptr<IfcRatioMeasure> ratio_measure = dynamic_pointer_cast<IfcRatioMeasure>( length_value_select );
									if( ratio_measure )
									{
										m_length_unit_factor = ratio_measure->m_value;
										m_length_unit_found = true;
									}
									shared_ptr<IfcLengthMeasure> length_measure = dynamic_pointer_cast<IfcLengthMeasure>( length_value_select );
									if( length_measure )
									{
										m_length_unit_factor = length_measure->m_value;
										m_length_unit_found = true;
									}
								}

								if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), L"INCH" ) )
								{
									m_length_unit_factor = 25.4*0.001; // 1 unit is 25.4 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), L"FOOT" ) )
								{
									m_length_unit_factor = 304.8*0.001; // 1 unit is 304.8 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), L"YARD" ) )
								{
									m_length_unit_factor = 914*0.001; // 1 unit is 914 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), L"MILE" ) )
								{
									m_length_unit_factor = 1609*0.001; // 1 unit is 1609 mm
									m_length_unit_found = true;
								}
								
							}
							else if( type->m_enum == IfcUnitEnum::ENUM_PLANEANGLEUNIT )
							{
								shared_ptr<IfcValue> plane_angle_value = conversion_factor->m_ValueComponent;
								if( plane_angle_value )
								{
									if( dynamic_pointer_cast<IfcPlaneAngleMeasure>( plane_angle_value ) )
									{
										shared_ptr<IfcPlaneAngleMeasure> plane_angle_measure = dynamic_pointer_cast<IfcPlaneAngleMeasure>( plane_angle_value );
										m_plane_angle_factor = plane_angle_measure->m_value;
										m_angular_unit = CONVERSION_BASED;
									}
									else if( dynamic_pointer_cast<IfcRatioMeasure>( plane_angle_value ) )
									{
										shared_ptr<IfcRatioMeasure> plane_angle_measure = dynamic_pointer_cast<IfcRatioMeasure>( plane_angle_value );
										m_plane_angle_factor = plane_angle_measure->m_value;
										m_angular_unit = CONVERSION_BASED;
									}
									else if( conversion_based_unit->m_Name )
									{
										if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), L"DEGREE" ) )
										{
											m_angular_unit = DEGREE;
											m_plane_angle_factor = M_PI / 180.0;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
