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
#include "include/IfcSpaceHeaterTypeEnum.h"

// TYPE IfcSpaceHeaterTypeEnum = ENUMERATION OF	(CONVECTOR	,RADIATOR	,USERDEFINED	,NOTDEFINED);
IfcSpaceHeaterTypeEnum::IfcSpaceHeaterTypeEnum() {}
IfcSpaceHeaterTypeEnum::~IfcSpaceHeaterTypeEnum() {}
shared_ptr<IfcPPObject> IfcSpaceHeaterTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSpaceHeaterTypeEnum> copy_self( new IfcSpaceHeaterTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpaceHeaterTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPACEHEATERTYPEENUM("; }
	if( m_enum == ENUM_CONVECTOR )
	{
		stream << ".CONVECTOR.";
	}
	else if( m_enum == ENUM_RADIATOR )
	{
		stream << ".RADIATOR.";
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
shared_ptr<IfcSpaceHeaterTypeEnum> IfcSpaceHeaterTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpaceHeaterTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpaceHeaterTypeEnum>(); }
	shared_ptr<IfcSpaceHeaterTypeEnum> type_object( new IfcSpaceHeaterTypeEnum() );
	if( boost::iequals( arg, L".CONVECTOR." ) )
	{
		type_object->m_enum = IfcSpaceHeaterTypeEnum::ENUM_CONVECTOR;
	}
	else if( boost::iequals( arg, L".RADIATOR." ) )
	{
		type_object->m_enum = IfcSpaceHeaterTypeEnum::ENUM_RADIATOR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceHeaterTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpaceHeaterTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
