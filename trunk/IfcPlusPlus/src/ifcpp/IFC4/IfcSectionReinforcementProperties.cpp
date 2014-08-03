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
shared_ptr<IfcPPObject> IfcSectionReinforcementProperties::getDeepCopy()
{
	shared_ptr<IfcSectionReinforcementProperties> copy_self( new IfcSectionReinforcementProperties() );
	if( m_LongitudinalStartPosition ) { copy_self->m_LongitudinalStartPosition = dynamic_pointer_cast<IfcLengthMeasure>( m_LongitudinalStartPosition->getDeepCopy() ); }
	if( m_LongitudinalEndPosition ) { copy_self->m_LongitudinalEndPosition = dynamic_pointer_cast<IfcLengthMeasure>( m_LongitudinalEndPosition->getDeepCopy() ); }
	if( m_TransversePosition ) { copy_self->m_TransversePosition = dynamic_pointer_cast<IfcLengthMeasure>( m_TransversePosition->getDeepCopy() ); }
	if( m_ReinforcementRole ) { copy_self->m_ReinforcementRole = dynamic_pointer_cast<IfcReinforcingBarRoleEnum>( m_ReinforcementRole->getDeepCopy() ); }
	if( m_SectionDefinition ) { copy_self->m_SectionDefinition = dynamic_pointer_cast<IfcSectionProperties>( m_SectionDefinition->getDeepCopy() ); }
	for( size_t ii=0; ii<m_CrossSectionReinforcementDefinitions.size(); ++ii )
	{
		auto item_ii = m_CrossSectionReinforcementDefinitions[ii];
		if( item_ii )
		{
			copy_self->m_CrossSectionReinforcementDefinitions.push_back( dynamic_pointer_cast<IfcReinforcementBarProperties>(item_ii->getDeepCopy() ) );
		}
	}
	return copy_self;
}
void IfcSectionReinforcementProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSECTIONREINFORCEMENTPROPERTIES" << "(";
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
void IfcSectionReinforcementProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( m_CrossSectionReinforcementDefinitions.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> CrossSectionReinforcementDefinitions_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_CrossSectionReinforcementDefinitions.begin(), m_CrossSectionReinforcementDefinitions.end(), std::back_inserter( CrossSectionReinforcementDefinitions_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "CrossSectionReinforcementDefinitions", CrossSectionReinforcementDefinitions_vec_object ) );
	}
}
void IfcSectionReinforcementProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedProperties::getAttributesInverse( vec_attributes_inverse );
}
void IfcSectionReinforcementProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedProperties::setInverseCounterparts( ptr_self_entity );
}
void IfcSectionReinforcementProperties::unlinkSelf()
{
	IfcPreDefinedProperties::unlinkSelf();
}
