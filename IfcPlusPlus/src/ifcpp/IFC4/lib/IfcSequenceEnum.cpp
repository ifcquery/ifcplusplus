/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcSequenceEnum.h"

// TYPE IfcSequenceEnum = ENUMERATION OF	(START_START	,START_FINISH	,FINISH_START	,FINISH_FINISH	,USERDEFINED	,NOTDEFINED);
IfcSequenceEnum::~IfcSequenceEnum() {}
shared_ptr<BuildingObject> IfcSequenceEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSequenceEnum> copy_self( new IfcSequenceEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSequenceEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSEQUENCEENUM("; }
	switch( m_enum )
	{
		case ENUM_START_START:	stream << ".START_START."; break;
		case ENUM_START_FINISH:	stream << ".START_FINISH."; break;
		case ENUM_FINISH_START:	stream << ".FINISH_START."; break;
		case ENUM_FINISH_FINISH:	stream << ".FINISH_FINISH."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSequenceEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_START_START:	return L"START_START";
		case ENUM_START_FINISH:	return L"START_FINISH";
		case ENUM_FINISH_START:	return L"FINISH_START";
		case ENUM_FINISH_FINISH:	return L"FINISH_FINISH";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcSequenceEnum> IfcSequenceEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSequenceEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSequenceEnum>(); }
	shared_ptr<IfcSequenceEnum> type_object( new IfcSequenceEnum() );
	if( boost::iequals( arg, L".START_START." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_START_START;
	}
	else if( boost::iequals( arg, L".START_FINISH." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_START_FINISH;
	}
	else if( boost::iequals( arg, L".FINISH_START." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_FINISH_START;
	}
	else if( boost::iequals( arg, L".FINISH_FINISH." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_FINISH_FINISH;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcSequenceEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
