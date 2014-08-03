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
#include "include/IfcElectricGeneratorTypeEnum.h"

// TYPE IfcElectricGeneratorTypeEnum 
IfcElectricGeneratorTypeEnum::IfcElectricGeneratorTypeEnum() {}
IfcElectricGeneratorTypeEnum::~IfcElectricGeneratorTypeEnum() {}
shared_ptr<IfcPPObject> IfcElectricGeneratorTypeEnum::getDeepCopy()
{
	shared_ptr<IfcElectricGeneratorTypeEnum> copy_self( new IfcElectricGeneratorTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElectricGeneratorTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICGENERATORTYPEENUM("; }
	if( m_enum == ENUM_CHP )
	{
		stream << ".CHP.";
	}
	else if( m_enum == ENUM_ENGINEGENERATOR )
	{
		stream << ".ENGINEGENERATOR.";
	}
	else if( m_enum == ENUM_STANDALONE )
	{
		stream << ".STANDALONE.";
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
shared_ptr<IfcElectricGeneratorTypeEnum> IfcElectricGeneratorTypeEnum::createObjectFromStepData( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricGeneratorTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricGeneratorTypeEnum>(); }
	shared_ptr<IfcElectricGeneratorTypeEnum> type_object( new IfcElectricGeneratorTypeEnum() );
	if( boost::iequals( arg, L".CHP." ) )
	{
		type_object->m_enum = IfcElectricGeneratorTypeEnum::ENUM_CHP;
	}
	else if( boost::iequals( arg, L".ENGINEGENERATOR." ) )
	{
		type_object->m_enum = IfcElectricGeneratorTypeEnum::ENUM_ENGINEGENERATOR;
	}
	else if( boost::iequals( arg, L".STANDALONE." ) )
	{
		type_object->m_enum = IfcElectricGeneratorTypeEnum::ENUM_STANDALONE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricGeneratorTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricGeneratorTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
