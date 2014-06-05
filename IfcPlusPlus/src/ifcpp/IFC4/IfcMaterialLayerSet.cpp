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
#include "include/IfcMaterialLayer.h"
#include "include/IfcMaterialLayerSet.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialLayerSet 
IfcMaterialLayerSet::IfcMaterialLayerSet() {}
IfcMaterialLayerSet::IfcMaterialLayerSet( int id ) { m_id = id; }
IfcMaterialLayerSet::~IfcMaterialLayerSet() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialLayerSet::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialLayerSet> other = dynamic_pointer_cast<IfcMaterialLayerSet>(other_entity);
	if( !other) { return; }
	m_MaterialLayers = other->m_MaterialLayers;
	m_LayerSetName = other->m_LayerSetName;
	m_Description = other->m_Description;
}
void IfcMaterialLayerSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALLAYERSET" << "(";
	writeEntityList( stream, m_MaterialLayers );
	stream << ",";
	if( m_LayerSetName ) { m_LayerSetName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialLayerSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialLayerSet::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialLayerSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcMaterialLayerSet, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_MaterialLayers, map );
	m_LayerSetName = IfcLabel::createObjectFromStepData( args[1] );
	m_Description = IfcText::createObjectFromStepData( args[2] );
}
void IfcMaterialLayerSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LayerSetName", m_LayerSetName ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
}
void IfcMaterialLayerSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcMaterialLayerSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialLayerSet> ptr_self = dynamic_pointer_cast<IfcMaterialLayerSet>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialLayerSet::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_MaterialLayers.size(); ++i )
	{
		if( m_MaterialLayers[i] )
		{
			m_MaterialLayers[i]->m_ToMaterialLayerSet_inverse = ptr_self;
		}
	}
}
void IfcMaterialLayerSet::unlinkSelf()
{
	IfcMaterialDefinition::unlinkSelf();
	for( int i=0; i<m_MaterialLayers.size(); ++i )
	{
		if( m_MaterialLayers[i] )
		{
			shared_ptr<IfcMaterialLayerSet> self_candidate( m_MaterialLayers[i]->m_ToMaterialLayerSet_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				weak_ptr<IfcMaterialLayerSet>& self_candidate_weak = m_MaterialLayers[i]->m_ToMaterialLayerSet_inverse;
				self_candidate_weak.reset();
			}
		}
	}
}
