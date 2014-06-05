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
#include "include/IfcLengthMeasure.h"
#include "include/IfcReinforcementBarProperties.h"
#include "include/IfcReinforcingBarRoleEnum.h"
#include "include/IfcSectionProperties.h"
#include "include/IfcSectionReinforcementProperties.h"

// ENTITY IfcSectionReinforcementProperties 
IfcSectionReinforcementProperties::IfcSectionReinforcementProperties() {}
IfcSectionReinforcementProperties::IfcSectionReinforcementProperties( int id ) { m_id = id; }
IfcSectionReinforcementProperties::~IfcSectionReinforcementProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSectionReinforcementProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSectionReinforcementProperties> other = dynamic_pointer_cast<IfcSectionReinforcementProperties>(other_entity);
	if( !other) { return; }
	m_LongitudinalStartPosition = other->m_LongitudinalStartPosition;
	m_LongitudinalEndPosition = other->m_LongitudinalEndPosition;
	m_TransversePosition = other->m_TransversePosition;
	m_ReinforcementRole = other->m_ReinforcementRole;
	m_SectionDefinition = other->m_SectionDefinition;
	m_CrossSectionReinforcementDefinitions = other->m_CrossSectionReinforcementDefinitions;
}
void IfcSectionReinforcementProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSECTIONREINFORCEMENTPROPERTIES" << "(";
	if( m_LongitudinalStartPosition ) { m_LongitudinalStartPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalEndPosition ) { m_LongitudinalEndPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransversePosition ) { m_TransversePosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ReinforcementRole ) { m_ReinforcementRole->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SectionDefinition ) { stream << "#" << m_SectionDefinition->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_CrossSectionReinforcementDefinitions );
	stream << ");";
}
void IfcSectionReinforcementProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSectionReinforcementProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSectionReinforcementProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcSectionReinforcementProperties, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_LongitudinalStartPosition = IfcLengthMeasure::createObjectFromStepData( args[0] );
	m_LongitudinalEndPosition = IfcLengthMeasure::createObjectFromStepData( args[1] );
	m_TransversePosition = IfcLengthMeasure::createObjectFromStepData( args[2] );
	m_ReinforcementRole = IfcReinforcingBarRoleEnum::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_SectionDefinition, map );
	readEntityReferenceList( args[5], m_CrossSectionReinforcementDefinitions, map );
}
void IfcSectionReinforcementProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedProperties::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LongitudinalStartPosition", m_LongitudinalStartPosition ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalEndPosition", m_LongitudinalEndPosition ) );
	vec_attributes.push_back( std::make_pair( "TransversePosition", m_TransversePosition ) );
	vec_attributes.push_back( std::make_pair( "ReinforcementRole", m_ReinforcementRole ) );
	vec_attributes.push_back( std::make_pair( "SectionDefinition", m_SectionDefinition ) );
}
void IfcSectionReinforcementProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSectionReinforcementProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedProperties::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionReinforcementProperties::unlinkSelf()
{
	IfcPreDefinedProperties::unlinkSelf();
}
