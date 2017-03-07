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

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcSpatialZoneTypeEnum.h"

// TYPE IfcSpatialZoneTypeEnum = ENUMERATION OF	(CONSTRUCTION	,FIRESAFETY	,LIGHTING	,OCCUPANCY	,SECURITY	,THERMAL	,TRANSPORT	,VENTILATION	,USERDEFINED	,NOTDEFINED);
IfcSpatialZoneTypeEnum::IfcSpatialZoneTypeEnum() {}
IfcSpatialZoneTypeEnum::~IfcSpatialZoneTypeEnum() {}
shared_ptr<IfcPPObject> IfcSpatialZoneTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSpatialZoneTypeEnum> copy_self( new IfcSpatialZoneTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpatialZoneTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPATIALZONETYPEENUM("; }
	if( m_enum == ENUM_CONSTRUCTION )
	{
		stream << ".CONSTRUCTION.";
	}
	else if( m_enum == ENUM_FIRESAFETY )
	{
		stream << ".FIRESAFETY.";
	}
	else if( m_enum == ENUM_LIGHTING )
	{
		stream << ".LIGHTING.";
	}
	else if( m_enum == ENUM_OCCUPANCY )
	{
		stream << ".OCCUPANCY.";
	}
	else if( m_enum == ENUM_SECURITY )
	{
		stream << ".SECURITY.";
	}
	else if( m_enum == ENUM_THERMAL )
	{
		stream << ".THERMAL.";
	}
	else if( m_enum == ENUM_TRANSPORT )
	{
		stream << ".TRANSPORT.";
	}
	else if( m_enum == ENUM_VENTILATION )
	{
		stream << ".VENTILATION.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSpatialZoneTypeEnum> IfcSpatialZoneTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpatialZoneTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpatialZoneTypeEnum>(); }
	shared_ptr<IfcSpatialZoneTypeEnum> type_object( new IfcSpatialZoneTypeEnum() );
	if( boost::iequals( arg, L".CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".FIRESAFETY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_FIRESAFETY;
	}
	else if( boost::iequals( arg, L".LIGHTING." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_LIGHTING;
	}
	else if( boost::iequals( arg, L".OCCUPANCY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_OCCUPANCY;
	}
	else if( boost::iequals( arg, L".SECURITY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_SECURITY;
	}
	else if( boost::iequals( arg, L".THERMAL." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_THERMAL;
	}
	else if( boost::iequals( arg, L".TRANSPORT." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_TRANSPORT;
	}
	else if( boost::iequals( arg, L".VENTILATION." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_VENTILATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
