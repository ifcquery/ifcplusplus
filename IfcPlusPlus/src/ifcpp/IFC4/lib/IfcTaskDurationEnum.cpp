/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcTaskDurationEnum.h"

// TYPE IfcTaskDurationEnum = ENUMERATION OF	(ELAPSEDTIME	,WORKTIME	,NOTDEFINED);
IfcTaskDurationEnum::IfcTaskDurationEnum() = default;
IfcTaskDurationEnum::~IfcTaskDurationEnum() = default;
shared_ptr<BuildingObject> IfcTaskDurationEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTaskDurationEnum> copy_self( new IfcTaskDurationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTaskDurationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTASKDURATIONENUM("; }
	switch( m_enum )
	{
		case ENUM_ELAPSEDTIME:	stream << ".ELAPSEDTIME."; break;
		case ENUM_WORKTIME:	stream << ".WORKTIME."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTaskDurationEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_ELAPSEDTIME:	return L"ELAPSEDTIME";
		case ENUM_WORKTIME:	return L"WORKTIME";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcTaskDurationEnum> IfcTaskDurationEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTaskDurationEnum>(); }
	shared_ptr<IfcTaskDurationEnum> type_object( new IfcTaskDurationEnum() );
	if( boost::iequals( arg, L".ELAPSEDTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_ELAPSEDTIME;
	}
	else if( boost::iequals( arg, L".WORKTIME." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_WORKTIME;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTaskDurationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
