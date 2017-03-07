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
#include "include/IfcWallTypeEnum.h"

// TYPE IfcWallTypeEnum = ENUMERATION OF	(MOVABLE	,PARAPET	,PARTITIONING	,PLUMBINGWALL	,SHEAR	,SOLIDWALL	,STANDARD	,POLYGONAL	,ELEMENTEDWALL	,USERDEFINED	,NOTDEFINED);
IfcWallTypeEnum::IfcWallTypeEnum() {}
IfcWallTypeEnum::~IfcWallTypeEnum() {}
shared_ptr<IfcPPObject> IfcWallTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcWallTypeEnum> copy_self( new IfcWallTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWallTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWALLTYPEENUM("; }
	if( m_enum == ENUM_MOVABLE )
	{
		stream << ".MOVABLE.";
	}
	else if( m_enum == ENUM_PARAPET )
	{
		stream << ".PARAPET.";
	}
	else if( m_enum == ENUM_PARTITIONING )
	{
		stream << ".PARTITIONING.";
	}
	else if( m_enum == ENUM_PLUMBINGWALL )
	{
		stream << ".PLUMBINGWALL.";
	}
	else if( m_enum == ENUM_SHEAR )
	{
		stream << ".SHEAR.";
	}
	else if( m_enum == ENUM_SOLIDWALL )
	{
		stream << ".SOLIDWALL.";
	}
	else if( m_enum == ENUM_STANDARD )
	{
		stream << ".STANDARD.";
	}
	else if( m_enum == ENUM_POLYGONAL )
	{
		stream << ".POLYGONAL.";
	}
	else if( m_enum == ENUM_ELEMENTEDWALL )
	{
		stream << ".ELEMENTEDWALL.";
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
shared_ptr<IfcWallTypeEnum> IfcWallTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWallTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWallTypeEnum>(); }
	shared_ptr<IfcWallTypeEnum> type_object( new IfcWallTypeEnum() );
	if( boost::iequals( arg, L".MOVABLE." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_MOVABLE;
	}
	else if( boost::iequals( arg, L".PARAPET." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PARAPET;
	}
	else if( boost::iequals( arg, L".PARTITIONING." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PARTITIONING;
	}
	else if( boost::iequals( arg, L".PLUMBINGWALL." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_PLUMBINGWALL;
	}
	else if( boost::iequals( arg, L".SHEAR." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_SHEAR;
	}
	else if( boost::iequals( arg, L".SOLIDWALL." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_SOLIDWALL;
	}
	else if( boost::iequals( arg, L".STANDARD." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_STANDARD;
	}
	else if( boost::iequals( arg, L".POLYGONAL." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_POLYGONAL;
	}
	else if( boost::iequals( arg, L".ELEMENTEDWALL." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_ELEMENTEDWALL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWallTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
