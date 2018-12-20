/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcProtectiveDeviceTrippingUnitTypeEnum.h"

// TYPE IfcProtectiveDeviceTrippingUnitTypeEnum = ENUMERATION OF	(ELECTRONIC	,ELECTROMAGNETIC	,RESIDUALCURRENT	,THERMAL	,USERDEFINED	,NOTDEFINED);
IfcProtectiveDeviceTrippingUnitTypeEnum::IfcProtectiveDeviceTrippingUnitTypeEnum() = default;
IfcProtectiveDeviceTrippingUnitTypeEnum::~IfcProtectiveDeviceTrippingUnitTypeEnum() = default;
shared_ptr<BuildingObject> IfcProtectiveDeviceTrippingUnitTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> copy_self( new IfcProtectiveDeviceTrippingUnitTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcProtectiveDeviceTrippingUnitTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROTECTIVEDEVICETRIPPINGUNITTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_ELECTRONIC:	stream << ".ELECTRONIC."; break;
		case ENUM_ELECTROMAGNETIC:	stream << ".ELECTROMAGNETIC."; break;
		case ENUM_RESIDUALCURRENT:	stream << ".RESIDUALCURRENT."; break;
		case ENUM_THERMAL:	stream << ".THERMAL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcProtectiveDeviceTrippingUnitTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ELECTRONIC:	return L"ELECTRONIC";
		case ENUM_ELECTROMAGNETIC:	return L"ELECTROMAGNETIC";
		case ENUM_RESIDUALCURRENT:	return L"RESIDUALCURRENT";
		case ENUM_THERMAL:	return L"THERMAL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> IfcProtectiveDeviceTrippingUnitTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum>(); }
	shared_ptr<IfcProtectiveDeviceTrippingUnitTypeEnum> type_object( new IfcProtectiveDeviceTrippingUnitTypeEnum() );
	if( boost::iequals( arg, L".ELECTRONIC." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_ELECTRONIC;
	}
	else if( boost::iequals( arg, L".ELECTROMAGNETIC." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_ELECTROMAGNETIC;
	}
	else if( boost::iequals( arg, L".RESIDUALCURRENT." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_RESIDUALCURRENT;
	}
	else if( boost::iequals( arg, L".THERMAL." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_THERMAL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcProtectiveDeviceTrippingUnitTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
