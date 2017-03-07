/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
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
