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
#include "include/IfcCableSegmentTypeEnum.h"

// TYPE IfcCableSegmentTypeEnum 
IfcCableSegmentTypeEnum::IfcCableSegmentTypeEnum() {}
IfcCableSegmentTypeEnum::~IfcCableSegmentTypeEnum() {}
shared_ptr<IfcPPObject> IfcCableSegmentTypeEnum::getDeepCopy()
{
	shared_ptr<IfcCableSegmentTypeEnum> copy_self( new IfcCableSegmentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcCableSegmentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCABLESEGMENTTYPEENUM("; }
	if( m_enum == ENUM_BUSBARSEGMENT )
	{
		stream << ".BUSBARSEGMENT.";
	}
	else if( m_enum == ENUM_CABLESEGMENT )
	{
		stream << ".CABLESEGMENT.";
	}
	else if( m_enum == ENUM_CONDUCTORSEGMENT )
	{
		stream << ".CONDUCTORSEGMENT.";
	}
	else if( m_enum == ENUM_CORESEGMENT )
	{
		stream << ".CORESEGMENT.";
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
shared_ptr<IfcCableSegmentTypeEnum> IfcCableSegmentTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcCableSegmentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcCableSegmentTypeEnum>(); }
	shared_ptr<IfcCableSegmentTypeEnum> type_object( new IfcCableSegmentTypeEnum() );
	if( boost::iequals( arg, L".BUSBARSEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_BUSBARSEGMENT;
	}
	else if( boost::iequals( arg, L".CABLESEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CABLESEGMENT;
	}
	else if( boost::iequals( arg, L".CONDUCTORSEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CONDUCTORSEGMENT;
	}
	else if( boost::iequals( arg, L".CORESEGMENT." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_CORESEGMENT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcCableSegmentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
