/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcChimneyTypeEnum.h"

// TYPE IfcChimneyTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
IfcChimneyTypeEnum::~IfcChimneyTypeEnum() {}
shared_ptr<BuildingObject> IfcChimneyTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcChimneyTypeEnum> copy_self( new IfcChimneyTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcChimneyTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCHIMNEYTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcChimneyTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcChimneyTypeEnum> IfcChimneyTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcChimneyTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcChimneyTypeEnum>(); }
	shared_ptr<IfcChimneyTypeEnum> type_object( new IfcChimneyTypeEnum() );
	if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcChimneyTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcChimneyTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
