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
#include "include/IfcLengthMeasure.h"
#include "include/IfcNamedUnit.h"
#include "include/IfcPhysicalComplexQuantity.h"
#include "include/IfcQuantityLength.h"
#include "include/IfcText.h"

// ENTITY IfcQuantityLength 
IfcQuantityLength::IfcQuantityLength() {}
IfcQuantityLength::IfcQuantityLength( int id ) { m_id = id; }
IfcQuantityLength::~IfcQuantityLength() {}

// method setEntity takes over all attributes from another instance of the class
void IfcQuantityLength::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcQuantityLength> other = dynamic_pointer_cast<IfcQuantityLength>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_Unit = other->m_Unit;
	m_LengthValue = other->m_LengthValue;
	m_Formula = other->m_Formula;
}
void IfcQuantityLength::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCQUANTITYLENGTH" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Unit ) { stream << "#" << m_Unit->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_LengthValue ) { m_LengthValue->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Formula ) { m_Formula->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcQuantityLength::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcQuantityLength::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcQuantityLength, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcQuantityLength, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Unit, map );
	m_LengthValue = IfcLengthMeasure::createObjectFromStepData( args[3] );
	m_Formula = IfcLabel::createObjectFromStepData( args[4] );
}
void IfcQuantityLength::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPhysicalSimpleQuantity::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LengthValue", m_LengthValue ) );
	vec_attributes.push_back( std::make_pair( "Formula", m_Formula ) );
}
void IfcQuantityLength::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcQuantityLength::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPhysicalSimpleQuantity::setInverseCounterparts( ptr_self_entity );
}
void IfcQuantityLength::unlinkSelf()
{
	IfcPhysicalSimpleQuantity::unlinkSelf();
}
