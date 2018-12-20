/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/IFC4/include/IfcTimeSeriesDataTypeEnum.h"

// TYPE IfcTimeSeriesDataTypeEnum = ENUMERATION OF	(CONTINUOUS	,DISCRETE	,DISCRETEBINARY	,PIECEWISEBINARY	,PIECEWISECONSTANT	,PIECEWISECONTINUOUS	,NOTDEFINED);
IfcTimeSeriesDataTypeEnum::IfcTimeSeriesDataTypeEnum() = default;
IfcTimeSeriesDataTypeEnum::~IfcTimeSeriesDataTypeEnum() = default;
shared_ptr<BuildingObject> IfcTimeSeriesDataTypeEnum::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcTimeSeriesDataTypeEnum> copy_self( new IfcTimeSeriesDataTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcTimeSeriesDataTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCTIMESERIESDATATYPEENUM("; }
	switch( m_enum )
	{
		case ENUM_CONTINUOUS:	stream << ".CONTINUOUS."; break;
		case ENUM_DISCRETE:	stream << ".DISCRETE."; break;
		case ENUM_DISCRETEBINARY:	stream << ".DISCRETEBINARY."; break;
		case ENUM_PIECEWISEBINARY:	stream << ".PIECEWISEBINARY."; break;
		case ENUM_PIECEWISECONSTANT:	stream << ".PIECEWISECONSTANT."; break;
		case ENUM_PIECEWISECONTINUOUS:	stream << ".PIECEWISECONTINUOUS."; break;
		case ENUM_NOTDEFINED:	stream << ".NOTDEFINED."; break;
	}
	if( is_select_type ) { stream << ")"; }
}
const std::wstring IfcTimeSeriesDataTypeEnum::toString() const
{
	switch( m_enum ) 
	{
		case ENUM_CONTINUOUS:	return L"CONTINUOUS";
		case ENUM_DISCRETE:	return L"DISCRETE";
		case ENUM_DISCRETEBINARY:	return L"DISCRETEBINARY";
		case ENUM_PIECEWISEBINARY:	return L"PIECEWISEBINARY";
		case ENUM_PIECEWISECONSTANT:	return L"PIECEWISECONSTANT";
		case ENUM_PIECEWISECONTINUOUS:	return L"PIECEWISECONTINUOUS";
		case ENUM_NOTDEFINED:	return L"NOTDEFINED";
	}
	return L"";
}
shared_ptr<IfcTimeSeriesDataTypeEnum> IfcTimeSeriesDataTypeEnum::createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcTimeSeriesDataTypeEnum>(); }
	if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcTimeSeriesDataTypeEnum>(); }
	shared_ptr<IfcTimeSeriesDataTypeEnum> type_object( new IfcTimeSeriesDataTypeEnum() );
	if( boost::iequals( arg, L".CONTINUOUS." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_CONTINUOUS;
	}
	else if( boost::iequals( arg, L".DISCRETE." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_DISCRETE;
	}
	else if( boost::iequals( arg, L".DISCRETEBINARY." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_DISCRETEBINARY;
	}
	else if( boost::iequals( arg, L".PIECEWISEBINARY." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISEBINARY;
	}
	else if( boost::iequals( arg, L".PIECEWISECONSTANT." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISECONSTANT;
	}
	else if( boost::iequals( arg, L".PIECEWISECONTINUOUS." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_PIECEWISECONTINUOUS;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcTimeSeriesDataTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
