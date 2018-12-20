/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcSlabTypeEnum.h"

// TYPE IfcSlabTypeEnum = ENUMERATION OF	(FLOOR	,ROOF	,LANDING	,BASESLAB	,USERDEFINED	,NOTDEFINED);
IfcSlabTypeEnum::IfcSlabTypeEnum() = default;
IfcSlabTypeEnum::~IfcSlabTypeEnum() = default;
shared_ptr<BuildingObject> IfcSlabTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSlabTypeEnum> copy_self( new IfcSlabTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSlabTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSLABTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_FLOOR:	stream << ".FLOOR."; break;
		case ENUM_ROOF:	stream << ".ROOF."; break;
		case ENUM_LANDING:	stream << ".LANDING."; break;
		case ENUM_BASESLAB:	stream << ".BASESLAB."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSlabTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_FLOOR:	return L"FLOOR";
		case ENUM_ROOF:	return L"ROOF";
		case ENUM_LANDING:	return L"LANDING";
		case ENUM_BASESLAB:	return L"BASESLAB";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcSlabTypeEnum> IfcSlabTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSlabTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSlabTypeEnum>(); }
	shared_ptr<IfcSlabTypeEnum> type_object( new IfcSlabTypeEnum() );
	if( boost::iequals( arg, L".FLOOR." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_FLOOR;
	}
	else if( boost::iequals( arg, L".ROOF." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_ROOF;
	}
	else if( boost::iequals( arg, L".LANDING." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_LANDING;
	}
	else if( boost::iequals( arg, L".BASESLAB." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_BASESLAB;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSlabTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
