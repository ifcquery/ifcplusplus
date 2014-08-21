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
#include "include/IfcObjectReferenceSelect.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertyReferenceValue.h"
#include "include/IfcPropertySet.h"
#include "include/IfcText.h"

// ENTITY IfcPropertyReferenceValue 
IfcPropertyReferenceValue::IfcPropertyReferenceValue() {}
IfcPropertyReferenceValue::IfcPropertyReferenceValue( int id ) { m_id = id; }
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
void IfcPropertyReferenceValue::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPropertyReferenceValue, expecting 4, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
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
void IfcPropertyReferenceValue::unlinkSelf()
{
	IfcSimpleProperty::unlinkSelf();
}
