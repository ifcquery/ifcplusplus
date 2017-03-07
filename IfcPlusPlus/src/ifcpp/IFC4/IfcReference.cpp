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
#include "include/IfcIdentifier.h"
#include "include/IfcInteger.h"
#include "include/IfcLabel.h"
#include "include/IfcReference.h"

// ENTITY IfcReference 
IfcReference::IfcReference() { m_entity_enum = IFCREFERENCE; }
IfcReference::IfcReference( int id ) { m_id = id; m_entity_enum = IFCREFERENCE; }
IfcReference::~IfcReference() {}
shared_ptr<IfcPPObject> IfcReference::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReference> copy_self( new IfcReference() );
	if( m_TypeIdentifier ) { copy_self->m_TypeIdentifier = dynamic_pointer_cast<IfcIdentifier>( m_TypeIdentifier->getDeepCopy(options) ); }
	if( m_AttributeIdentifier ) { copy_self->m_AttributeIdentifier = dynamic_pointer_cast<IfcIdentifier>( m_AttributeIdentifier->getDeepCopy(options) ); }
	if( m_InstanceName ) { copy_self->m_InstanceName = dynamic_pointer_cast<IfcLabel>( m_InstanceName->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_ListPositions.size(); ++ii )
	{
		auto item_ii = m_ListPositions[ii];
		if( item_ii )
		{
			copy_self->m_ListPositions.push_back( dynamic_pointer_cast<IfcInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_InnerReference ) { copy_self->m_InnerReference = dynamic_pointer_cast<IfcReference>( m_InnerReference->getDeepCopy(options) ); }
	return copy_self;
}
void IfcReference::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREFERENCE" << "(";
	if( m_TypeIdentifier ) { m_TypeIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AttributeIdentifier ) { m_AttributeIdentifier->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InstanceName ) { m_InstanceName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeNumericTypeList( stream, m_ListPositions );
	stream << ",";
	if( m_InnerReference ) { stream << "#" << m_InnerReference->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcReference::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReference::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcReference, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_TypeIdentifier = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_AttributeIdentifier = IfcIdentifier::createObjectFromSTEP( args[1] );
	m_InstanceName = IfcLabel::createObjectFromSTEP( args[2] );
	readTypeOfIntList( args[3], m_ListPositions );
	readEntityReference( args[4], m_InnerReference, map );
}
void IfcReference::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	vec_attributes.push_back( std::make_pair( "TypeIdentifier", m_TypeIdentifier ) );
	vec_attributes.push_back( std::make_pair( "AttributeIdentifier", m_AttributeIdentifier ) );
	vec_attributes.push_back( std::make_pair( "InstanceName", m_InstanceName ) );
	if( m_ListPositions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> ListPositions_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_ListPositions.begin(), m_ListPositions.end(), std::back_inserter( ListPositions_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "ListPositions", ListPositions_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "InnerReference", m_InnerReference ) );
}
void IfcReference::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcReference::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcReference::unlinkFromInverseCounterparts()
{
}
