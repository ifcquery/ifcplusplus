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
#include "include/IfcBoundaryFaceCondition.h"
#include "include/IfcLabel.h"
#include "include/IfcModulusOfSubgradeReactionSelect.h"

// ENTITY IfcBoundaryFaceCondition 
IfcBoundaryFaceCondition::IfcBoundaryFaceCondition() {}
IfcBoundaryFaceCondition::IfcBoundaryFaceCondition( int id ) { m_id = id; }
IfcBoundaryFaceCondition::~IfcBoundaryFaceCondition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBoundaryFaceCondition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBoundaryFaceCondition> other = dynamic_pointer_cast<IfcBoundaryFaceCondition>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_TranslationalStiffnessByAreaX = other->m_TranslationalStiffnessByAreaX;
	m_TranslationalStiffnessByAreaY = other->m_TranslationalStiffnessByAreaY;
	m_TranslationalStiffnessByAreaZ = other->m_TranslationalStiffnessByAreaZ;
}
void IfcBoundaryFaceCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOUNDARYFACECONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByAreaX ) { m_TranslationalStiffnessByAreaX->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByAreaY ) { m_TranslationalStiffnessByAreaY->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessByAreaZ ) { m_TranslationalStiffnessByAreaZ->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcBoundaryFaceCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryFaceCondition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryFaceCondition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcBoundaryFaceCondition, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_TranslationalStiffnessByAreaX = IfcModulusOfSubgradeReactionSelect::createObjectFromStepData( args[1], map );
	m_TranslationalStiffnessByAreaY = IfcModulusOfSubgradeReactionSelect::createObjectFromStepData( args[2], map );
	m_TranslationalStiffnessByAreaZ = IfcModulusOfSubgradeReactionSelect::createObjectFromStepData( args[3], map );
}
void IfcBoundaryFaceCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByAreaX", m_TranslationalStiffnessByAreaX ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByAreaY", m_TranslationalStiffnessByAreaY ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessByAreaZ", m_TranslationalStiffnessByAreaZ ) );
}
void IfcBoundaryFaceCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBoundaryFaceCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryFaceCondition::unlinkSelf()
{
	IfcBoundaryCondition::unlinkSelf();
}
