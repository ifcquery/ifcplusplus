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
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcSlippageConnectionCondition.h"

// ENTITY IfcSlippageConnectionCondition 
IfcSlippageConnectionCondition::IfcSlippageConnectionCondition() { m_entity_enum = IFCSLIPPAGECONNECTIONCONDITION; }
IfcSlippageConnectionCondition::IfcSlippageConnectionCondition( int id ) { m_id = id; m_entity_enum = IFCSLIPPAGECONNECTIONCONDITION; }
IfcSlippageConnectionCondition::~IfcSlippageConnectionCondition() {}
shared_ptr<IfcPPObject> IfcSlippageConnectionCondition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSlippageConnectionCondition> copy_self( new IfcSlippageConnectionCondition() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_SlippageX ) { copy_self->m_SlippageX = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageX->getDeepCopy(options) ); }
	if( m_SlippageY ) { copy_self->m_SlippageY = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageY->getDeepCopy(options) ); }
	if( m_SlippageZ ) { copy_self->m_SlippageZ = dynamic_pointer_cast<IfcLengthMeasure>( m_SlippageZ->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSlippageConnectionCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSLIPPAGECONNECTIONCONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_SlippageX ) { m_SlippageX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SlippageY ) { m_SlippageY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SlippageZ ) { m_SlippageZ->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSlippageConnectionCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSlippageConnectionCondition::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSlippageConnectionCondition, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_SlippageX = IfcLengthMeasure::createObjectFromSTEP( args[1] );
	m_SlippageY = IfcLengthMeasure::createObjectFromSTEP( args[2] );
	m_SlippageZ = IfcLengthMeasure::createObjectFromSTEP( args[3] );
}
void IfcSlippageConnectionCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralConnectionCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SlippageX", m_SlippageX ) );
	vec_attributes.push_back( std::make_pair( "SlippageY", m_SlippageY ) );
	vec_attributes.push_back( std::make_pair( "SlippageZ", m_SlippageZ ) );
}
void IfcSlippageConnectionCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralConnectionCondition::getAttributesInverse( vec_attributes_inverse );
}
void IfcSlippageConnectionCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralConnectionCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcSlippageConnectionCondition::unlinkFromInverseCounterparts()
{
	IfcStructuralConnectionCondition::unlinkFromInverseCounterparts();
}
