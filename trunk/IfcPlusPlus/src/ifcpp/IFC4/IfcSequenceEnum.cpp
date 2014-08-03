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
#include "include/IfcSequenceEnum.h"

// TYPE IfcSequenceEnum 
IfcSequenceEnum::IfcSequenceEnum() {}
IfcSequenceEnum::~IfcSequenceEnum() {}
shared_ptr<IfcPPObject> IfcSequenceEnum::getDeepCopy()
{
	shared_ptr<IfcSequenceEnum> copy_self( new IfcSequenceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSequenceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSEQUENCEENUM("; }
	if( m_enum == ENUM_START_START )
	{
		stream << ".START_START.";
	}
	else if( m_enum == ENUM_START_FINISH )
	{
		stream << ".START_FINISH.";
	}
	else if( m_enum == ENUM_FINISH_START )
	{
		stream << ".FINISH_START.";
	}
	else if( m_enum == ENUM_FINISH_FINISH )
	{
		stream << ".FINISH_FINISH.";
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
shared_ptr<IfcSequenceEnum> IfcSequenceEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSequenceEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSequenceEnum>(); }
	shared_ptr<IfcSequenceEnum> type_object( new IfcSequenceEnum() );
	if( boost::iequals( arg, L".START_START." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_START_START;
	}
	else if( boost::iequals( arg, L".START_FINISH." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_START_FINISH;
	}
	else if( boost::iequals( arg, L".FINISH_START." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_FINISH_START;
	}
	else if( boost::iequals( arg, L".FINISH_FINISH." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_FINISH_FINISH;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
