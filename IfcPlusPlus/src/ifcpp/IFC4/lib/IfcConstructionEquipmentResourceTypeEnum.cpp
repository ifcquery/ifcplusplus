/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcConstructionEquipmentResourceTypeEnum.h"

// TYPE IfcConstructionEquipmentResourceTypeEnum = ENUMERATION OF	(DEMOLISHING	,EARTHMOVING	,ERECTING	,HEATING	,LIGHTING	,PAVING	,PUMPING	,TRANSPORTING	,USERDEFINED	,NOTDEFINED);
IfcConstructionEquipmentResourceTypeEnum::~IfcConstructionEquipmentResourceTypeEnum() {}
shared_ptr<BuildingObject> IfcConstructionEquipmentResourceTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcConstructionEquipmentResourceTypeEnum> copy_self( new IfcConstructionEquipmentResourceTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcConstructionEquipmentResourceTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCONSTRUCTIONEQUIPMENTRESOURCETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_DEMOLISHING:	stream << ".DEMOLISHING."; break;
		case ENUM_EARTHMOVING:	stream << ".EARTHMOVING."; break;
		case ENUM_ERECTING:	stream << ".ERECTING."; break;
		case ENUM_HEATING:	stream << ".HEATING."; break;
		case ENUM_LIGHTING:	stream << ".LIGHTING."; break;
		case ENUM_PAVING:	stream << ".PAVING."; break;
		case ENUM_PUMPING:	stream << ".PUMPING."; break;
		case ENUM_TRANSPORTING:	stream << ".TRANSPORTING."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcConstructionEquipmentResourceTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_DEMOLISHING:	return L"DEMOLISHING";
		case ENUM_EARTHMOVING:	return L"EARTHMOVING";
		case ENUM_ERECTING:	return L"ERECTING";
		case ENUM_HEATING:	return L"HEATING";
		case ENUM_LIGHTING:	return L"LIGHTING";
		case ENUM_PAVING:	return L"PAVING";
		case ENUM_PUMPING:	return L"PUMPING";
		case ENUM_TRANSPORTING:	return L"TRANSPORTING";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcConstructionEquipmentResourceTypeEnum> IfcConstructionEquipmentResourceTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcConstructionEquipmentResourceTypeEnum>(); }
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
