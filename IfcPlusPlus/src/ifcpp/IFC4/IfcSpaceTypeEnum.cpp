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
#include "include/IfcSpaceTypeEnum.h"

// TYPE IfcSpaceTypeEnum = ENUMERATION OF	(SPACE	,PARKING	,GFA	,INTERNAL	,EXTERNAL	,USERDEFINED	,NOTDEFINED);
IfcSpaceTypeEnum::IfcSpaceTypeEnum() {}
IfcSpaceTypeEnum::~IfcSpaceTypeEnum() {}
shared_ptr<IfcPPObject> IfcSpaceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSpaceTypeEnum> copy_self( new IfcSpaceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpaceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPACETYPEENUM("; }
	if( m_enum == ENUM_SPACE )
	{
		stream << ".SPACE.";
	}
	else if( m_enum == ENUM_PARKING )
	{
		stream << ".PARKING.";
	}
	else if( m_enum == ENUM_GFA )
	{
		stream << ".GFA.";
	}
	else if( m_enum == ENUM_INTERNAL )
	{
		stream << ".INTERNAL.";
	}
	else if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
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
shared_ptr<IfcSpaceTypeEnum> IfcSpaceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpaceTypeEnum>(); }
	shared_ptr<IfcSpaceTypeEnum> type_object( new IfcSpaceTypeEnum() );
	if( boost::iequals( arg, L".SPACE." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_SPACE;
	}
	else if( boost::iequals( arg, L".PARKING." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_PARKING;
	}
	else if( boost::iequals( arg, L".GFA." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_GFA;
	}
	else if( boost::iequals( arg, L".INTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_INTERNAL;
	}
	else if( boost::iequals( arg, L".EXTERNAL." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_EXTERNAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
