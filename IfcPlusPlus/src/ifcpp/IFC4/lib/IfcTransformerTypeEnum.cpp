/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcTransformerTypeEnum.h"

// TYPE IfcTransformerTypeEnum = ENUMERATION OF	(CURRENT	,FREQUENCY	,INVERTER	,RECTIFIER	,VOLTAGE	,USERDEFINED	,NOTDEFINED);
IfcTransformerTypeEnum::IfcTransformerTypeEnum() = default;
IfcTransformerTypeEnum::~IfcTransformerTypeEnum() = default;
shared_ptr<BuildingObject> IfcTransformerTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTransformerTypeEnum> copy_self( new IfcTransformerTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTransformerTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTRANSFORMERTYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_CURRENT:	stream << ".CURRENT."; break;
		case ENUM_FREQUENCY:	stream << ".FREQUENCY."; break;
		case ENUM_INVERTER:	stream << ".INVERTER."; break;
		case ENUM_RECTIFIER:	stream << ".RECTIFIER."; break;
		case ENUM_VOLTAGE:	stream << ".VOLTAGE."; break;
		case ENUM_USERDEFINED:	stream << ".USERDEFINED."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTransformerTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_CURRENT:	return L"CURRENT";
		case ENUM_FREQUENCY:	return L"FREQUENCY";
		case ENUM_INVERTER:	return L"INVERTER";
		case ENUM_RECTIFIER:	return L"RECTIFIER";
		case ENUM_VOLTAGE:	return L"VOLTAGE";
		case ENUM_USERDEFINED:	return L"USERDEFINED";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcTransformerTypeEnum> IfcTransformerTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTransformerTypeEnum>(); }
	shared_ptr<IfcTransformerTypeEnum> type_object( new IfcTransformerTypeEnum() );
	if( boost::iequals( arg, L".CURRENT." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_CURRENT;
	}
	else if( boost::iequals( arg, L".FREQUENCY." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_FREQUENCY;
	}
	else if( boost::iequals( arg, L".INVERTER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_INVERTER;
	}
	else if( boost::iequals( arg, L".RECTIFIER." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_RECTIFIER;
	}
	else if( boost::iequals( arg, L".VOLTAGE." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_VOLTAGE;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTransformerTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
