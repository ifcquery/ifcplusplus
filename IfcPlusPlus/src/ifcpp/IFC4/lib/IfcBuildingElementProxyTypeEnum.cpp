/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcBuildingElementProxyTypeEnum.h"

// TYPE IfcBuildingElementProxyTypeEnum = ENUMERATION OF	(COMPLEX	,ELEMENT	,PARTIAL	,PROVISIONFORVOID	,PROVISIONFORSPACE	,USERDEFINED	,NOTDEFINED);
IfcBuildingElementProxyTypeEnum::~IfcBuildingElementProxyTypeEnum() {}
shared_ptr<BuildingObject> IfcBuildingElementProxyTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcBuildingElementProxyTypeEnum> copy_self( new IfcBuildingElementProxyTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBuildingElementProxyTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBUILDINGELEMENTPROXYTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_COMPLEX:	stream << ".COMPLEX."; break;
		case ENUM_ELEMENT:	stream << ".ELEMENT."; break;
		case ENUM_PARTIAL:	stream << ".PARTIAL."; break;
		case ENUM_PROVISIONFORVOID:	stream << ".PROVISIONFORVOID."; break;
		case ENUM_PROVISIONFORSPACE:	stream << ".PROVISIONFORSPACE."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcBuildingElementProxyTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_COMPLEX:	return L"COMPLEX";
		case ENUM_ELEMENT:	return L"ELEMENT";
		case ENUM_PARTIAL:	return L"PARTIAL";
		case ENUM_PROVISIONFORVOID:	return L"PROVISIONFORVOID";
		case ENUM_PROVISIONFORSPACE:	return L"PROVISIONFORSPACE";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcBuildingElementProxyTypeEnum> IfcBuildingElementProxyTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBuildingElementProxyTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBuildingElementProxyTypeEnum>(); }
	shared_ptr<IfcBuildingElementProxyTypeEnum> type_object( new IfcBuildingElementProxyTypeEnum() );
	if( boost::iequals( arg, L".COMPLEX." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_COMPLEX;
	}
	else if( boost::iequals( arg, L".ELEMENT." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_ELEMENT;
	}
	else if( boost::iequals( arg, L".PARTIAL." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_PARTIAL;
	}
	else if( boost::iequals( arg, L".PROVISIONFORVOID." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_PROVISIONFORVOID;
	}
	else if( boost::iequals( arg, L".PROVISIONFORSPACE." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_PROVISIONFORSPACE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBuildingElementProxyTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
