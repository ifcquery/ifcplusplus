/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
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

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcSIUnitName.h"

// TYPE IfcSIUnitName = ENUMERATION OF	(AMPERE	,BECQUEREL	,CANDELA	,COULOMB	,CUBIC_METRE	,DEGREE_CELSIUS	,FARAD	,GRAM	,GRAY	,HENRY	,HERTZ	,JOULE	,KELVIN	,LUMEN	,LUX	,METRE	,MOLE	,NEWTON	,OHM	,PASCAL	,RADIAN	,SECOND	,SIEMENS	,SIEVERT	,SQUARE_METRE	,STERADIAN	,TESLA	,VOLT	,WATT	,WEBER);
IfcSIUnitName::IfcSIUnitName() {}
IfcSIUnitName::~IfcSIUnitName() {}
shared_ptr<IfcPPObject> IfcSIUnitName::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSIUnitName> copy_self( new IfcSIUnitName() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSIUnitName::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSIUNITNAME("; }
	if( m_enum == ENUM_AMPERE )
	{
		stream << ".AMPERE.";
	}
	else if( m_enum == ENUM_BECQUEREL )
	{
		stream << ".BECQUEREL.";
	}
	else if( m_enum == ENUM_CANDELA )
	{
		stream << ".CANDELA.";
	}
	else if( m_enum == ENUM_COULOMB )
	{
		stream << ".COULOMB.";
	}
	else if( m_enum == ENUM_CUBIC_METRE )
	{
		stream << ".CUBIC_METRE.";
	}
	else if( m_enum == ENUM_DEGREE_CELSIUS )
	{
		stream << ".DEGREE_CELSIUS.";
	}
	else if( m_enum == ENUM_FARAD )
	{
		stream << ".FARAD.";
	}
	else if( m_enum == ENUM_GRAM )
	{
		stream << ".GRAM.";
	}
	else if( m_enum == ENUM_GRAY )
	{
		stream << ".GRAY.";
	}
	else if( m_enum == ENUM_HENRY )
	{
		stream << ".HENRY.";
	}
	else if( m_enum == ENUM_HERTZ )
	{
		stream << ".HERTZ.";
	}
	else if( m_enum == ENUM_JOULE )
	{
		stream << ".JOULE.";
	}
	else if( m_enum == ENUM_KELVIN )
	{
		stream << ".KELVIN.";
	}
	else if( m_enum == ENUM_LUMEN )
	{
		stream << ".LUMEN.";
	}
	else if( m_enum == ENUM_LUX )
	{
		stream << ".LUX.";
	}
	else if( m_enum == ENUM_METRE )
	{
		stream << ".METRE.";
	}
	else if( m_enum == ENUM_MOLE )
	{
		stream << ".MOLE.";
	}
	else if( m_enum == ENUM_NEWTON )
	{
		stream << ".NEWTON.";
	}
	else if( m_enum == ENUM_OHM )
	{
		stream << ".OHM.";
	}
	else if( m_enum == ENUM_PASCAL )
	{
		stream << ".PASCAL.";
	}
	else if( m_enum == ENUM_RADIAN )
	{
		stream << ".RADIAN.";
	}
	else if( m_enum == ENUM_SECOND )
	{
		stream << ".SECOND.";
	}
	else if( m_enum == ENUM_SIEMENS )
	{
		stream << ".SIEMENS.";
	}
	else if( m_enum == ENUM_SIEVERT )
	{
		stream << ".SIEVERT.";
	}
	else if( m_enum == ENUM_SQUARE_METRE )
	{
		stream << ".SQUARE_METRE.";
	}
	else if( m_enum == ENUM_STERADIAN )
	{
		stream << ".STERADIAN.";
	}
	else if( m_enum == ENUM_TESLA )
	{
		stream << ".TESLA.";
	}
	else if( m_enum == ENUM_VOLT )
	{
		stream << ".VOLT.";
	}
	else if( m_enum == ENUM_WATT )
	{
		stream << ".WATT.";
	}
	else if( m_enum == ENUM_WEBER )
	{
		stream << ".WEBER.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSIUnitName> IfcSIUnitName::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSIUnitName>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSIUnitName>(); }
	shared_ptr<IfcSIUnitName> type_object( new IfcSIUnitName() );
	if( boost::iequals( arg, L".AMPERE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_AMPERE;
	}
	else if( boost::iequals( arg, L".BECQUEREL." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_BECQUEREL;
	}
	else if( boost::iequals( arg, L".CANDELA." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_CANDELA;
	}
	else if( boost::iequals( arg, L".COULOMB." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_COULOMB;
	}
	else if( boost::iequals( arg, L".CUBIC_METRE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_CUBIC_METRE;
	}
	else if( boost::iequals( arg, L".DEGREE_CELSIUS." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_DEGREE_CELSIUS;
	}
	else if( boost::iequals( arg, L".FARAD." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_FARAD;
	}
	else if( boost::iequals( arg, L".GRAM." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_GRAM;
	}
	else if( boost::iequals( arg, L".GRAY." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_GRAY;
	}
	else if( boost::iequals( arg, L".HENRY." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_HENRY;
	}
	else if( boost::iequals( arg, L".HERTZ." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_HERTZ;
	}
	else if( boost::iequals( arg, L".JOULE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_JOULE;
	}
	else if( boost::iequals( arg, L".KELVIN." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_KELVIN;
	}
	else if( boost::iequals( arg, L".LUMEN." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_LUMEN;
	}
	else if( boost::iequals( arg, L".LUX." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_LUX;
	}
	else if( boost::iequals( arg, L".METRE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_METRE;
	}
	else if( boost::iequals( arg, L".MOLE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_MOLE;
	}
	else if( boost::iequals( arg, L".NEWTON." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_NEWTON;
	}
	else if( boost::iequals( arg, L".OHM." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_OHM;
	}
	else if( boost::iequals( arg, L".PASCAL." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_PASCAL;
	}
	else if( boost::iequals( arg, L".RADIAN." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_RADIAN;
	}
	else if( boost::iequals( arg, L".SECOND." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_SECOND;
	}
	else if( boost::iequals( arg, L".SIEMENS." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_SIEMENS;
	}
	else if( boost::iequals( arg, L".SIEVERT." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_SIEVERT;
	}
	else if( boost::iequals( arg, L".SQUARE_METRE." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_SQUARE_METRE;
	}
	else if( boost::iequals( arg, L".STERADIAN." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_STERADIAN;
	}
	else if( boost::iequals( arg, L".TESLA." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_TESLA;
	}
	else if( boost::iequals( arg, L".VOLT." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_VOLT;
	}
	else if( boost::iequals( arg, L".WATT." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_WATT;
	}
	else if( boost::iequals( arg, L".WEBER." ) )
	{
		type_object->m_enum = IfcSIUnitName::ENUM_WEBER;
	}
	return type_object;
}
