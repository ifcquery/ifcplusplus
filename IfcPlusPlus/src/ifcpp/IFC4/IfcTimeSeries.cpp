/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
#include "include/IfcLabel.h"
#include "include/IfcText.h"
#include "include/IfcTimeSeries.h"
#include "include/IfcTimeSeriesDataTypeEnum.h"
#include "include/IfcUnit.h"

// ENTITY IfcTimeSeries 
IfcTimeSeries::IfcTimeSeries() { m_entity_enum = IFCTIMESERIES; }
IfcTimeSeries::IfcTimeSeries( int id ) { m_id = id; m_entity_enum = IFCTIMESERIES; }
IfcTimeSeries::~IfcTimeSeries() {}
shared_ptr<IfcPPObject> IfcTimeSeries::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTimeSeries> copy_self( new IfcTimeSeries() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_StartTime ) { copy_self->m_StartTime = dynamic_pointer_cast<IfcDateTime>( m_StartTime->getDeepCopy(options) ); }
	if( m_EndTime ) { copy_self->m_EndTime = dynamic_pointer_cast<IfcDateTime>( m_EndTime->getDeepCopy(options) ); }
	if( m_TimeSeriesDataType ) { copy_self->m_TimeSeriesDataType = dynamic_pointer_cast<IfcTimeSeriesDataTypeEnum>( m_TimeSeriesDataType->getDeepCopy(options) ); }
	if( m_DataOrigin ) { copy_self->m_DataOrigin = dynamic_pointer_cast<IfcDataOriginEnum>( m_DataOrigin->getDeepCopy(options) ); }
	if( m_UserDefinedDataOrigin ) { copy_self->m_UserDefinedDataOrigin = dynamic_pointer_cast<IfcLabel>( m_UserDefinedDataOrigin->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTimeSeries::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTIMESERIES" << "(";
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
	stream << ");";
}
void IfcTimeSeries::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTimeSeries::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 8 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTimeSeries, expecting 8, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_StartTime = IfcDateTime::createObjectFromSTEP( args[2] );
	m_EndTime = IfcDateTime::createObjectFromSTEP( args[3] );
	m_TimeSeriesDataType = IfcTimeSeriesDataTypeEnum::createObjectFromSTEP( args[4] );
	m_DataOrigin = IfcDataOriginEnum::createObjectFromSTEP( args[5] );
	m_UserDefinedDataOrigin = IfcLabel::createObjectFromSTEP( args[6] );
	m_Unit = IfcUnit::createObjectFromSTEP( args[7], map );
}
void IfcTimeSeries::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	vec_attributes.push_back( std::make_pair( "StartTime", m_StartTime ) );
	vec_attributes.push_back( std::make_pair( "EndTime", m_EndTime ) );
	vec_attributes.push_back( std::make_pair( "TimeSeriesDataType", m_TimeSeriesDataType ) );
	vec_attributes.push_back( std::make_pair( "DataOrigin", m_DataOrigin ) );
	vec_attributes.push_back( std::make_pair( "UserDefinedDataOrigin", m_UserDefinedDataOrigin ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcTimeSeries::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_HasExternalReference_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReference_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReference_inverse.size(); ++i )
		{
			if( !m_HasExternalReference_inverse[i].expired() )
			{
				HasExternalReference_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReference_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReference_inverse", HasExternalReference_inverse_vec_obj ) );
	}
}
void IfcTimeSeries::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcTimeSeries::unlinkFromInverseCounterparts()
{
}
