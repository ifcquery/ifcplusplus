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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNamedUnit.h"
#include "include/IfcPhysicalComplexQuantity.h"
#include "include/IfcPhysicalSimpleQuantity.h"
#include "include/IfcText.h"

// ENTITY IfcPhysicalSimpleQuantity 
IfcPhysicalSimpleQuantity::IfcPhysicalSimpleQuantity() {}
IfcPhysicalSimpleQuantity::IfcPhysicalSimpleQuantity( int id ) { m_id = id; }
IfcPhysicalSimpleQuantity::~IfcPhysicalSimpleQuantity() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPhysicalSimpleQuantity::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPhysicalSimpleQuantity> other = dynamic_pointer_cast<IfcPhysicalSimpleQuantity>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Unit = other->m_Unit;
}
void IfcPhysicalSimpleQuantity::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPHYSICALSIMPLEQUANTITY" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { stream << "#" << m_Unit->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcPhysicalSimpleQuantity::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPhysicalSimpleQuantity::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPhysicalSimpleQuantity, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcPhysicalSimpleQuantity, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Unit, map );
}
void IfcPhysicalSimpleQuantity::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPhysicalQuantity::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Unit", m_Unit ) );
}
void IfcPhysicalSimpleQuantity::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPhysicalSimpleQuantity::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPhysicalQuantity::setInverseCounterparts( ptr_self_entity );
}
void IfcPhysicalSimpleQuantity::unlinkSelf()
{
	IfcPhysicalQuantity::unlinkSelf();
}
