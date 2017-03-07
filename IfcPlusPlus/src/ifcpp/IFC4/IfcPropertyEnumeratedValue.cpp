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
#include "include/IfcComplexProperty.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertyEnumeratedValue.h"
#include "include/IfcPropertyEnumeration.h"
#include "include/IfcPropertySet.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyEnumeratedValue 
IfcPropertyEnumeratedValue::IfcPropertyEnumeratedValue() { m_entity_enum = IFCPROPERTYENUMERATEDVALUE; }
IfcPropertyEnumeratedValue::IfcPropertyEnumeratedValue( int id ) { m_id = id; m_entity_enum = IFCPROPERTYENUMERATEDVALUE; }
IfcPropertyEnumeratedValue::~IfcPropertyEnumeratedValue() {}
shared_ptr<IfcPPObject> IfcPropertyEnumeratedValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyEnumeratedValue> copy_self( new IfcPropertyEnumeratedValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_EnumerationValues.size(); ++ii )
	{
		auto item_ii = m_EnumerationValues[ii];
		if( item_ii )
		{
			copy_self->m_EnumerationValues.push_back( dynamic_pointer_cast<IfcValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_EnumerationReference ) { copy_self->m_EnumerationReference = dynamic_pointer_cast<IfcPropertyEnumeration>( m_EnumerationReference->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyEnumeratedValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYENUMERATEDVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_EnumerationValues.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcValue>& type_object = m_EnumerationValues[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_EnumerationReference ) { stream << "#" << m_EnumerationReference->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcPropertyEnumeratedValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyEnumeratedValue::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyEnumeratedValue, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readSelectList( args[2], m_EnumerationValues, map );
	readEntityReference( args[3], m_EnumerationReference, map );
}
void IfcPropertyEnumeratedValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	if( m_EnumerationValues.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> EnumerationValues_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_EnumerationValues.begin(), m_EnumerationValues.end(), std::back_inserter( EnumerationValues_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "EnumerationValues", EnumerationValues_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "EnumerationReference", m_EnumerationReference ) );
}
void IfcPropertyEnumeratedValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSimpleProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyEnumeratedValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyEnumeratedValue::unlinkFromInverseCounterparts()
{
	IfcSimpleProperty::unlinkFromInverseCounterparts();
}
