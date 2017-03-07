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
#include "include/IfcTaskTypeEnum.h"

// TYPE IfcTaskTypeEnum = ENUMERATION OF	(ATTENDANCE	,CONSTRUCTION	,DEMOLITION	,DISMANTLE	,DISPOSAL	,INSTALLATION	,LOGISTIC	,MAINTENANCE	,MOVE	,OPERATION	,REMOVAL	,RENOVATION	,USERDEFINED	,NOTDEFINED);
IfcTaskTypeEnum::IfcTaskTypeEnum() {}
IfcTaskTypeEnum::~IfcTaskTypeEnum() {}
shared_ptr<IfcPPObject> IfcTaskTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTaskTypeEnum> copy_self( new IfcTaskTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTaskTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTASKTYPEENUM("; }
	if( m_enum == ENUM_ATTENDANCE )
	{
		stream << ".ATTENDANCE.";
	}
	else if( m_enum == ENUM_CONSTRUCTION )
	{
		stream << ".CONSTRUCTION.";
	}
	else if( m_enum == ENUM_DEMOLITION )
	{
		stream << ".DEMOLITION.";
	}
	else if( m_enum == ENUM_DISMANTLE )
	{
		stream << ".DISMANTLE.";
	}
	else if( m_enum == ENUM_DISPOSAL )
	{
		stream << ".DISPOSAL.";
	}
	else if( m_enum == ENUM_INSTALLATION )
	{
		stream << ".INSTALLATION.";
	}
	else if( m_enum == ENUM_LOGISTIC )
	{
		stream << ".LOGISTIC.";
	}
	else if( m_enum == ENUM_MAINTENANCE )
	{
		stream << ".MAINTENANCE.";
	}
	else if( m_enum == ENUM_MOVE )
	{
		stream << ".MOVE.";
	}
	else if( m_enum == ENUM_OPERATION )
	{
		stream << ".OPERATION.";
	}
	else if( m_enum == ENUM_REMOVAL )
	{
		stream << ".REMOVAL.";
	}
	else if( m_enum == ENUM_RENOVATION )
	{
		stream << ".RENOVATION.";
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
shared_ptr<IfcTaskTypeEnum> IfcTaskTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTaskTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTaskTypeEnum>(); }
	shared_ptr<IfcTaskTypeEnum> type_object( new IfcTaskTypeEnum() );
	if( boost::iequals( arg, L".ATTENDANCE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_ATTENDANCE;
	}
	else if( boost::iequals( arg, L".CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".DEMOLITION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DEMOLITION;
	}
	else if( boost::iequals( arg, L".DISMANTLE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DISMANTLE;
	}
	else if( boost::iequals( arg, L".DISPOSAL." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_DISPOSAL;
	}
	else if( boost::iequals( arg, L".INSTALLATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_INSTALLATION;
	}
	else if( boost::iequals( arg, L".LOGISTIC." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_LOGISTIC;
	}
	else if( boost::iequals( arg, L".MAINTENANCE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_MAINTENANCE;
	}
	else if( boost::iequals( arg, L".MOVE." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_MOVE;
	}
	else if( boost::iequals( arg, L".OPERATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_OPERATION;
	}
	else if( boost::iequals( arg, L".REMOVAL." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_REMOVAL;
	}
	else if( boost::iequals( arg, L".RENOVATION." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_RENOVATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTaskTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
