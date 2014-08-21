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
#include "include/IfcLogicalOperatorEnum.h"

// TYPE IfcLogicalOperatorEnum = ENUMERATION OF	(LOGICALAND	,LOGICALOR	,LOGICALXOR	,LOGICALNOTAND	,LOGICALNOTOR);
IfcLogicalOperatorEnum::IfcLogicalOperatorEnum() {}
IfcLogicalOperatorEnum::~IfcLogicalOperatorEnum() {}
shared_ptr<IfcPPObject> IfcLogicalOperatorEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLogicalOperatorEnum> copy_self( new IfcLogicalOperatorEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLogicalOperatorEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLOGICALOPERATORENUM("; }
	if( m_enum == ENUM_LOGICALAND )
	{
		stream << ".LOGICALAND.";
	}
	else if( m_enum == ENUM_LOGICALOR )
	{
		stream << ".LOGICALOR.";
	}
	else if( m_enum == ENUM_LOGICALXOR )
	{
		stream << ".LOGICALXOR.";
	}
	else if( m_enum == ENUM_LOGICALNOTAND )
	{
		stream << ".LOGICALNOTAND.";
	}
	else if( m_enum == ENUM_LOGICALNOTOR )
	{
		stream << ".LOGICALNOTOR.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLogicalOperatorEnum> IfcLogicalOperatorEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLogicalOperatorEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLogicalOperatorEnum>(); }
	shared_ptr<IfcLogicalOperatorEnum> type_object( new IfcLogicalOperatorEnum() );
	if( boost::iequals( arg, L".LOGICALAND." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALAND;
	}
	else if( boost::iequals( arg, L".LOGICALOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALOR;
	}
	else if( boost::iequals( arg, L".LOGICALXOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALXOR;
	}
	else if( boost::iequals( arg, L".LOGICALNOTAND." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALNOTAND;
	}
	else if( boost::iequals( arg, L".LOGICALNOTOR." ) )
	{
		type_object->m_enum = IfcLogicalOperatorEnum::ENUM_LOGICALNOTOR;
	}
	return type_object;
}
