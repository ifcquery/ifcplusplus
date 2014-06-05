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
#include "include/IfcBoundaryNodeCondition.h"
#include "include/IfcLabel.h"
#include "include/IfcRotationalStiffnessSelect.h"
#include "include/IfcTranslationalStiffnessSelect.h"

// ENTITY IfcBoundaryNodeCondition 
IfcBoundaryNodeCondition::IfcBoundaryNodeCondition() {}
IfcBoundaryNodeCondition::IfcBoundaryNodeCondition( int id ) { m_id = id; }
IfcBoundaryNodeCondition::~IfcBoundaryNodeCondition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBoundaryNodeCondition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBoundaryNodeCondition> other = dynamic_pointer_cast<IfcBoundaryNodeCondition>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_TranslationalStiffnessX = other->m_TranslationalStiffnessX;
	m_TranslationalStiffnessY = other->m_TranslationalStiffnessY;
	m_TranslationalStiffnessZ = other->m_TranslationalStiffnessZ;
	m_RotationalStiffnessX = other->m_RotationalStiffnessX;
	m_RotationalStiffnessY = other->m_RotationalStiffnessY;
	m_RotationalStiffnessZ = other->m_RotationalStiffnessZ;
}
void IfcBoundaryNodeCondition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOUNDARYNODECONDITION" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessX ) { m_TranslationalStiffnessX->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessY ) { m_TranslationalStiffnessY->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TranslationalStiffnessZ ) { m_TranslationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessX ) { m_RotationalStiffnessX->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessY ) { m_RotationalStiffnessY->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RotationalStiffnessZ ) { m_RotationalStiffnessZ->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcBoundaryNodeCondition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundaryNodeCondition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundaryNodeCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcBoundaryNodeCondition, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_TranslationalStiffnessX = IfcTranslationalStiffnessSelect::createObjectFromStepData( args[1], map );
	m_TranslationalStiffnessY = IfcTranslationalStiffnessSelect::createObjectFromStepData( args[2], map );
	m_TranslationalStiffnessZ = IfcTranslationalStiffnessSelect::createObjectFromStepData( args[3], map );
	m_RotationalStiffnessX = IfcRotationalStiffnessSelect::createObjectFromStepData( args[4], map );
	m_RotationalStiffnessY = IfcRotationalStiffnessSelect::createObjectFromStepData( args[5], map );
	m_RotationalStiffnessZ = IfcRotationalStiffnessSelect::createObjectFromStepData( args[6], map );
}
void IfcBoundaryNodeCondition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundaryCondition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessX", m_TranslationalStiffnessX ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessY", m_TranslationalStiffnessY ) );
	vec_attributes.push_back( std::make_pair( "TranslationalStiffnessZ", m_TranslationalStiffnessZ ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessX", m_RotationalStiffnessX ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessY", m_RotationalStiffnessY ) );
	vec_attributes.push_back( std::make_pair( "RotationalStiffnessZ", m_RotationalStiffnessZ ) );
}
void IfcBoundaryNodeCondition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBoundaryNodeCondition::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundaryCondition::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundaryNodeCondition::unlinkSelf()
{
	IfcBoundaryCondition::unlinkSelf();
}
