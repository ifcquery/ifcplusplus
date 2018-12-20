/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcTankTypeEnum.h"

// TYPE IfcTankTypeEnum = ENUMERATION OF	(BASIN	,BREAKPRESSURE	,EXPANSION	,FEEDANDEXPANSION	,PRESSUREVESSEL	,STORAGE	,VESSEL	,USERDEFINED	,NOTDEFINED);
IfcTankTypeEnum::IfcTankTypeEnum() = default;
IfcTankTypeEnum::~IfcTankTypeEnum() = default;
shared_ptr<BuildingObject> IfcTankTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTankTypeEnum> copy_self( new IfcTankTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTankTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTANKTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_BASIN:	stream << ".BASIN."; break;
		case ENUM_BREAKPRESSURE:	stream << ".BREAKPRESSURE."; break;
		case ENUM_EXPANSION:	stream << ".EXPANSION."; break;
		case ENUM_FEEDANDEXPANSION:	stream << ".FEEDANDEXPANSION."; break;
		case ENUM_PRESSUREVESSEL:	stream << ".PRESSUREVESSEL."; break;
		case ENUM_STORAGE:	stream << ".STORAGE."; break;
		case ENUM_VESSEL:	stream << ".VESSEL."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTankTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_BASIN:	return L"BASIN";
		case ENUM_BREAKPRESSURE:	return L"BREAKPRESSURE";
		case ENUM_EXPANSION:	return L"EXPANSION";
		case ENUM_FEEDANDEXPANSION:	return L"FEEDANDEXPANSION";
		case ENUM_PRESSUREVESSEL:	return L"PRESSUREVESSEL";
		case ENUM_STORAGE:	return L"STORAGE";
		case ENUM_VESSEL:	return L"VESSEL";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcTankTypeEnum> IfcTankTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTankTypeEnum>(); }
	shared_ptr<IfcTankTypeEnum> type_object( new IfcTankTypeEnum() );
	if( boost::iequals( arg, L".BASIN." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BASIN;
	}
	else if( boost::iequals( arg, L".BREAKPRESSURE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_BREAKPRESSURE;
	}
	else if( boost::iequals( arg, L".EXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_EXPANSION;
	}
	else if( boost::iequals( arg, L".FEEDANDEXPANSION." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_FEEDANDEXPANSION;
	}
	else if( boost::iequals( arg, L".PRESSUREVESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_PRESSUREVESSEL;
	}
	else if( boost::iequals( arg, L".STORAGE." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_STORAGE;
	}
	else if( boost::iequals( arg, L".VESSEL." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_VESSEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTankTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
