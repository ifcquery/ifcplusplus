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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcIrregularTimeSeries::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIrregularTimeSeries> copy_self( new IfcIrregularTimeSeries() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_StartTime ) { copy_self->m_StartTime = dynamic_pointer_cast<IfcDateTime>( m_StartTime->getDeepCopy(options) ); }
	if( m_EndTime ) { copy_self->m_EndTime = dynamic_pointer_cast<IfcDateTime>( m_EndTime->getDeepCopy(options) ); }
	if( m_TimeSeriesDataType ) { copy_self->m_TimeSeriesDataType = dynamic_pointer_cast<IfcTimeSeriesDataTypeEnum>( m_TimeSeriesDataType->getDeepCopy(options) ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy(options) ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Values.size(); ++ii )
	{
		auto item_ii = m_Values[ii];
		if( item_ii )
		{
			copy_self->m_Values.push_back( dynamic_pointer_cast<IfcIrregularTimeSeriesValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcIrregularTimeSeries::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCIRREGULARTIMESERIES" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_StartTime ) { m_StartTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_EndTime ) { m_EndTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TimeSeriesDataType ) { m_TimeSeriesDataType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DataOrigin ) { m_DataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedDataOrigin ) { m_UserDefinedDataOrigin->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	writeEntityList( stream, m_Values );
	stream << ");";
}
void IfcIrregularTimeSeries::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIrregularTimeSeries::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIrregularTimeSeries, expecting 9, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_StartTime = IfcDateTime::createObjectFromSTEP( args[2] );
	m_EndTime = IfcDateTime::createObjectFromSTEP( args[3] );
	m_TimeSeriesDataType = IfcTimeSeriesDataTypeEnum::createObjectFromSTEP( args[4] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromSTEP( args[5] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromSTEP( args[6] );
	m_Unit = IfcUnit::createObjectFromSTEP( args[7], map );
	readEntityReferenceList( args[8], m_Values, map );
}
void IfcIrregularTimeSeries::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTimeSeries::getAttributes( vec_attributes );
	if( m_Values.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Values_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Values.begin(), m_Values.end(), std::back_inserter( Values_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Values", Values_vec_object ) );
	}
}
void IfcIrregularTimeSeries::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTimeSeries::getAttributesInverse( vec_attributes_inverse );
}
void IfcIrregularTimeSeries::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTimeSeries::setInverseCounterparts( ptr_self_entity );
}
void IfcIrregularTimeSeries::unlinkSelf()
{
	IfcTimeSeries::unlinkSelf();
}
