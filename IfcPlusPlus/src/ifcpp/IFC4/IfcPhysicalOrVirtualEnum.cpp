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
#include "include/IfcPhysicalOrVirtualEnum.h"

// TYPE IfcPhysicalOrVirtualEnum = ENUMERATION OF	(PHYSICAL	,VIRTUAL	,NOTDEFINED);
IfcPhysicalOrVirtualEnum::IfcPhysicalOrVirtualEnum() {}
IfcPhysicalOrVirtualEnum::~IfcPhysicalOrVirtualEnum() {}
shared_ptr<IfcPPObject> IfcPhysicalOrVirtualEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPhysicalOrVirtualEnum> copy_self( new IfcPhysicalOrVirtualEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPhysicalOrVirtualEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPHYSICALORVIRTUALENUM("; }
	if( m_enum == ENUM_PHYSICAL )
	{
		stream << ".PHYSICAL.";
	}
	else if( m_enum == ENUM_VIRTUAL )
	{
		stream << ".VIRTUAL.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPhysicalOrVirtualEnum> IfcPhysicalOrVirtualEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPhysicalOrVirtualEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPhysicalOrVirtualEnum>(); }
	shared_ptr<IfcPhysicalOrVirtualEnum> type_object( new IfcPhysicalOrVirtualEnum() );
	if( boost::iequals( arg, L".PHYSICAL." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_PHYSICAL;
	}
	else if( boost::iequals( arg, L".VIRTUAL." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_VIRTUAL;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPhysicalOrVirtualEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
