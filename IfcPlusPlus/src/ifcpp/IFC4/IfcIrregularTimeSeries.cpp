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
#include "include/IfcIrregularTimeSeries.h"
#include "include/IfcIrregularTimeSeriesValue.h"
#include "include/IfcLabel.h"
#include "include/IfcText.h"
#include "include/IfcTimeSeriesDataTypeEnum.h"
#include "include/IfcUnit.h"

// ENTITY IfcIrregularTimeSeries 
IfcIrregularTimeSeries::IfcIrregularTimeSeries() {}
IfcIrregularTimeSeries::IfcIrregularTimeSeries( int id ) { m_id = id; }
IfcIrregularTimeSeries::~IfcIrregularTimeSeries() {}

// method setEntity takes over all attributes from another instance of the class
void IfcIrregularTimeSeries::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcIrregularTimeSeries> other = dynamic_pointer_cast<IfcIrregularTimeSeries>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_StartTime = other->m_StartTime;
	m_EndTime = other->m_EndTime;
	m_TimeSeriesDataType = other->m_TimeSeriesDataType;
	m_DataOrigin = other->m_DataOrigin;
	m_UserDefinedDataOrigin = other->m_UserDefinedDataOrigin;
	m_Unit = other->m_Unit;
	m_Values = other->m_Values;
}
void IfcIrregularTimeSeries::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCIRREGULARTIMESERIES" << "(";
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
	writeEntityList( stream, m_Values );
	stream << ");";
}
void IfcIrregularTimeSeries::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIrregularTimeSeries::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIrregularTimeSeries, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcIrregularTimeSeries, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_StartTime = IfcDateTime::createObjectFromStepData( args[2] );
	m_EndTime = IfcDateTime::createObjectFromStepData( args[3] );
	m_TimeSeriesDataType = IfcTimeSeriesDataTypeEnum::createObjectFromStepData( args[4] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromStepData( args[5] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromStepData( args[6] );
	m_Unit = IfcUnit::createObjectFromStepData( args[7], map );
	readEntityReferenceList( args[8], m_Values, map );
}
void IfcIrregularTimeSeries::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTimeSeries::getAttributes( vec_attributes );
}
void IfcIrregularTimeSeries::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcIrregularTimeSeries::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTimeSeries::setInverseCounterparts( ptr_self_entity );
}
void IfcIrregularTimeSeries::unlinkSelf()
{
	IfcTimeSeries::unlinkSelf();
}
