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
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcResourceApprovalRelationship.h"
#include "include/IfcResourceConstraintRelationship.h"
#include "include/IfcSimpleProperty.h"
#include "include/IfcText.h"

// ENTITY IfcSimpleProperty 
IfcSimpleProperty::IfcSimpleProperty() { m_entity_enum = IFCSIMPLEPROPERTY; }
IfcSimpleProperty::IfcSimpleProperty( int id ) { m_id = id; m_entity_enum = IFCSIMPLEPROPERTY; }
IfcSimpleProperty::~IfcSimpleProperty() {}
shared_ptr<IfcPPObject> IfcSimpleProperty::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSimpleProperty> copy_self( new IfcSimpleProperty() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcIdentifier>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSimpleProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSIMPLEPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcSimpleProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSimpleProperty::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSimpleProperty, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcIdentifier::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
}
void IfcSimpleProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProperty::getAttributes( vec_attributes );
}
void IfcSimpleProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcProperty::getAttributesInverse( vec_attributes_inverse );
}
void IfcSimpleProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcSimpleProperty::unlinkFromInverseCounterparts()
{
	IfcProperty::unlinkFromInverseCounterparts();
}
