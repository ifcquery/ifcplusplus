/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcSimplePropertyTemplateTypeEnum.h"

// TYPE IfcSimplePropertyTemplateTypeEnum = ENUMERATION OF	(P_SINGLEVALUE	,P_ENUMERATEDVALUE	,P_BOUNDEDVALUE	,P_LISTVALUE	,P_TABLEVALUE	,P_REFERENCEVALUE	,Q_LENGTH	,Q_AREA	,Q_VOLUME	,Q_COUNT	,Q_WEIGHT	,Q_TIME);
IfcSimplePropertyTemplateTypeEnum::IfcSimplePropertyTemplateTypeEnum() = default;
IfcSimplePropertyTemplateTypeEnum::~IfcSimplePropertyTemplateTypeEnum() = default;
shared_ptr<BuildingObject> IfcSimplePropertyTemplateTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcSimplePropertyTemplateTypeEnum> copy_self( new IfcSimplePropertyTemplateTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSimplePropertyTemplateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSIMPLEPROPERTYTEMPLATETYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_P_SINGLEVALUE:	stream << ".P_SINGLEVALUE."; break;
		case ENUM_P_ENUMERATEDVALUE:	stream << ".P_ENUMERATEDVALUE."; break;
		case ENUM_P_BOUNDEDVALUE:	stream << ".P_BOUNDEDVALUE."; break;
		case ENUM_P_LISTVALUE:	stream << ".P_LISTVALUE."; break;
		case ENUM_P_TABLEVALUE:	stream << ".P_TABLEVALUE."; break;
		case ENUM_P_REFERENCEVALUE:	stream << ".P_REFERENCEVALUE."; break;
		case ENUM_Q_LENGTH:	stream << ".Q_LENGTH."; break;
		case ENUM_Q_AREA:	stream << ".Q_AREA."; break;
		case ENUM_Q_VOLUME:	stream << ".Q_VOLUME."; break;
		case ENUM_Q_COUNT:	stream << ".Q_COUNT."; break;
		case ENUM_Q_WEIGHT:	stream << ".Q_WEIGHT."; break;
		case ENUM_Q_TIME:	stream << ".Q_TIME."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcSimplePropertyTemplateTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_P_SINGLEVALUE:	return L"P_SINGLEVALUE";
		case ENUM_P_ENUMERATEDVALUE:	return L"P_ENUMERATEDVALUE";
		case ENUM_P_BOUNDEDVALUE:	return L"P_BOUNDEDVALUE";
		case ENUM_P_LISTVALUE:	return L"P_LISTVALUE";
		case ENUM_P_TABLEVALUE:	return L"P_TABLEVALUE";
		case ENUM_P_REFERENCEVALUE:	return L"P_REFERENCEVALUE";
		case ENUM_Q_LENGTH:	return L"Q_LENGTH";
		case ENUM_Q_AREA:	return L"Q_AREA";
		case ENUM_Q_VOLUME:	return L"Q_VOLUME";
		case ENUM_Q_COUNT:	return L"Q_COUNT";
		case ENUM_Q_WEIGHT:	return L"Q_WEIGHT";
		case ENUM_Q_TIME:	return L"Q_TIME";
	}
	return L"";
}
shared_ptr<IfcSimplePropertyTemplateTypeEnum> IfcSimplePropertyTemplateTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSimplePropertyTemplateTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSimplePropertyTemplateTypeEnum>(); }
	shared_ptr<IfcSimplePropertyTemplateTypeEnum> type_object( new IfcSimplePropertyTemplateTypeEnum() );
	if( boost::iequals( arg, L".P_SINGLEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_SINGLEVALUE;
	}
	else if( boost::iequals( arg, L".P_ENUMERATEDVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_ENUMERATEDVALUE;
	}
	else if( boost::iequals( arg, L".P_BOUNDEDVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_BOUNDEDVALUE;
	}
	else if( boost::iequals( arg, L".P_LISTVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_LISTVALUE;
	}
	else if( boost::iequals( arg, L".P_TABLEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_TABLEVALUE;
	}
	else if( boost::iequals( arg, L".P_REFERENCEVALUE." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_P_REFERENCEVALUE;
	}
	else if( boost::iequals( arg, L".Q_LENGTH." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_LENGTH;
	}
	else if( boost::iequals( arg, L".Q_AREA." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_AREA;
	}
	else if( boost::iequals( arg, L".Q_VOLUME." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_VOLUME;
	}
	else if( boost::iequals( arg, L".Q_COUNT." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_COUNT;
	}
	else if( boost::iequals( arg, L".Q_WEIGHT." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_WEIGHT;
	}
	else if( boost::iequals( arg, L".Q_TIME." ) )
	{
		type_object->m_enum = IfcSimplePropertyTemplateTypeEnum::ENUM_Q_TIME;
	}
	return type_object;
}
