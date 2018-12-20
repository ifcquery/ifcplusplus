/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcReinforcingBarTypeEnum.h"

// TYPE IfcReinforcingBarTypeEnum = ENUMERATION OF	(ANCHORING	,EDGE	,LIGATURE	,MAIN	,PUNCHING	,RING	,SHEAR	,STUD	,USERDEFINED	,NOTDEFINED);
IfcReinforcingBarTypeEnum::IfcReinforcingBarTypeEnum() = default;
IfcReinforcingBarTypeEnum::~IfcReinforcingBarTypeEnum() = default;
shared_ptr<BuildingObject> IfcReinforcingBarTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcReinforcingBarTypeEnum> copy_self( new IfcReinforcingBarTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_ANCHORING:	stream << ".ANCHORING."; break;
		case ENUM_EDGE:	stream << ".EDGE."; break;
		case ENUM_LIGATURE:	stream << ".LIGATURE."; break;
		case ENUM_MAIN:	stream << ".MAIN."; break;
		case ENUM_PUNCHING:	stream << ".PUNCHING."; break;
		case ENUM_RING:	stream << ".RING."; break;
		case ENUM_SHEAR:	stream << ".SHEAR."; break;
		case ENUM_STUD:	stream << ".STUD."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcReinforcingBarTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ANCHORING:	return L"ANCHORING";
		case ENUM_EDGE:	return L"EDGE";
		case ENUM_LIGATURE:	return L"LIGATURE";
		case ENUM_MAIN:	return L"MAIN";
		case ENUM_PUNCHING:	return L"PUNCHING";
		case ENUM_RING:	return L"RING";
		case ENUM_SHEAR:	return L"SHEAR";
		case ENUM_STUD:	return L"STUD";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcReinforcingBarTypeEnum> IfcReinforcingBarTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	shared_ptr<IfcReinforcingBarTypeEnum> type_object( new IfcReinforcingBarTypeEnum() );
	if( boost::iequals( arg, L".ANCHORING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_ANCHORING;
	}
	else if( boost::iequals( arg, L".EDGE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_EDGE;
	}
	else if( boost::iequals( arg, L".LIGATURE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_LIGATURE;
	}
	else if( boost::iequals( arg, L".MAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_MAIN;
	}
	else if( boost::iequals( arg, L".PUNCHING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_PUNCHING;
	}
	else if( boost::iequals( arg, L".RING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_RING;
	}
	else if( boost::iequals( arg, L".SHEAR." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_SHEAR;
	}
	else if( boost::iequals( arg, L".STUD." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_STUD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
