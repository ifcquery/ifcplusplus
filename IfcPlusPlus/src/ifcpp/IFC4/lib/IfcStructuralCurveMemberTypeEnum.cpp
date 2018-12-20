/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcStructuralCurveMemberTypeEnum.h"

// TYPE IfcStructuralCurveMemberTypeEnum = ENUMERATION OF	(RIGID_JOINED_MEMBER	,PIN_JOINED_MEMBER	,CABLE	,TENSION_MEMBER	,COMPRESSION_MEMBER	,USERDEFINED	,NOTDEFINED);
IfcStructuralCurveMemberTypeEnum::IfcStructuralCurveMemberTypeEnum() = default;
IfcStructuralCurveMemberTypeEnum::~IfcStructuralCurveMemberTypeEnum() = default;
shared_ptr<BuildingObject> IfcStructuralCurveMemberTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcStructuralCurveMemberTypeEnum> copy_self( new IfcStructuralCurveMemberTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcStructuralCurveMemberTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSTRUCTURALCURVEMEMBERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_RIGID_JOINED_MEMBER:	stream << ".RIGID_JOINED_MEMBER."; break;
		case ENUM_PIN_JOINED_MEMBER:	stream << ".PIN_JOINED_MEMBER."; break;
		case ENUM_CABLE:	stream << ".CABLE."; break;
		case ENUM_TENSION_MEMBER:	stream << ".TENSION_MEMBER."; break;
		case ENUM_COMPRESSION_MEMBER:	stream << ".COMPRESSION_MEMBER."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcStructuralCurveMemberTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_RIGID_JOINED_MEMBER:	return L"RIGID_JOINED_MEMBER";
		case ENUM_PIN_JOINED_MEMBER:	return L"PIN_JOINED_MEMBER";
		case ENUM_CABLE:	return L"CABLE";
		case ENUM_TENSION_MEMBER:	return L"TENSION_MEMBER";
		case ENUM_COMPRESSION_MEMBER:	return L"COMPRESSION_MEMBER";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcStructuralCurveMemberTypeEnum> IfcStructuralCurveMemberTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcStructuralCurveMemberTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcStructuralCurveMemberTypeEnum>(); }
	shared_ptr<IfcStructuralCurveMemberTypeEnum> type_object( new IfcStructuralCurveMemberTypeEnum() );
	if( boost::iequals( arg, L".RIGID_JOINED_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_RIGID_JOINED_MEMBER;
	}
	else if( boost::iequals( arg, L".PIN_JOINED_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_PIN_JOINED_MEMBER;
	}
	else if( boost::iequals( arg, L".CABLE." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_CABLE;
	}
	else if( boost::iequals( arg, L".TENSION_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_TENSION_MEMBER;
	}
	else if( boost::iequals( arg, L".COMPRESSION_MEMBER." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_COMPRESSION_MEMBER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcStructuralCurveMemberTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
