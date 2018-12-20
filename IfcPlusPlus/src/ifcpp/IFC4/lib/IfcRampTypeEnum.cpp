/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcRampTypeEnum.h"

// TYPE IfcRampTypeEnum = ENUMERATION OF	(STRAIGHT_RUN_RAMP	,TWO_STRAIGHT_RUN_RAMP	,QUARTER_TURN_RAMP	,TWO_QUARTER_TURN_RAMP	,HALF_TURN_RAMP	,SPIRAL_RAMP	,USERDEFINED	,NOTDEFINED);
IfcRampTypeEnum::IfcRampTypeEnum() = default;
IfcRampTypeEnum::~IfcRampTypeEnum() = default;
shared_ptr<BuildingObject> IfcRampTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcRampTypeEnum> copy_self( new IfcRampTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRampTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCRAMPTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_STRAIGHT_RUN_RAMP:	stream << ".STRAIGHT_RUN_RAMP."; break;
		case ENUM_TWO_STRAIGHT_RUN_RAMP:	stream << ".TWO_STRAIGHT_RUN_RAMP."; break;
		case ENUM_QUARTER_TURN_RAMP:	stream << ".QUARTER_TURN_RAMP."; break;
		case ENUM_TWO_QUARTER_TURN_RAMP:	stream << ".TWO_QUARTER_TURN_RAMP."; break;
		case ENUM_HALF_TURN_RAMP:	stream << ".HALF_TURN_RAMP."; break;
		case ENUM_SPIRAL_RAMP:	stream << ".SPIRAL_RAMP."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcRampTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_STRAIGHT_RUN_RAMP:	return L"STRAIGHT_RUN_RAMP";
		case ENUM_TWO_STRAIGHT_RUN_RAMP:	return L"TWO_STRAIGHT_RUN_RAMP";
		case ENUM_QUARTER_TURN_RAMP:	return L"QUARTER_TURN_RAMP";
		case ENUM_TWO_QUARTER_TURN_RAMP:	return L"TWO_QUARTER_TURN_RAMP";
		case ENUM_HALF_TURN_RAMP:	return L"HALF_TURN_RAMP";
		case ENUM_SPIRAL_RAMP:	return L"SPIRAL_RAMP";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcRampTypeEnum> IfcRampTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRampTypeEnum>(); }
	shared_ptr<IfcRampTypeEnum> type_object( new IfcRampTypeEnum() );
	if( boost::iequals( arg, L".STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_STRAIGHT_RUN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_STRAIGHT_RUN_RAMP;
	}
	else if( boost::iequals( arg, L".QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".TWO_QUARTER_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_TWO_QUARTER_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".HALF_TURN_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_HALF_TURN_RAMP;
	}
	else if( boost::iequals( arg, L".SPIRAL_RAMP." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_SPIRAL_RAMP;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRampTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
