/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcSpatialZoneTypeEnum.h"

// TYPE IfcSpatialZoneTypeEnum = ENUMERATION OF	(CONSTRUCTION	,FIRESAFETY	,LIGHTING	,OCCUPANCY	,SECURITY	,THERMAL	,TRANSPORT	,VENTILATION	,USERDEFINED	,NOTDEFINED);
IfcSpatialZoneTypeEnum::IfcSpatialZoneTypeEnum() = default;
IfcSpatialZoneTypeEnum::~IfcSpatialZoneTypeEnum() = default;
shared_ptr<BuildingObject> IfcSpatialZoneTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSpatialZoneTypeEnum> copy_self( new IfcSpatialZoneTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSpatialZoneTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSPATIALZONETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_CONSTRUCTION:	stream << ".CONSTRUCTION."; break;
		case ENUM_FIRESAFETY:	stream << ".FIRESAFETY."; break;
		case ENUM_LIGHTING:	stream << ".LIGHTING."; break;
		case ENUM_OCCUPANCY:	stream << ".OCCUPANCY."; break;
		case ENUM_SECURITY:	stream << ".SECURITY."; break;
		case ENUM_THERMAL:	stream << ".THERMAL."; break;
		case ENUM_TRANSPORT:	stream << ".TRANSPORT."; break;
		case ENUM_VENTILATION:	stream << ".VENTILATION."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSpatialZoneTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_CONSTRUCTION:	return L"CONSTRUCTION";
		case ENUM_FIRESAFETY:	return L"FIRESAFETY";
		case ENUM_LIGHTING:	return L"LIGHTING";
		case ENUM_OCCUPANCY:	return L"OCCUPANCY";
		case ENUM_SECURITY:	return L"SECURITY";
		case ENUM_THERMAL:	return L"THERMAL";
		case ENUM_TRANSPORT:	return L"TRANSPORT";
		case ENUM_VENTILATION:	return L"VENTILATION";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcSpatialZoneTypeEnum> IfcSpatialZoneTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSpatialZoneTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSpatialZoneTypeEnum>(); }
	shared_ptr<IfcSpatialZoneTypeEnum> type_object( new IfcSpatialZoneTypeEnum() );
	if( boost::iequals( arg, L".CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".FIRESAFETY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_FIRESAFETY;
	}
	else if( boost::iequals( arg, L".LIGHTING." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_LIGHTING;
	}
	else if( boost::iequals( arg, L".OCCUPANCY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_OCCUPANCY;
	}
	else if( boost::iequals( arg, L".SECURITY." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_SECURITY;
	}
	else if( boost::iequals( arg, L".THERMAL." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_THERMAL;
	}
	else if( boost::iequals( arg, L".TRANSPORT." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_TRANSPORT;
	}
	else if( boost::iequals( arg, L".VENTILATION." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_VENTILATION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSpatialZoneTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
