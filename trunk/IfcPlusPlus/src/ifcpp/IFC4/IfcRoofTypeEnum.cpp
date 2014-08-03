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
#include "include/IfcRoofTypeEnum.h"

// TYPE IfcRoofTypeEnum 
IfcRoofTypeEnum::IfcRoofTypeEnum() {}
IfcRoofTypeEnum::~IfcRoofTypeEnum() {}
shared_ptr<IfcPPObject> IfcRoofTypeEnum::getDeepCopy()
{
	shared_ptr<IfcRoofTypeEnum> copy_self( new IfcRoofTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRoofTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCROOFTYPEENUM("; }
	if( m_enum == ENUM_FLAT_ROOF )
	{
		stream << ".FLAT_ROOF.";
	}
	else if( m_enum == ENUM_SHED_ROOF )
	{
		stream << ".SHED_ROOF.";
	}
	else if( m_enum == ENUM_GABLE_ROOF )
	{
		stream << ".GABLE_ROOF.";
	}
	else if( m_enum == ENUM_HIP_ROOF )
	{
		stream << ".HIP_ROOF.";
	}
	else if( m_enum == ENUM_HIPPED_GABLE_ROOF )
	{
		stream << ".HIPPED_GABLE_ROOF.";
	}
	else if( m_enum == ENUM_GAMBREL_ROOF )
	{
		stream << ".GAMBREL_ROOF.";
	}
	else if( m_enum == ENUM_MANSARD_ROOF )
	{
		stream << ".MANSARD_ROOF.";
	}
	else if( m_enum == ENUM_BARREL_ROOF )
	{
		stream << ".BARREL_ROOF.";
	}
	else if( m_enum == ENUM_RAINBOW_ROOF )
	{
		stream << ".RAINBOW_ROOF.";
	}
	else if( m_enum == ENUM_BUTTERFLY_ROOF )
	{
		stream << ".BUTTERFLY_ROOF.";
	}
	else if( m_enum == ENUM_PAVILION_ROOF )
	{
		stream << ".PAVILION_ROOF.";
	}
	else if( m_enum == ENUM_DOME_ROOF )
	{
		stream << ".DOME_ROOF.";
	}
	else if( m_enum == ENUM_FREEFORM )
	{
		stream << ".FREEFORM.";
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
shared_ptr<IfcRoofTypeEnum> IfcRoofTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	shared_ptr<IfcRoofTypeEnum> type_object( new IfcRoofTypeEnum() );
	if( boost::iequals( arg, L".FLAT_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FLAT_ROOF;
	}
	else if( boost::iequals( arg, L".SHED_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_SHED_ROOF;
	}
	else if( boost::iequals( arg, L".GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".HIP_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIP_ROOF;
	}
	else if( boost::iequals( arg, L".HIPPED_GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIPPED_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".GAMBREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GAMBREL_ROOF;
	}
	else if( boost::iequals( arg, L".MANSARD_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_MANSARD_ROOF;
	}
	else if( boost::iequals( arg, L".BARREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BARREL_ROOF;
	}
	else if( boost::iequals( arg, L".RAINBOW_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_RAINBOW_ROOF;
	}
	else if( boost::iequals( arg, L".BUTTERFLY_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BUTTERFLY_ROOF;
	}
	else if( boost::iequals( arg, L".PAVILION_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_PAVILION_ROOF;
	}
	else if( boost::iequals( arg, L".DOME_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_DOME_ROOF;
	}
	else if( boost::iequals( arg, L".FREEFORM." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FREEFORM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
