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
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcComplexProperty.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcIdentifier.h"
#include "include/IfcPropertyDependencyRelationship.h"
#include "include/IfcPropertySet.h"
#include "include/IfcSimpleProperty.h"
#include "include/IfcText.h"

// ENTITY IfcSimpleProperty 
IfcSimpleProperty::IfcSimpleProperty() {}
IfcSimpleProperty::IfcSimpleProperty( int id ) { m_id = id; }
IfcSimpleProperty::~IfcSimpleProperty() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSimpleProperty::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSimpleProperty> other = dynamic_pointer_cast<IfcSimpleProperty>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
}
void IfcSimpleProperty::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSIMPLEPROPERTY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSimpleProperty::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSimpleProperty::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSimpleProperty, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcSimpleProperty, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcIdentifier::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
}
void IfcSimpleProperty::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProperty::getAttributes( vec_attributes );
}
void IfcSimpleProperty::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSimpleProperty::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProperty::setInverseCounterparts( ptr_self_entity );
}
void IfcSimpleProperty::unlinkSelf()
{
	IfcProperty::unlinkSelf();
}
