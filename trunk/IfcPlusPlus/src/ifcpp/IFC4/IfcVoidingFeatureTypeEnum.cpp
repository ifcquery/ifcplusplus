/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcVoidingFeatureTypeEnum.h"

// TYPE IfcVoidingFeatureTypeEnum 
IfcVoidingFeatureTypeEnum::IfcVoidingFeatureTypeEnum() {}
IfcVoidingFeatureTypeEnum::~IfcVoidingFeatureTypeEnum() {}
shared_ptr<IfcPPObject> IfcVoidingFeatureTypeEnum::getDeepCopy()
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
shared_ptr<IfcVoidingFeatureTypeEnum> IfcVoidingFeatureTypeEnum::createObjectFromStepData( const std::wstring& arg )
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
