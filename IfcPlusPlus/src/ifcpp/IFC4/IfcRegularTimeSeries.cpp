/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcDataOriginEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcRegularTimeSeries.h"
#include "include/IfcText.h"
#include "include/IfcTimeMeasure.h"
#include "include/IfcTimeSeriesDataTypeEnum.h"
#include "include/IfcTimeSeriesValue.h"
#include "include/IfcUnit.h"

// ENTITY IfcRegularTimeSeries 
IfcRegularTimeSeries::IfcRegularTimeSeries() {}
IfcRegularTimeSeries::IfcRegularTimeSeries( int id ) { m_id = id; }
IfcRegularTimeSeries::~IfcRegularTimeSeries() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRegularTimeSeries::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRegularTimeSeries> other = dynamic_pointer_cast<IfcRegularTimeSeries>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_StartTime = other->m_StartTime;
	m_EndTime = other->m_EndTime;
	m_TimeSeriesDataType = other->m_TimeSeriesDataType;
	m_DataOrigin = other->m_DataOrigin;
	m_UserDefinedDataOrigin = other->m_UserDefinedDataOrigin;
	m_Unit = other->m_Unit;
	m_TimeStep = other->m_TimeStep;
	m_Values = other->m_Values;
}
void IfcRegularTimeSeries::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREGULARTIMESERIES" << "(";
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
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TimeStep ) { m_TimeStep->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Values );
	stream << ");";
}
void IfcRegularTimeSeries::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRegularTimeSeries::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRegularTimeSeries, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcRegularTimeSeries, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_StartTime = IfcDateTime::createObjectFromStepData( args[2] );
	m_EndTime = IfcDateTime::createObjectFromStepData( args[3] );
	m_TimeSeriesDataType = IfcTimeSeriesDataTypeEnum::createObjectFromStepData( args[4] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[5] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[6] );
	m_Unit = IfcUnit::createObjectFromStepData( args[7], map );
	m_TimeStep = IfcTimeMeasure::createObjectFromStepData( args[8] );
	readEntityReferenceList( args[9], m_Values, map );
}
void IfcRegularTimeSeries::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTimeSeries::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TimeStep", m_TimeStep ) );
}
void IfcRegularTimeSeries::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRegularTimeSeries::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTimeSeries::setInverseCounterparts( ptr_self_entity );
}
void IfcRegularTimeSeries::unlinkSelf()
{
	IfcTimeSeries::unlinkSelf();
}
