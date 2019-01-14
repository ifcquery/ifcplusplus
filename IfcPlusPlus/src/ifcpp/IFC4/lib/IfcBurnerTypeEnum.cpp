/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcBurnerTypeEnum.h"

// TYPE IfcBurnerTypeEnum = ENUMERATION OF	(USERDEFINED	,NOTDEFINED);
IfcBurnerTypeEnum::~IfcBurnerTypeEnum() {}
shared_ptr<BuildingObject> IfcBurnerTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcBurnerTypeEnum> copy_self( new IfcBurnerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBurnerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBURNERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcBurnerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcBurnerTypeEnum> IfcBurnerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBurnerTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBurnerTypeEnum>(); }
	shared_ptr<IfcBurnerTypeEnum> type_object( new IfcBurnerTypeEnum() );
	if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBurnerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBurnerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
