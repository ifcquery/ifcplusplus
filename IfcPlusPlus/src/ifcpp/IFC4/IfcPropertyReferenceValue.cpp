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
#include "include/IfcObjectReferenceSelect.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertyReferenceValue.h"
#include "include/IfcPropertySet.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"

// ENTITY IfcPropertyReferenceValue 
IfcPropertyReferenceValue::IfcPropertyReferenceValue() { m_entity_enum = IFCPROPERTYREFERENCEVALUE; }
IfcPropertyReferenceValue::IfcPropertyReferenceValue( int id ) { m_id = id; m_entity_enum = IFCPROPERTYREFERENCEVALUE; }
IfcPropertyReferenceValue::~IfcPropertyReferenceValue() {}
shared_ptr<IfcPPObject> IfcPropertyReferenceValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyReferenceValue> copy_self( new IfcPropertyReferenceValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UsageName ) { copy_self->m_UsageName = dynamic_pointer_cast<IfcText>( m_UsageName->getDeepCopy(options) ); }
	if( m_PropertyReference ) { copy_self->m_PropertyReference = dynamic_pointer_cast<IfcObjectReferenceSelect>( m_PropertyReference->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyReferenceValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYREFERENCEVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UsageName ) { m_UsageName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PropertyReference ) { m_PropertyReference->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcPropertyReferenceValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyReferenceValue::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyReferenceValue, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UsageName = IfcText::createObjectFromSTEP( args[2] );
	m_PropertyReference = IfcObjectReferenceSelect::createObjectFromSTEP( args[3], map );
}
void IfcPropertyReferenceValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UsageName", m_UsageName ) );
	vec_attributes.push_back( std::make_pair( "PropertyReference", m_PropertyReference ) );
}
void IfcPropertyReferenceValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSimpleProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyReferenceValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyReferenceValue::unlinkFromInverseCounterparts()
{
	IfcSimpleProperty::unlinkFromInverseCounterparts();
}
