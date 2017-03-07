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
#include "include/IfcActorSelect.h"
#include "include/IfcBenchmarkEnum.h"
#include "include/IfcConstraintEnum.h"
#include "include/IfcDateTime.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMetric.h"
#include "include/IfcMetricValueSelect.h"
#include "include/IfcReference.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcMetric 
IfcMetric::IfcMetric() { m_entity_enum = IFCMETRIC; }
IfcMetric::IfcMetric( int id ) { m_id = id; m_entity_enum = IFCMETRIC; }
IfcMetric::~IfcMetric() {}
shared_ptr<IfcPPObject> IfcMetric::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMetric> copy_self( new IfcMetric() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ConstraintGrade ) { copy_self->m_ConstraintGrade = dynamic_pointer_cast<IfcConstraintEnum>( m_ConstraintGrade->getDeepCopy(options) ); }
	if( m_ConstraintSource ) { copy_self->m_ConstraintSource = dynamic_pointer_cast<IfcLabel>( m_ConstraintSource->getDeepCopy(options) ); }
	if( m_CreatingActor ) { copy_self->m_CreatingActor = dynamic_pointer_cast<IfcActorSelect>( m_CreatingActor->getDeepCopy(options) ); }
	if( m_CreationTime ) { copy_self->m_CreationTime = dynamic_pointer_cast<IfcDateTime>( m_CreationTime->getDeepCopy(options) ); }
	if( m_UserDefinedGrade ) { copy_self->m_UserDefinedGrade = dynamic_pointer_cast<IfcLabel>( m_UserDefinedGrade->getDeepCopy(options) ); }
	if( m_Benchmark ) { copy_self->m_Benchmark = dynamic_pointer_cast<IfcBenchmarkEnum>( m_Benchmark->getDeepCopy(options) ); }
	if( m_ValueSource ) { copy_self->m_ValueSource = dynamic_pointer_cast<IfcLabel>( m_ValueSource->getDeepCopy(options) ); }
	if( m_DataValue ) { copy_self->m_DataValue = dynamic_pointer_cast<IfcMetricValueSelect>( m_DataValue->getDeepCopy(options) ); }
	if( m_ReferencePath ) { copy_self->m_ReferencePath = dynamic_pointer_cast<IfcReference>( m_ReferencePath->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMetric::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMETRIC" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConstraintGrade ) { m_ConstraintGrade->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConstraintSource ) { m_ConstraintSource->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CreatingActor ) { m_CreatingActor->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_CreationTime ) { m_CreationTime->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UserDefinedGrade ) { m_UserDefinedGrade->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Benchmark ) { m_Benchmark->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ValueSource ) { m_ValueSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DataValue ) { m_DataValue->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ReferencePath ) { stream << "#" << m_ReferencePath->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcMetric::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMetric::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 11 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMetric, expecting 11, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_ConstraintGrade = IfcConstraintEnum::createObjectFromSTEP( args[2] );
	m_ConstraintSource = IfcLabel::createObjectFromSTEP( args[3] );
	m_CreatingActor = IfcActorSelect::createObjectFromSTEP( args[4], map );
	m_CreationTime = IfcDateTime::createObjectFromSTEP( args[5] );
	m_UserDefinedGrade = IfcLabel::createObjectFromSTEP( args[6] );
	m_Benchmark = IfcBenchmarkEnum::createObjectFromSTEP( args[7] );
	m_ValueSource = IfcLabel::createObjectFromSTEP( args[8] );
	m_DataValue = IfcMetricValueSelect::createObjectFromSTEP( args[9], map );
	readEntityReference( args[10], m_ReferencePath, map );
}
void IfcMetric::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcConstraint::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Benchmark", m_Benchmark ) );
	vec_attributes.push_back( std::make_pair( "ValueSource", m_ValueSource ) );
	vec_attributes.push_back( std::make_pair( "DataValue", m_DataValue ) );
	vec_attributes.push_back( std::make_pair( "ReferencePath", m_ReferencePath ) );
}
void IfcMetric::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcConstraint::getAttributesInverse( vec_attributes_inverse );
}
void IfcMetric::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcConstraint::setInverseCounterparts( ptr_self_entity );
}
void IfcMetric::unlinkFromInverseCounterparts()
{
	IfcConstraint::unlinkFromInverseCounterparts();
}
