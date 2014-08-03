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
#include "include/IfcDistributionChamberElementTypeEnum.h"

// TYPE IfcDistributionChamberElementTypeEnum 
IfcDistributionChamberElementTypeEnum::IfcDistributionChamberElementTypeEnum() {}
IfcDistributionChamberElementTypeEnum::~IfcDistributionChamberElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcDistributionChamberElementTypeEnum::getDeepCopy()
{
	shared_ptr<IfcDistributionChamberElementTypeEnum> copy_self( new IfcDistributionChamberElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDistributionChamberElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDISTRIBUTIONCHAMBERELEMENTTYPEENUM("; }
	if( m_enum == ENUM_FORMEDDUCT )
	{
		stream << ".FORMEDDUCT.";
	}
	else if( m_enum == ENUM_INSPECTIONCHAMBER )
	{
		stream << ".INSPECTIONCHAMBER.";
	}
	else if( m_enum == ENUM_INSPECTIONPIT )
	{
		stream << ".INSPECTIONPIT.";
	}
	else if( m_enum == ENUM_MANHOLE )
	{
		stream << ".MANHOLE.";
	}
	else if( m_enum == ENUM_METERCHAMBER )
	{
		stream << ".METERCHAMBER.";
	}
	else if( m_enum == ENUM_SUMP )
	{
		stream << ".SUMP.";
	}
	else if( m_enum == ENUM_TRENCH )
	{
		stream << ".TRENCH.";
	}
	else if( m_enum == ENUM_VALVECHAMBER )
	{
		stream << ".VALVECHAMBER.";
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
shared_ptr<IfcDistributionChamberElementTypeEnum> IfcDistributionChamberElementTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDistributionChamberElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDistributionChamberElementTypeEnum>(); }
	shared_ptr<IfcDistributionChamberElementTypeEnum> type_object( new IfcDistributionChamberElementTypeEnum() );
	if( boost::iequals( arg, L".FORMEDDUCT." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_FORMEDDUCT;
	}
	else if( boost::iequals( arg, L".INSPECTIONCHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_INSPECTIONCHAMBER;
	}
	else if( boost::iequals( arg, L".INSPECTIONPIT." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_INSPECTIONPIT;
	}
	else if( boost::iequals( arg, L".MANHOLE." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_MANHOLE;
	}
	else if( boost::iequals( arg, L".METERCHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_METERCHAMBER;
	}
	else if( boost::iequals( arg, L".SUMP." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_SUMP;
	}
	else if( boost::iequals( arg, L".TRENCH." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_TRENCH;
	}
	else if( boost::iequals( arg, L".VALVECHAMBER." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_VALVECHAMBER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDistributionChamberElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
