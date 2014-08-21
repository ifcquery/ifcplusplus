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
#include "include/IfcEngineTypeEnum.h"

// TYPE IfcEngineTypeEnum = ENUMERATION OF	(EXTERNALCOMBUSTION	,INTERNALCOMBUSTION	,USERDEFINED	,NOTDEFINED);
IfcEngineTypeEnum::IfcEngineTypeEnum() {}
IfcEngineTypeEnum::~IfcEngineTypeEnum() {}
shared_ptr<IfcPPObject> IfcEngineTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEngineTypeEnum> copy_self( new IfcEngineTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEngineTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCENGINETYPEENUM("; }
	if( m_enum == ENUM_EXTERNALCOMBUSTION )
	{
		stream << ".EXTERNALCOMBUSTION.";
	}
	else if( m_enum == ENUM_INTERNALCOMBUSTION )
	{
		stream << ".INTERNALCOMBUSTION.";
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
shared_ptr<IfcEngineTypeEnum> IfcEngineTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEngineTypeEnum>(); }
	shared_ptr<IfcEngineTypeEnum> type_object( new IfcEngineTypeEnum() );
	if( boost::iequals( arg, L".EXTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_EXTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".INTERNALCOMBUSTION." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_INTERNALCOMBUSTION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEngineTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
