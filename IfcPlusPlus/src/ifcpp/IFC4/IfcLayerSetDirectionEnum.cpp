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
#include "include/IfcLayerSetDirectionEnum.h"

// TYPE IfcLayerSetDirectionEnum = ENUMERATION OF	(AXIS1	,AXIS2	,AXIS3);
IfcLayerSetDirectionEnum::IfcLayerSetDirectionEnum() {}
IfcLayerSetDirectionEnum::~IfcLayerSetDirectionEnum() {}
shared_ptr<IfcPPObject> IfcLayerSetDirectionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLayerSetDirectionEnum> copy_self( new IfcLayerSetDirectionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcLayerSetDirectionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCLAYERSETDIRECTIONENUM("; }
	if( m_enum == ENUM_AXIS1 )
	{
		stream << ".AXIS1.";
	}
	else if( m_enum == ENUM_AXIS2 )
	{
		stream << ".AXIS2.";
	}
	else if( m_enum == ENUM_AXIS3 )
	{
		stream << ".AXIS3.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcLayerSetDirectionEnum> IfcLayerSetDirectionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcLayerSetDirectionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcLayerSetDirectionEnum>(); }
	shared_ptr<IfcLayerSetDirectionEnum> type_object( new IfcLayerSetDirectionEnum() );
	if( boost::iequals( arg, L".AXIS1." ) )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS1;
	}
	else if( boost::iequals( arg, L".AXIS2." ) )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS2;
	}
	else if( boost::iequals( arg, L".AXIS3." ) )
	{
		type_object->m_enum = IfcLayerSetDirectionEnum::ENUM_AXIS3;
	}
	return type_object;
}
