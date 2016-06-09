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
#include "include/IfcComplexProperty.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyBoundedValue.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcText.h"
#include "include/IfcUnit.h"
#include "include/IfcValue.h"

// ENTITY IfcPropertyBoundedValue 
IfcPropertyBoundedValue::IfcPropertyBoundedValue() { m_entity_enum = IFCPROPERTYBOUNDEDVALUE; }
IfcPropertyBoundedValue::IfcPropertyBoundedValue( int id ) { m_id = id; m_entity_enum = IFCPROPERTYBOUNDEDVALUE; }
IfcPropertyBoundedValue::~IfcPropertyBoundedValue() {}
shared_ptr<IfcPPObject> IfcPropertyBoundedValue::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPropertyBoundedValue> copy_self( new IfcPropertyBoundedValue() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_UpperBoundValue ) { copy_self->m_UpperBoundValue = dynamic_pointer_cast<IfcValue>( m_UpperBoundValue->getDeepCopy(options) ); }
	if( m_LowerBoundValue ) { copy_self->m_LowerBoundValue = dynamic_pointer_cast<IfcValue>( m_LowerBoundValue->getDeepCopy(options) ); }
	if( m_Unit ) { copy_self->m_Unit = dynamic_pointer_cast<IfcUnit>( m_Unit->getDeepCopy(options) ); }
	if( m_SetPointValue ) { copy_self->m_SetPointValue = dynamic_pointer_cast<IfcValue>( m_SetPointValue->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPropertyBoundedValue::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPROPERTYBOUNDEDVALUE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_UpperBoundValue ) { m_UpperBoundValue->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_LowerBoundValue ) { m_LowerBoundValue->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_Unit ) { m_Unit->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_SetPointValue ) { m_SetPointValue->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcPropertyBoundedValue::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPropertyBoundedValue::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPropertyBoundedValue, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	m_UpperBoundValue = IfcValue::createObjectFromSTEP( args[2], map );
	m_LowerBoundValue = IfcValue::createObjectFromSTEP( args[3], map );
	m_Unit = IfcUnit::createObjectFromSTEP( args[4], map );
	m_SetPointValue = IfcValue::createObjectFromSTEP( args[5], map );
}
void IfcPropertyBoundedValue::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSimpleProperty::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "UpperBoundValue", m_UpperBoundValue ) );
	vec_attributes.push_back( std::make_pair( "LowerBoundValue", m_LowerBoundValue ) );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
	vec_attributes.push_back( std::make_pair( "SetPointValue", m_SetPointValue ) );
}
void IfcPropertyBoundedValue::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSimpleProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcPropertyBoundedValue::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSimpleProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcPropertyBoundedValue::unlinkFromInverseCounterparts()
{
	IfcSimpleProperty::unlinkFromInverseCounterparts();
}
