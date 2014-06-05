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
#include "include/IfcMaterialConstituent.h"
#include "include/IfcMaterialConstituentSet.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialConstituentSet 
IfcMaterialConstituentSet::IfcMaterialConstituentSet() {}
IfcMaterialConstituentSet::IfcMaterialConstituentSet( int id ) { m_id = id; }
IfcMaterialConstituentSet::~IfcMaterialConstituentSet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialConstituentSet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialConstituentSet> other = dynamic_pointer_cast<IfcMaterialConstituentSet>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_MaterialConstituents = other->m_MaterialConstituents;
}
void IfcMaterialConstituentSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALCONSTITUENTSET" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_MaterialConstituents );
	stream << ");";
}
void IfcMaterialConstituentSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialConstituentSet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialConstituentSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcMaterialConstituentSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	readEntityReferenceList( args[2], m_MaterialConstituents, map );
}
void IfcMaterialConstituentSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcMaterialConstituentSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialConstituentSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialConstituentSet> ptr_self = dynamic_pointer_cast<IfcMaterialConstituentSet>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialConstituentSet::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_MaterialConstituents.size(); ++i )
	{
		if( m_MaterialConstituents[i] )
		{
			m_MaterialConstituents[i]->m_ToMaterialConstituentSet_inverse = ptr_self;
		}
	}
}
void IfcMaterialConstituentSet::unlinkSelf()
{
	IfcMaterialDefinition::unlinkSelf();
	for( int i=0; i<m_MaterialConstituents.size(); ++i )
	{
		if( m_MaterialConstituents[i] )
		{
			shared_ptr<IfcMaterialConstituentSet> self_candidate( m_MaterialConstituents[i]->m_ToMaterialConstituentSet_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				weak_ptr<IfcMaterialConstituentSet>& self_candidate_weak = m_MaterialConstituents[i]->m_ToMaterialConstituentSet_inverse;
				self_candidate_weak.reset();
			}
		}
	}
}
