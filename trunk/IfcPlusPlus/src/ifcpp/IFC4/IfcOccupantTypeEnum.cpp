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
#include "include/IfcOccupantTypeEnum.h"

// TYPE IfcOccupantTypeEnum = ENUMERATION OF	(ASSIGNEE	,ASSIGNOR	,LESSEE	,LESSOR	,LETTINGAGENT	,OWNER	,TENANT	,USERDEFINED	,NOTDEFINED);
IfcOccupantTypeEnum::IfcOccupantTypeEnum() {}
IfcOccupantTypeEnum::~IfcOccupantTypeEnum() {}
shared_ptr<IfcPPObject> IfcOccupantTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcOccupantTypeEnum> copy_self( new IfcOccupantTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcOccupantTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOCCUPANTTYPEENUM("; }
	if( m_enum == ENUM_ASSIGNEE )
	{
		stream << ".ASSIGNEE.";
	}
	else if( m_enum == ENUM_ASSIGNOR )
	{
		stream << ".ASSIGNOR.";
	}
	else if( m_enum == ENUM_LESSEE )
	{
		stream << ".LESSEE.";
	}
	else if( m_enum == ENUM_LESSOR )
	{
		stream << ".LESSOR.";
	}
	else if( m_enum == ENUM_LETTINGAGENT )
	{
		stream << ".LETTINGAGENT.";
	}
	else if( m_enum == ENUM_OWNER )
	{
		stream << ".OWNER.";
	}
	else if( m_enum == ENUM_TENANT )
	{
		stream << ".TENANT.";
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
shared_ptr<IfcOccupantTypeEnum> IfcOccupantTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcOccupantTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcOccupantTypeEnum>(); }
	shared_ptr<IfcOccupantTypeEnum> type_object( new IfcOccupantTypeEnum() );
	if( boost::iequals( arg, L".ASSIGNEE." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_ASSIGNEE;
	}
	else if( boost::iequals( arg, L".ASSIGNOR." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_ASSIGNOR;
	}
	else if( boost::iequals( arg, L".LESSEE." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LESSEE;
	}
	else if( boost::iequals( arg, L".LESSOR." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LESSOR;
	}
	else if( boost::iequals( arg, L".LETTINGAGENT." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LETTINGAGENT;
	}
	else if( boost::iequals( arg, L".OWNER." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_OWNER;
	}
	else if( boost::iequals( arg, L".TENANT." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_TENANT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
