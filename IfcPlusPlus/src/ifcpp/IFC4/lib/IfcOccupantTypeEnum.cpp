/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcOccupantTypeEnum.h"

// TYPE IfcOccupantTypeEnum = ENUMERATION OF	(ASSIGNEE	,ASSIGNOR	,LESSEE	,LESSOR	,LETTINGAGENT	,OWNER	,TENANT	,USERDEFINED	,NOTDEFINED);
IfcOccupantTypeEnum::IfcOccupantTypeEnum() = default;
IfcOccupantTypeEnum::~IfcOccupantTypeEnum() = default;
shared_ptr<BuildingObject> IfcOccupantTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcOccupantTypeEnum> copy_self( new IfcOccupantTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcOccupantTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOCCUPANTTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_ASSIGNEE:	stream << ".ASSIGNEE."; break;
		case ENUM_ASSIGNOR:	stream << ".ASSIGNOR."; break;
		case ENUM_LESSEE:	stream << ".LESSEE."; break;
		case ENUM_LESSOR:	stream << ".LESSOR."; break;
		case ENUM_LETTINGAGENT:	stream << ".LETTINGAGENT."; break;
		case ENUM_OWNER:	stream << ".OWNER."; break;
		case ENUM_TENANT:	stream << ".TENANT."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcOccupantTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ASSIGNEE:	return L"ASSIGNEE";
		case ENUM_ASSIGNOR:	return L"ASSIGNOR";
		case ENUM_LESSEE:	return L"LESSEE";
		case ENUM_LESSOR:	return L"LESSOR";
		case ENUM_LETTINGAGENT:	return L"LETTINGAGENT";
		case ENUM_OWNER:	return L"OWNER";
		case ENUM_TENANT:	return L"TENANT";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcOccupantTypeEnum> IfcOccupantTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcOccupantTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcOccupantTypeEnum>(); }
	shared_ptr<IfcOccupantTypeEnum> type_object( new IfcOccupantTypeEnum() );
	if( boost::iequals( arg, L".ASSIGNEE." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_ASSIGNEE;
	}
	else if( boost::iequals( arg, L".ASSIGNOR." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_ASSIGNOR;
	}
	else if( boost::iequals( arg, L".LESSEE." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LESSEE;
	}
	else if( boost::iequals( arg, L".LESSOR." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LESSOR;
	}
	else if( boost::iequals( arg, L".LETTINGAGENT." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_LETTINGAGENT;
	}
	else if( boost::iequals( arg, L".OWNER." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_OWNER;
	}
	else if( boost::iequals( arg, L".TENANT." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_TENANT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcOccupantTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
