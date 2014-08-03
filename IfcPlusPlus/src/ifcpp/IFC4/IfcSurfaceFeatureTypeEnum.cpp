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
#include "include/IfcSurfaceFeatureTypeEnum.h"

// TYPE IfcSurfaceFeatureTypeEnum 
IfcSurfaceFeatureTypeEnum::IfcSurfaceFeatureTypeEnum() {}
IfcSurfaceFeatureTypeEnum::~IfcSurfaceFeatureTypeEnum() {}
shared_ptr<IfcPPObject> IfcSurfaceFeatureTypeEnum::getDeepCopy()
{
	shared_ptr<IfcSurfaceFeatureTypeEnum> copy_self( new IfcSurfaceFeatureTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSurfaceFeatureTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSURFACEFEATURETYPEENUM("; }
	if( m_enum == ENUM_MARK )
	{
		stream << ".MARK.";
	}
	else if( m_enum == ENUM_TAG )
	{
		stream << ".TAG.";
	}
	else if( m_enum == ENUM_TREATMENT )
	{
		stream << ".TREATMENT.";
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
shared_ptr<IfcSurfaceFeatureTypeEnum> IfcSurfaceFeatureTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSurfaceFeatureTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSurfaceFeatureTypeEnum>(); }
	shared_ptr<IfcSurfaceFeatureTypeEnum> type_object( new IfcSurfaceFeatureTypeEnum() );
	if( boost::iequals( arg, L".MARK." ) )
	{
		type_object->m_enum = IfcSurfaceFeatureTypeEnum::ENUM_MARK;
	}
	else if( boost::iequals( arg, L".TAG." ) )
	{
		type_object->m_enum = IfcSurfaceFeatureTypeEnum::ENUM_TAG;
	}
	else if( boost::iequals( arg, L".TREATMENT." ) )
	{
		type_object->m_enum = IfcSurfaceFeatureTypeEnum::ENUM_TREATMENT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSurfaceFeatureTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSurfaceFeatureTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
