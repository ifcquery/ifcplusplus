/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcDataOriginEnum.h"
#include "ifcpp/IFC4/include/IfcDateTime.h"
#include "ifcpp/IFC4/include/IfcExternalReferenceRelationship.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcRegularTimeSeries.h"
#include "ifcpp/IFC4/include/IfcText.h"
#include "ifcpp/IFC4/include/IfcTimeMeasure.h"
#include "ifcpp/IFC4/include/IfcTimeSeriesDataTypeEnum.h"
#include "ifcpp/IFC4/include/IfcTimeSeriesValue.h"
#include "ifcpp/IFC4/include/IfcUnit.h"

// ENTITY IfcRegularTimeSeries 
IfcRegularTimeSeries::IfcRegularTimeSeries() = default;
IfcRegularTimeSeries::IfcRegularTimeSeries( int id ) { m_entity_id = id; }
IfcRegularTimeSeries::~IfcRegularTimeSeries() = default;
shared_ptr<BuildingObject> IfcRegularTimeSeries::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcRegularTimeSeries> copy_self( new IfcRegularTimeSeries() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_StartTime ) { copy_self->m_StartTime = dynamic_pointer_cast<IfcDateTime>( m_StartTime->getDeepCopy(options) ); }
	if( m_EndTime ) { copy_self->m_EndTime = dynamic_pointer_cast<IfcDateTime>( m_EndTime->getDeepCopy(options) ); }
	if( m_TimeSeriesDataType ) { copy_self->m_TimeSeriesDataType = dynamic_pointer_cast<IfcTimeSeriesDataTypeEnum>( m_TimeSeriesDataType->getDeepCopy(options) ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy(options) ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	if( m_TimeStep ) { copy_self->m_TimeStep = dynamic_pointer_cast<IfcTimeMeasure>( m_TimeStep->getDeepCopy(options) ); }
	for(auto item_ii : m_Values)
	{
			if( item_ii )
		{
			copy_self->m_Values.push_back( dynamic_pointer_cast<IfcTimeSeriesValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcRegularTimeSeries::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCREGULARTIMESERIES" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_StartTime ) { m_StartTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EndTime ) { m_EndTime->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TimeSeriesDataType ) { m_TimeSeriesDataType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TimeStep ) { m_TimeStep->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Values );
	stream << ");";
}
void IfcRegularTimeSeries::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcRegularTimeSeries::toString() const { return L"IfcRegularTimeSeries"; }
void IfcRegularTimeSeries::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 10 ){ std::stringstream err; err << "Wrong parameter count for entity IfcRegularTimeSeries, expecting 10, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0], map );
	m_Description = IfcText::createObjectFromSTEP( args[1], map );
	m_StartTime = IfcDateTime::createObjectFromSTEP( args[2], map );
	m_EndTime = IfcDateTime::createObjectFromSTEP( args[3], map );
	m_TimeSeriesDataType = IfcTimeSeriesDataTypeEnum::createObjectFromSTEP( args[4], map );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromSTEP( args[5], map );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromSTEP( args[6], map );
	m_Unit = IfcUnit::createObjectFromSTEP( args[7], map );
	m_TimeStep = IfcTimeMeasure::createObjectFromSTEP( args[8], map );
	readEntityReferenceList( args[9], m_Values, map );
}
void IfcRegularTimeSeries::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcTimeSeries::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "TimeStep", m_TimeStep );
	if( !m_Values.empty() )
	{
		shared_ptr<AttributeObjectVector> Values_vec_object( new AttributeObjectVector() );
		std::copy( m_Values.begin(), m_Values.end(), std::back_inserter( Values_vec_object->m_vec ) );
		vec_attributes.emplace_back( "Values", Values_vec_object );
	}
}
void IfcRegularTimeSeries::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcTimeSeries::getAttributesInverse( vec_attributes_inverse );
}
void IfcRegularTimeSeries::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcTimeSeries::setInverseCounterparts( ptr_self_entity );
}
void IfcRegularTimeSeries::unlinkFromInverseCounterparts()
{
	IfcTimeSeries::unlinkFromInverseCounterparts();
}
