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
#include "include/IfcBoundaryEdgeCondition.h"
#include "include/IfcLabel.h"
#include "include/IfcModulusOfRotationalSubgradeReactionSelect.h"
#include "include/IfcModulusOfTranslationalSubgradeReactionSelect.h"

// ENTITY IfcBoundaryEdgeCondition 
IfcBoundaryEdgeCondition::IfcBoundaryEdgeCondition() {}
IfcBoundaryEdgeCondition::IfcBoundaryEdgeCondition( int id ) { m_id = id; }
IfcBoundaryEdgeCondition::~IfcBoundaryEdgeCondition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBoundaryEdgeCondition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBoundaryEdgeCondition> other = dynamic_pointer_cast<IfcBoundaryEdgeCondition>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_TranslationalStiffnessByLengthX = other->m_TranslationalStiffnessByLengthX;
	m_TranslationalStiffnessByLengthY = other->m_TranslationalStiffnessByLengthY;
	m_TranslationalStiffnessByLengthZ = other->m_TranslationalStiffnessByLengthZ;
	m_RotationalStiffnessByLengthX = other->m_RotationalStiffnessByLengthX;
	m_RotationalStiffnessByLengthY = other->m_RotationalStiffnessByLengthY;
	m_RotationalStiffnessByLengthZ = other->m_RotationalStiffnessByLengthZ;
}
void IfcBoundaryEdgeCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOUNDARYEDGECONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthX ) { m_TranslationalStiffnessByLengthX->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthY ) { m_TranslationalStiffnessByLengthY->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByLengthZ ) { m_TranslationalStiffnessByLengthZ->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessByLengthX ) { m_RotationalStiffnessByLengthX->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessByLengthY ) { m_RotationalStiffnessByLengthY->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessByLengthZ ) { m_RotationalStiffnessByLengthZ->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcBoundaryEdgeCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryEdgeCondition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryEdgeCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcBoundaryEdgeCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_TranslationalStiffnessByLengthX = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromStepData( args[1], map );
	m_TranslationalStiffnessByLengthY = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromStepData( args[2], map );
	m_TranslationalStiffnessByLengthZ = IfcModulusOfTranslationalSubgradeReactionSelect::createObjectFromStepData( args[3], map );
	m_RotationalStiffnessByLengthX = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromStepData( args[4], map );
	m_RotationalStiffnessByLengthY = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromStepData( args[5], map );
	m_RotationalStiffnessByLengthZ = IfcModulusOfRotationalSubgradeReactionSelect::createObjectFromStepData( args[6], map );
}
void IfcBoundaryEdgeCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthX", m_TranslationalStiffnessByLengthX ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthY", m_TranslationalStiffnessByLengthY ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByLengthZ", m_TranslationalStiffnessByLengthZ ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthX", m_RotationalStiffnessByLengthX ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthY", m_RotationalStiffnessByLengthY ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessByLengthZ", m_RotationalStiffnessByLengthZ ) );
}
void IfcBoundaryEdgeCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBoundaryEdgeCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryEdgeCondition::unlinkSelf()
{
	IfcBoundaryCondition::unlinkSelf();
}
