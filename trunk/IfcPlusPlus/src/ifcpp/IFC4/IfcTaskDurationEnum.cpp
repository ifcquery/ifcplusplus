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
#include "include/IfcTaskDurationEnum.h"

// TYPE IfcTaskDurationEnum = ENUMERATION OF	(ELAPSEDTIME	,WORKTIME	,NOTDEFINED);
IfcTaskDurationEnum::IfcTaskDurationEnum() {}
IfcTaskDurationEnum::~IfcTaskDurationEnum() {}
shared_ptr<IfcPPObject> IfcTaskDurationEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTaskDurationEnum> copy_self( new IfcTaskDurationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTaskDurationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTASKDURATIONENUM("; }
	if( m_enum == ENUM_ELAPSEDTIME )
	{
		stream << ".ELAPSEDTIME.";
	}
	else if( m_enum == ENUM_WORKTIME )
	{
		stream << ".WORKTIME.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcTaskDurationEnum> IfcTaskDurationEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	shared_ptr<IfcTaskDurationEnum> type_object( new IfcTaskDurationEnum() );
	if( boost::iequals( arg, L".ELAPSEDTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_ELAPSEDTIME;
	}
	else if( boost::iequals( arg, L".WORKTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_WORKTIME;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
