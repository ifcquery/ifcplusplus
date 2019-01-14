/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcEventTriggerTypeEnum.h"

// TYPE IfcEventTriggerTypeEnum = ENUMERATION OF	(EVENTRULE	,EVENTMESSAGE	,EVENTTIME	,EVENTCOMPLEX	,USERDEFINED	,NOTDEFINED);
IfcEventTriggerTypeEnum::~IfcEventTriggerTypeEnum() {}
shared_ptr<BuildingObject> IfcEventTriggerTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcEventTriggerTypeEnum> copy_self( new IfcEventTriggerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcEventTriggerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCEVENTTRIGGERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_EVENTRULE:	stream << ".EVENTRULE."; break;
		case ENUM_EVENTMESSAGE:	stream << ".EVENTMESSAGE."; break;
		case ENUM_EVENTTIME:	stream << ".EVENTTIME."; break;
		case ENUM_EVENTCOMPLEX:	stream << ".EVENTCOMPLEX."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcEventTriggerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_EVENTRULE:	return L"EVENTRULE";
		case ENUM_EVENTMESSAGE:	return L"EVENTMESSAGE";
		case ENUM_EVENTTIME:	return L"EVENTTIME";
		case ENUM_EVENTCOMPLEX:	return L"EVENTCOMPLEX";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcEventTriggerTypeEnum> IfcEventTriggerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcEventTriggerTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcEventTriggerTypeEnum>(); }
	shared_ptr<IfcEventTriggerTypeEnum> type_object( new IfcEventTriggerTypeEnum() );
	if( boost::iequals( arg, L".EVENTRULE." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_EVENTRULE;
	}
	else if( boost::iequals( arg, L".EVENTMESSAGE." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_EVENTMESSAGE;
	}
	else if( boost::iequals( arg, L".EVENTTIME." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_EVENTTIME;
	}
	else if( boost::iequals( arg, L".EVENTCOMPLEX." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_EVENTCOMPLEX;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcEventTriggerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
