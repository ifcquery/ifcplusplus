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
#include "include/IfcTransportElementTypeEnum.h"

// TYPE IfcTransportElementTypeEnum = ENUMERATION OF	(ELEVATOR	,ESCALATOR	,MOVINGWALKWAY	,CRANEWAY	,LIFTINGGEAR	,USERDEFINED	,NOTDEFINED);
IfcTransportElementTypeEnum::IfcTransportElementTypeEnum() {}
IfcTransportElementTypeEnum::~IfcTransportElementTypeEnum() {}
shared_ptr<IfcPPObject> IfcTransportElementTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTransportElementTypeEnum> copy_self( new IfcTransportElementTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTransportElementTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSPORTELEMENTTYPEENUM("; }
	if( m_enum == ENUM_ELEVATOR )
	{
		stream << ".ELEVATOR.";
	}
	else if( m_enum == ENUM_ESCALATOR )
	{
		stream << ".ESCALATOR.";
	}
	else if( m_enum == ENUM_MOVINGWALKWAY )
	{
		stream << ".MOVINGWALKWAY.";
	}
	else if( m_enum == ENUM_CRANEWAY )
	{
		stream << ".CRANEWAY.";
	}
	else if( m_enum == ENUM_LIFTINGGEAR )
	{
		stream << ".LIFTINGGEAR.";
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
shared_ptr<IfcTransportElementTypeEnum> IfcTransportElementTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTransportElementTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTransportElementTypeEnum>(); }
	shared_ptr<IfcTransportElementTypeEnum> type_object( new IfcTransportElementTypeEnum() );
	if( boost::iequals( arg, L".ELEVATOR." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_ELEVATOR;
	}
	else if( boost::iequals( arg, L".ESCALATOR." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_ESCALATOR;
	}
	else if( boost::iequals( arg, L".MOVINGWALKWAY." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_MOVINGWALKWAY;
	}
	else if( boost::iequals( arg, L".CRANEWAY." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_CRANEWAY;
	}
	else if( boost::iequals( arg, L".LIFTINGGEAR." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_LIFTINGGEAR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTransportElementTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
