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
#include "include/IfcVoidingFeatureTypeEnum.h"

// TYPE IfcVoidingFeatureTypeEnum = ENUMERATION OF	(CUTOUT	,NOTCH	,HOLE	,MITER	,CHAMFER	,EDGE	,USERDEFINED	,NOTDEFINED);
IfcVoidingFeatureTypeEnum::IfcVoidingFeatureTypeEnum() {}
IfcVoidingFeatureTypeEnum::~IfcVoidingFeatureTypeEnum() {}
shared_ptr<IfcPPObject> IfcVoidingFeatureTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVoidingFeatureTypeEnum> copy_self( new IfcVoidingFeatureTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcVoidingFeatureTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVOIDINGFEATURETYPEENUM("; }
	if( m_enum == ENUM_CUTOUT )
	{
		stream << ".CUTOUT.";
	}
	else if( m_enum == ENUM_NOTCH )
	{
		stream << ".NOTCH.";
	}
	else if( m_enum == ENUM_HOLE )
	{
		stream << ".HOLE.";
	}
	else if( m_enum == ENUM_MITER )
	{
		stream << ".MITER.";
	}
	else if( m_enum == ENUM_CHAMFER )
	{
		stream << ".CHAMFER.";
	}
	else if( m_enum == ENUM_EDGE )
	{
		stream << ".EDGE.";
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
shared_ptr<IfcVoidingFeatureTypeEnum> IfcVoidingFeatureTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcVoidingFeatureTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcVoidingFeatureTypeEnum>(); }
	shared_ptr<IfcVoidingFeatureTypeEnum> type_object( new IfcVoidingFeatureTypeEnum() );
	if( boost::iequals( arg, L".CUTOUT." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_CUTOUT;
	}
	else if( boost::iequals( arg, L".NOTCH." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_NOTCH;
	}
	else if( boost::iequals( arg, L".HOLE." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_HOLE;
	}
	else if( boost::iequals( arg, L".MITER." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_MITER;
	}
	else if( boost::iequals( arg, L".CHAMFER." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_CHAMFER;
	}
	else if( boost::iequals( arg, L".EDGE." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_EDGE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcVoidingFeatureTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
