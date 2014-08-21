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
#include "include/IfcLaborResourceTypeEnum.h"

// TYPE IfcLaborResourceTypeEnum = ENUMERATION OF	(ADMINISTRATION	,CARPENTRY	,CLEANING	,CONCRETE	,DRYWALL	,ELECTRIC	,FINISHING	,FLOORING	,GENERAL	,HVAC	,LANDSCAPING	,MASONRY	,PAINTING	,PAVING	,PLUMBING	,ROOFING	,SITEGRADING	,STEELWORK	,SURVEYING	,USERDEFINED	,NOTDEFINED);
IfcLaborResourceTypeEnum::IfcLaborResourceTypeEnum() {}
IfcLaborResourceTypeEnum::~IfcLaborResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcLaborResourceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLaborResourceTypeEnum> copy_self( new IfcLaborResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLaborResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLABORRESOURCETYPEENUM("; }
	if( m_enum == ENUM_ADMINISTRATION )
	{
		stream << ".ADMINISTRATION.";
	}
	else if( m_enum == ENUM_CARPENTRY )
	{
		stream << ".CARPENTRY.";
	}
	else if( m_enum == ENUM_CLEANING )
	{
		stream << ".CLEANING.";
	}
	else if( m_enum == ENUM_CONCRETE )
	{
		stream << ".CONCRETE.";
	}
	else if( m_enum == ENUM_DRYWALL )
	{
		stream << ".DRYWALL.";
	}
	else if( m_enum == ENUM_ELECTRIC )
	{
		stream << ".ELECTRIC.";
	}
	else if( m_enum == ENUM_FINISHING )
	{
		stream << ".FINISHING.";
	}
	else if( m_enum == ENUM_FLOORING )
	{
		stream << ".FLOORING.";
	}
	else if( m_enum == ENUM_GENERAL )
	{
		stream << ".GENERAL.";
	}
	else if( m_enum == ENUM_HVAC )
	{
		stream << ".HVAC.";
	}
	else if( m_enum == ENUM_LANDSCAPING )
	{
		stream << ".LANDSCAPING.";
	}
	else if( m_enum == ENUM_MASONRY )
	{
		stream << ".MASONRY.";
	}
	else if( m_enum == ENUM_PAINTING )
	{
		stream << ".PAINTING.";
	}
	else if( m_enum == ENUM_PAVING )
	{
		stream << ".PAVING.";
	}
	else if( m_enum == ENUM_PLUMBING )
	{
		stream << ".PLUMBING.";
	}
	else if( m_enum == ENUM_ROOFING )
	{
		stream << ".ROOFING.";
	}
	else if( m_enum == ENUM_SITEGRADING )
	{
		stream << ".SITEGRADING.";
	}
	else if( m_enum == ENUM_STEELWORK )
	{
		stream << ".STEELWORK.";
	}
	else if( m_enum == ENUM_SURVEYING )
	{
		stream << ".SURVEYING.";
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
shared_ptr<IfcLaborResourceTypeEnum> IfcLaborResourceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLaborResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLaborResourceTypeEnum>(); }
	shared_ptr<IfcLaborResourceTypeEnum> type_object( new IfcLaborResourceTypeEnum() );
	if( boost::iequals( arg, L".ADMINISTRATION." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ADMINISTRATION;
	}
	else if( boost::iequals( arg, L".CARPENTRY." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CARPENTRY;
	}
	else if( boost::iequals( arg, L".CLEANING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CLEANING;
	}
	else if( boost::iequals( arg, L".CONCRETE." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_CONCRETE;
	}
	else if( boost::iequals( arg, L".DRYWALL." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_DRYWALL;
	}
	else if( boost::iequals( arg, L".ELECTRIC." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ELECTRIC;
	}
	else if( boost::iequals( arg, L".FINISHING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_FINISHING;
	}
	else if( boost::iequals( arg, L".FLOORING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_FLOORING;
	}
	else if( boost::iequals( arg, L".GENERAL." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_GENERAL;
	}
	else if( boost::iequals( arg, L".HVAC." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_HVAC;
	}
	else if( boost::iequals( arg, L".LANDSCAPING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_LANDSCAPING;
	}
	else if( boost::iequals( arg, L".MASONRY." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_MASONRY;
	}
	else if( boost::iequals( arg, L".PAINTING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PAINTING;
	}
	else if( boost::iequals( arg, L".PAVING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PAVING;
	}
	else if( boost::iequals( arg, L".PLUMBING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_PLUMBING;
	}
	else if( boost::iequals( arg, L".ROOFING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_ROOFING;
	}
	else if( boost::iequals( arg, L".SITEGRADING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_SITEGRADING;
	}
	else if( boost::iequals( arg, L".STEELWORK." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_STEELWORK;
	}
	else if( boost::iequals( arg, L".SURVEYING." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_SURVEYING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcLaborResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
