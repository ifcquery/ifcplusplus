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
#include "include/IfcConstructionEquipmentResourceTypeEnum.h"

// TYPE IfcConstructionEquipmentResourceTypeEnum = ENUMERATION OF	(DEMOLISHING	,EARTHMOVING	,ERECTING	,HEATING	,LIGHTING	,PAVING	,PUMPING	,TRANSPORTING	,USERDEFINED	,NOTDEFINED);
IfcConstructionEquipmentResourceTypeEnum::IfcConstructionEquipmentResourceTypeEnum() {}
IfcConstructionEquipmentResourceTypeEnum::~IfcConstructionEquipmentResourceTypeEnum() {}
shared_ptr<IfcPPObject> IfcConstructionEquipmentResourceTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConstructionEquipmentResourceTypeEnum> copy_self( new IfcConstructionEquipmentResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcConstructionEquipmentResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONSTRUCTIONEQUIPMENTRESOURCETYPEENUM("; }
	if( m_enum == ENUM_DEMOLISHING )
	{
		stream << ".DEMOLISHING.";
	}
	else if( m_enum == ENUM_EARTHMOVING )
	{
		stream << ".EARTHMOVING.";
	}
	else if( m_enum == ENUM_ERECTING )
	{
		stream << ".ERECTING.";
	}
	else if( m_enum == ENUM_HEATING )
	{
		stream << ".HEATING.";
	}
	else if( m_enum == ENUM_LIGHTING )
	{
		stream << ".LIGHTING.";
	}
	else if( m_enum == ENUM_PAVING )
	{
		stream << ".PAVING.";
	}
	else if( m_enum == ENUM_PUMPING )
	{
		stream << ".PUMPING.";
	}
	else if( m_enum == ENUM_TRANSPORTING )
	{
		stream << ".TRANSPORTING.";
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
shared_ptr<IfcConstructionEquipmentResourceTypeEnum> IfcConstructionEquipmentResourceTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
	shared_ptr<IfcConstructionEquipmentResourceTypeEnum> type_object( new IfcConstructionEquipmentResourceTypeEnum() );
	if( boost::iequals( arg, L".DEMOLISHING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_DEMOLISHING;
	}
	else if( boost::iequals( arg, L".EARTHMOVING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_EARTHMOVING;
	}
	else if( boost::iequals( arg, L".ERECTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_ERECTING;
	}
	else if( boost::iequals( arg, L".HEATING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_HEATING;
	}
	else if( boost::iequals( arg, L".LIGHTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_LIGHTING;
	}
	else if( boost::iequals( arg, L".PAVING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_PAVING;
	}
	else if( boost::iequals( arg, L".PUMPING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_PUMPING;
	}
	else if( boost::iequals( arg, L".TRANSPORTING." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_TRANSPORTING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcConstructionEquipmentResourceTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
