/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcJunctionBoxTypeEnum.h"

// TYPE IfcJunctionBoxTypeEnum = ENUMERATION OF	(DATA	,POWER	,USERDEFINED	,NOTDEFINED);
IfcJunctionBoxTypeEnum::~IfcJunctionBoxTypeEnum() {}
shared_ptr<BuildingObject> IfcJunctionBoxTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcJunctionBoxTypeEnum> copy_self( new IfcJunctionBoxTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcJunctionBoxTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCJUNCTIONBOXTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_DATA:	stream << ".DATA."; break;
		case ENUM_POWER:	stream << ".POWER."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcJunctionBoxTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_DATA:	return L"DATA";
		case ENUM_POWER:	return L"POWER";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcJunctionBoxTypeEnum> IfcJunctionBoxTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcJunctionBoxTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcJunctionBoxTypeEnum>(); }
	shared_ptr<IfcJunctionBoxTypeEnum> type_object( new IfcJunctionBoxTypeEnum() );
	if( boost::iequals( arg, L".DATA." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_DATA;
	}
	else if( boost::iequals( arg, L".POWER." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_POWER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcJunctionBoxTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
