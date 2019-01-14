/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcGridTypeEnum.h"

// TYPE IfcGridTypeEnum = ENUMERATION OF	(RECTANGULAR	,RADIAL	,TRIANGULAR	,IRREGULAR	,USERDEFINED	,NOTDEFINED);
IfcGridTypeEnum::~IfcGridTypeEnum() {}
shared_ptr<BuildingObject> IfcGridTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcGridTypeEnum> copy_self( new IfcGridTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcGridTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCGRIDTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_RECTANGULAR:	stream << ".RECTANGULAR."; break;
		case ENUM_RADIAL:	stream << ".RADIAL."; break;
		case ENUM_TRIANGULAR:	stream << ".TRIANGULAR."; break;
		case ENUM_IRREGULAR:	stream << ".IRREGULAR."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcGridTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_RECTANGULAR:	return L"RECTANGULAR";
		case ENUM_RADIAL:	return L"RADIAL";
		case ENUM_TRIANGULAR:	return L"TRIANGULAR";
		case ENUM_IRREGULAR:	return L"IRREGULAR";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcGridTypeEnum> IfcGridTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcGridTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcGridTypeEnum>(); }
	shared_ptr<IfcGridTypeEnum> type_object( new IfcGridTypeEnum() );
	if( boost::iequals( arg, L".RECTANGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RECTANGULAR;
	}
	else if( boost::iequals( arg, L".RADIAL." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_RADIAL;
	}
	else if( boost::iequals( arg, L".TRIANGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_TRIANGULAR;
	}
	else if( boost::iequals( arg, L".IRREGULAR." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_IRREGULAR;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcGridTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
