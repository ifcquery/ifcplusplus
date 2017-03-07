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
#include "include/IfcReflectanceMethodEnum.h"

// TYPE IfcReflectanceMethodEnum = ENUMERATION OF	(BLINN	,FLAT	,GLASS	,MATT	,METAL	,MIRROR	,PHONG	,PLASTIC	,STRAUSS	,NOTDEFINED);
IfcReflectanceMethodEnum::IfcReflectanceMethodEnum() {}
IfcReflectanceMethodEnum::~IfcReflectanceMethodEnum() {}
shared_ptr<IfcPPObject> IfcReflectanceMethodEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReflectanceMethodEnum> copy_self( new IfcReflectanceMethodEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReflectanceMethodEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREFLECTANCEMETHODENUM("; }
	if( m_enum == ENUM_BLINN )
	{
		stream << ".BLINN.";
	}
	else if( m_enum == ENUM_FLAT )
	{
		stream << ".FLAT.";
	}
	else if( m_enum == ENUM_GLASS )
	{
		stream << ".GLASS.";
	}
	else if( m_enum == ENUM_MATT )
	{
		stream << ".MATT.";
	}
	else if( m_enum == ENUM_METAL )
	{
		stream << ".METAL.";
	}
	else if( m_enum == ENUM_MIRROR )
	{
		stream << ".MIRROR.";
	}
	else if( m_enum == ENUM_PHONG )
	{
		stream << ".PHONG.";
	}
	else if( m_enum == ENUM_PLASTIC )
	{
		stream << ".PLASTIC.";
	}
	else if( m_enum == ENUM_STRAUSS )
	{
		stream << ".STRAUSS.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcReflectanceMethodEnum> IfcReflectanceMethodEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReflectanceMethodEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReflectanceMethodEnum>(); }
	shared_ptr<IfcReflectanceMethodEnum> type_object( new IfcReflectanceMethodEnum() );
	if( boost::iequals( arg, L".BLINN." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_BLINN;
	}
	else if( boost::iequals( arg, L".FLAT." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_FLAT;
	}
	else if( boost::iequals( arg, L".GLASS." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_GLASS;
	}
	else if( boost::iequals( arg, L".MATT." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MATT;
	}
	else if( boost::iequals( arg, L".METAL." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_METAL;
	}
	else if( boost::iequals( arg, L".MIRROR." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_MIRROR;
	}
	else if( boost::iequals( arg, L".PHONG." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PHONG;
	}
	else if( boost::iequals( arg, L".PLASTIC." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_PLASTIC;
	}
	else if( boost::iequals( arg, L".STRAUSS." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_STRAUSS;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcReflectanceMethodEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
