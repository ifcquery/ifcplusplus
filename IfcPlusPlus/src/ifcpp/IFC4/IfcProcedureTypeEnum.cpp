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
#include "include/IfcProcedureTypeEnum.h"

// TYPE IfcProcedureTypeEnum = ENUMERATION OF	(ADVICE_CAUTION	,ADVICE_NOTE	,ADVICE_WARNING	,CALIBRATION	,DIAGNOSTIC	,SHUTDOWN	,STARTUP	,USERDEFINED	,NOTDEFINED);
IfcProcedureTypeEnum::IfcProcedureTypeEnum() {}
IfcProcedureTypeEnum::~IfcProcedureTypeEnum() {}
shared_ptr<IfcPPObject> IfcProcedureTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcProcedureTypeEnum> copy_self( new IfcProcedureTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcProcedureTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROCEDURETYPEENUM("; }
	if( m_enum == ENUM_ADVICE_CAUTION )
	{
		stream << ".ADVICE_CAUTION.";
	}
	else if( m_enum == ENUM_ADVICE_NOTE )
	{
		stream << ".ADVICE_NOTE.";
	}
	else if( m_enum == ENUM_ADVICE_WARNING )
	{
		stream << ".ADVICE_WARNING.";
	}
	else if( m_enum == ENUM_CALIBRATION )
	{
		stream << ".CALIBRATION.";
	}
	else if( m_enum == ENUM_DIAGNOSTIC )
	{
		stream << ".DIAGNOSTIC.";
	}
	else if( m_enum == ENUM_SHUTDOWN )
	{
		stream << ".SHUTDOWN.";
	}
	else if( m_enum == ENUM_STARTUP )
	{
		stream << ".STARTUP.";
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
shared_ptr<IfcProcedureTypeEnum> IfcProcedureTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProcedureTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProcedureTypeEnum>(); }
	shared_ptr<IfcProcedureTypeEnum> type_object( new IfcProcedureTypeEnum() );
	if( boost::iequals( arg, L".ADVICE_CAUTION." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_ADVICE_CAUTION;
	}
	else if( boost::iequals( arg, L".ADVICE_NOTE." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_ADVICE_NOTE;
	}
	else if( boost::iequals( arg, L".ADVICE_WARNING." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_ADVICE_WARNING;
	}
	else if( boost::iequals( arg, L".CALIBRATION." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_CALIBRATION;
	}
	else if( boost::iequals( arg, L".DIAGNOSTIC." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_DIAGNOSTIC;
	}
	else if( boost::iequals( arg, L".SHUTDOWN." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_SHUTDOWN;
	}
	else if( boost::iequals( arg, L".STARTUP." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_STARTUP;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcProcedureTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
