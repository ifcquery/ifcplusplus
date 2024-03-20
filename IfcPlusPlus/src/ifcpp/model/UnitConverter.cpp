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
#include <IfcConversionBasedUnit.h>
#include <IfcLabel.h>
#include <IfcLengthMeasure.h>
#include <IfcMeasureWithUnit.h>
#include <IfcPlaneAngleMeasure.h>
#include <IfcProject.h>
#include <IfcRatioMeasure.h>
#include <IfcUnit.h>
#include <IfcSIUnit.h>
#include <IfcSIUnitName.h>
#include <IfcSIPrefix.h>
#include <IfcUnitAssignment.h>
#include <IfcUnitEnum.h>
#include <IfcValue.h>

#include "ifcpp/reader/ReaderUtil.h"
#include "UnitConverter.h"

using namespace IFC4X3;

UnitConverter::UnitConverter()
{
	resetComplete();
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
		m_plane_angle_factor = M_PI/180.0; // 360 degrees
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
	m_loaded_units.clear();
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
		m_plane_angle_factor = M_PI / 180.0; // 360 degrees
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

std::string UnitConverter::getUnitLabel(IFC4X3::IfcUnitEnum::IfcUnitEnumEnum unitEnum)
{
	double unit_factor = 1.0;
	std::string unit_label = "";

	auto itFind = m_loaded_units.find(unitEnum);
	if( itFind != m_loaded_units.end() )
	{
		shared_ptr<IfcSIUnit> si_unit = itFind->second;
		if( si_unit )
		{
			if( si_unit->m_Prefix )
			{
				switch( si_unit->m_Prefix->m_enum )
				{
				case IfcSIPrefix::ENUM_EXA:
				{
					unit_factor = 1E18;
					unit_label = "exa_";
					break;
				}
				case IfcSIPrefix::ENUM_PETA:
				{
					unit_factor = 1E15;
					unit_label = "peta_";
					break;
				}
				case IfcSIPrefix::ENUM_TERA:
				{
					unit_factor = 1E12;
					unit_label = "tera_";
					break;
				}
				case IfcSIPrefix::ENUM_GIGA:
				{
					unit_factor = 1E9;
					unit_label = "giga_";
					break;
				}
				case IfcSIPrefix::ENUM_MEGA:
				{
					unit_factor = 1E6;
					unit_label = "mega_";
					break;
				}
				case IfcSIPrefix::ENUM_KILO:
				{
					unit_factor = 1E3;
					unit_label = "kilo_";
					break;
				}
				case IfcSIPrefix::ENUM_HECTO:
				{
					unit_factor = 1E2;
					unit_label = "hecto_m";
					break;
				}
				case IfcSIPrefix::ENUM_DECA:
				{
					unit_factor = 1E1;
					unit_label = "";
					break;
				}
				case IfcSIPrefix::ENUM_DECI:
				{
					unit_factor = 1E-1;
					unit_label = "deci_";
					break;
				}
				case IfcSIPrefix::ENUM_CENTI:
				{
					unit_factor = 1E-2;
					unit_label = "centi_";
					break;
				}
				case IfcSIPrefix::ENUM_MILLI:
				{
					unit_factor = 1E-3;
					unit_label = "milli_";
					break;
				}
				case IfcSIPrefix::ENUM_MICRO:
				{
					unit_factor = 1E-6;
					unit_label = "micro_";
					break;
				}
				case IfcSIPrefix::ENUM_NANO:
				{
					unit_factor = 1E-9;
					unit_label = "nano_";
					break;
				}
				case IfcSIPrefix::ENUM_PICO:
				{
					unit_factor = 1E-12;
					unit_label = "pico_";
					break;
				}
				case IfcSIPrefix::ENUM_FEMTO:
				{
					unit_factor = 1E-15;
					unit_label = "femto_";
					break;
				}
				case IfcSIPrefix::ENUM_ATTO:
				{
					unit_factor = 1E-18;
					unit_label = "atto_";
					break;
				}
				}
			}

			shared_ptr<IfcUnitEnum> unit_type = si_unit->m_UnitType;
			if( unit_type )
			{
				if( unit_type->m_enum == IfcUnitEnum::ENUM_LENGTHUNIT )
				{
					unit_label += "m";

					if( si_unit->m_Prefix )
					{
						if( si_unit->m_Prefix->m_enum == IfcSIPrefix::ENUM_MILLI )
						{
							unit_label = "mm";
						}
					}
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_AREAUNIT )
				{
					unit_label += "m2";
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_VOLUMEUNIT )
				{
					unit_label += "m3";
				}
				else if( unit_type->m_enum == IfcUnitEnum::ENUM_MASSUNIT )
				{
					unit_label += "gram";
				}
			}
		}
	}
	return unit_label;
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
				m_loaded_units[unit_type->m_enum] = si_unit;
				if( unit_type->m_enum == IfcUnitEnum::ENUM_LENGTHUNIT )
				{
					if( unit_name )
					{
						if( unit_name->m_enum == IfcSIUnitName::ENUM_METRE )
						{
							m_length_unit_factor = 1.0;
							m_length_unit_found = true;
							//unit_label = "m";
						}
					}

					if( si_unit->m_Prefix )
					{
						m_loaded_prefix = si_unit->m_Prefix;
						m_length_unit_found = true;

						switch( m_loaded_prefix->m_enum )
						{
						case IfcSIPrefix::ENUM_EXA:
						{
							m_length_unit_factor = 1E18;
							break;
						}
						case IfcSIPrefix::ENUM_PETA:
						{
							m_length_unit_factor = 1E15;
							break;
						}
						case IfcSIPrefix::ENUM_TERA:
						{
							m_length_unit_factor = 1E12;
							break;
						}
						case IfcSIPrefix::ENUM_GIGA:
						{
							m_length_unit_factor = 1E9;
							break;
						}
						case IfcSIPrefix::ENUM_MEGA:
						{
							m_length_unit_factor = 1E6;
							break;
						}
						case IfcSIPrefix::ENUM_KILO:
						{
							m_length_unit_factor = 1E3;
							break;
						}
						case IfcSIPrefix::ENUM_HECTO:
						{
							m_length_unit_factor = 1E2;
							break;
						}
						case IfcSIPrefix::ENUM_DECA:
						{
							m_length_unit_factor = 1E1;
							break;
						}
						case IfcSIPrefix::ENUM_DECI:
						{
							m_length_unit_factor = 1E-1;
							break;
						}
						case IfcSIPrefix::ENUM_CENTI:
						{
							m_length_unit_factor = 1E-2;
							break;
						}
						case IfcSIPrefix::ENUM_MILLI:
						{
							m_length_unit_factor = 1E-3;
							break;
						}
						case IfcSIPrefix::ENUM_MICRO:
						{
							m_length_unit_factor = 1E-6;
							break;
						}
						case IfcSIPrefix::ENUM_NANO:
						{
							m_length_unit_factor = 1E-9;
							break;
						}
						case IfcSIPrefix::ENUM_PICO:
						{
							m_length_unit_factor = 1E-12;
							break;
						}
						case IfcSIPrefix::ENUM_FEMTO:
						{
							m_length_unit_factor = 1E-15;
							break;
						}
						case IfcSIPrefix::ENUM_ATTO:
						{
							m_length_unit_factor = 1E-18;
							break;
						}
						
						default:
							m_length_unit_found = false;
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
										//typedef double IfcRatioMeasure;

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

								if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), "INCH" ) )
								{
									m_length_unit_factor = 25.4*0.001; // 1 unit is 25.4 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), "FOOT" ) )
								{
									m_length_unit_factor = 304.8*0.001; // 1 unit is 304.8 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), "YARD" ) )
								{
									m_length_unit_factor = 914*0.001; // 1 unit is 914 mm
									m_length_unit_found = true;
								}
								else if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), "MILE" ) )
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
										if( std_iequal( conversion_based_unit->m_Name->m_value.c_str(), "DEGREE" ) )
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
