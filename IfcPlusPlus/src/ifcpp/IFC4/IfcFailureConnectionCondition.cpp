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
#include "include/IfcFailureConnectionCondition.h"
#include "include/IfcForceMeasure.h"
#include "include/IfcLabel.h"

// ENTITY IfcFailureConnectionCondition 
IfcFailureConnectionCondition::IfcFailureConnectionCondition() { m_entity_enum = IFCFAILURECONNECTIONCONDITION; }
IfcFailureConnectionCondition::IfcFailureConnectionCondition( int id ) { m_id = id; m_entity_enum = IFCFAILURECONNECTIONCONDITION; }
IfcFailureConnectionCondition::~IfcFailureConnectionCondition() {}
shared_ptr<IfcPPObject> IfcFailureConnectionCondition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFailureConnectionCondition> copy_self( new IfcFailureConnectionCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_TensionFailureX ) { copy_self->m_TensionFailureX = dynamic_pointer_cast<IfcForceMeasure>( m_TensionFailureX->getDeepCopy(options) ); }
	if( m_TensionFailureY ) { copy_self->m_TensionFailureY = dynamic_pointer_cast<IfcForceMeasure>( m_TensionFailureY->getDeepCopy(options) ); }
	if( m_TensionFailureZ ) { copy_self->m_TensionFailureZ = dynamic_pointer_cast<IfcForceMeasure>( m_TensionFailureZ->getDeepCopy(options) ); }
	if( m_CompressionFailureX ) { copy_self->m_CompressionFailureX = dynamic_pointer_cast<IfcForceMeasure>( m_CompressionFailureX->getDeepCopy(options) ); }
	if( m_CompressionFailureY ) { copy_self->m_CompressionFailureY = dynamic_pointer_cast<IfcForceMeasure>( m_CompressionFailureY->getDeepCopy(options) ); }
	if( m_CompressionFailureZ ) { copy_self->m_CompressionFailureZ = dynamic_pointer_cast<IfcForceMeasure>( m_CompressionFailureZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFailureConnectionCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFAILURECONNECTIONCONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TensionFailureX ) { m_TensionFailureX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionFailureY ) { m_TensionFailureY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionFailureZ ) { m_TensionFailureZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureX ) { m_CompressionFailureX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureY ) { m_CompressionFailureY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompressionFailureZ ) { m_CompressionFailureZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFailureConnectionCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFailureConnectionCondition::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFailureConnectionCondition, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_TensionFailureX = IfcForceMeasure::createObjectFromSTEP( args[1] );
	m_TensionFailureY = IfcForceMeasure::createObjectFromSTEP( args[2] );
	m_TensionFailureZ = IfcForceMeasure::createObjectFromSTEP( args[3] );
	m_CompressionFailureX = IfcForceMeasure::createObjectFromSTEP( args[4] );
	m_CompressionFailureY = IfcForceMeasure::createObjectFromSTEP( args[5] );
	m_CompressionFailureZ = IfcForceMeasure::createObjectFromSTEP( args[6] );
}
void IfcFailureConnectionCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralConnectionCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TensionFailureX", m_TensionFailureX ) );
	vec_attributes.push_back( std::make_pair( "TensionFailureY", m_TensionFailureY ) );
	vec_attributes.push_back( std::make_pair( "TensionFailureZ", m_TensionFailureZ ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureX", m_CompressionFailureX ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureY", m_CompressionFailureY ) );
	vec_attributes.push_back( std::make_pair( "CompressionFailureZ", m_CompressionFailureZ ) );
}
void IfcFailureConnectionCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralConnectionCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcFailureConnectionCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralConnectionCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcFailureConnectionCondition::unlinkFromInverseCounterparts()
{
	IfcStructuralConnectionCondition::unlinkFromInverseCounterparts();
}
