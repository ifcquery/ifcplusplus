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
#include "include/IfcDocumentConfidentialityEnum.h"

// TYPE IfcDocumentConfidentialityEnum 
IfcDocumentConfidentialityEnum::IfcDocumentConfidentialityEnum() {}
IfcDocumentConfidentialityEnum::~IfcDocumentConfidentialityEnum() {}
shared_ptr<IfcPPObject> IfcDocumentConfidentialityEnum::getDeepCopy()
{
	shared_ptr<IfcDocumentConfidentialityEnum> copy_self( new IfcDocumentConfidentialityEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDocumentConfidentialityEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOCUMENTCONFIDENTIALITYENUM("; }
	if( m_enum == ENUM_PUBLIC )
	{
		stream << ".PUBLIC.";
	}
	else if( m_enum == ENUM_RESTRICTED )
	{
		stream << ".RESTRICTED.";
	}
	else if( m_enum == ENUM_CONFIDENTIAL )
	{
		stream << ".CONFIDENTIAL.";
	}
	else if( m_enum == ENUM_PERSONAL )
	{
		stream << ".PERSONAL.";
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
shared_ptr<IfcDocumentConfidentialityEnum> IfcDocumentConfidentialityEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDocumentConfidentialityEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDocumentConfidentialityEnum>(); }
	shared_ptr<IfcDocumentConfidentialityEnum> type_object( new IfcDocumentConfidentialityEnum() );
	if( boost::iequals( arg, L".PUBLIC." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_PUBLIC;
	}
	else if( boost::iequals( arg, L".RESTRICTED." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_RESTRICTED;
	}
	else if( boost::iequals( arg, L".CONFIDENTIAL." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_CONFIDENTIAL;
	}
	else if( boost::iequals( arg, L".PERSONAL." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_PERSONAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDocumentConfidentialityEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
