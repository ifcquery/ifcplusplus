/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcWindowStyleConstructionEnum.h"

// TYPE IfcWindowStyleConstructionEnum = ENUMERATION OF	(ALUMINIUM	,HIGH_GRADE_STEEL	,STEEL	,WOOD	,ALUMINIUM_WOOD	,PLASTIC	,OTHER_CONSTRUCTION	,NOTDEFINED);
IfcWindowStyleConstructionEnum::IfcWindowStyleConstructionEnum() = default;
IfcWindowStyleConstructionEnum::~IfcWindowStyleConstructionEnum() = default;
shared_ptr<BuildingObject> IfcWindowStyleConstructionEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcWindowStyleConstructionEnum> copy_self( new IfcWindowStyleConstructionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcWindowStyleConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCWINDOWSTYLECONSTRUCTIONENUM("; }
	switch( m_enum )
	{
		case ENUM_ALUMINIUM:	stream << ".ALUMINIUM."; break;
		case ENUM_HIGH_GRADE_STEEL:	stream << ".HIGH_GRADE_STEEL."; break;
		case ENUM_STEEL:	stream << ".STEEL."; break;
		case ENUM_WOOD:	stream << ".WOOD."; break;
		case ENUM_ALUMINIUM_WOOD:	stream << ".ALUMINIUM_WOOD."; break;
		case ENUM_PLASTIC:	stream << ".PLASTIC."; break;
		case ENUM_OTHER_CONSTRUCTION:	stream << ".OTHER_CONSTRUCTION."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcWindowStyleConstructionEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ALUMINIUM:	return L"ALUMINIUM";
		case ENUM_HIGH_GRADE_STEEL:	return L"HIGH_GRADE_STEEL";
		case ENUM_STEEL:	return L"STEEL";
		case ENUM_WOOD:	return L"WOOD";
		case ENUM_ALUMINIUM_WOOD:	return L"ALUMINIUM_WOOD";
		case ENUM_PLASTIC:	return L"PLASTIC";
		case ENUM_OTHER_CONSTRUCTION:	return L"OTHER_CONSTRUCTION";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcWindowStyleConstructionEnum> IfcWindowStyleConstructionEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcWindowStyleConstructionEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcWindowStyleConstructionEnum>(); }
	shared_ptr<IfcWindowStyleConstructionEnum> type_object( new IfcWindowStyleConstructionEnum() );
	if( boost::iequals( arg, L".ALUMINIUM." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM;
	}
	else if( boost::iequals( arg, L".HIGH_GRADE_STEEL." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_HIGH_GRADE_STEEL;
	}
	else if( boost::iequals( arg, L".STEEL." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_STEEL;
	}
	else if( boost::iequals( arg, L".WOOD." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_WOOD;
	}
	else if( boost::iequals( arg, L".ALUMINIUM_WOOD." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_ALUMINIUM_WOOD;
	}
	else if( boost::iequals( arg, L".PLASTIC." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_PLASTIC;
	}
	else if( boost::iequals( arg, L".OTHER_CONSTRUCTION." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_OTHER_CONSTRUCTION;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcWindowStyleConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
