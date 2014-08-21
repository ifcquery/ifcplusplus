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
#include "ifcpp/model/IfcPPGuid.h"
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
shared_ptr<IfcPPObject> IfcMaterialConstituentSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialConstituentSet> copy_self( new IfcMaterialConstituentSet() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_MaterialConstituents.size(); ++ii )
	{
		auto item_ii = m_MaterialConstituents[ii];
		if( item_ii )
		{
			copy_self->m_MaterialConstituents.push_back( dynamic_pointer_cast<IfcMaterialConstituent>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcMaterialConstituentSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALCONSTITUENTSET" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_MaterialConstituents );
	stream << ");";
}
void IfcMaterialConstituentSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialConstituentSet::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcMaterialConstituentSet, expecting 3, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReferenceList( args[2], m_MaterialConstituents, map );
}
void IfcMaterialConstituentSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	if( m_MaterialConstituents.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MaterialConstituents_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_MaterialConstituents.begin(), m_MaterialConstituents.end(), std::back_inserter( MaterialConstituents_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "MaterialConstituents", MaterialConstituents_vec_object ) );
	}
}
void IfcMaterialConstituentSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialConstituentSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialConstituentSet> ptr_self = dynamic_pointer_cast<IfcMaterialConstituentSet>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialConstituentSet::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_MaterialConstituents.size(); ++i )
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
	for( size_t i=0; i<m_MaterialConstituents.size(); ++i )
	{
		if( m_MaterialConstituents[i] )
		{
			shared_ptr<IfcMaterialConstituentSet> self_candidate( m_MaterialConstituents[i]->m_ToMaterialConstituentSet_inverse );
			if( self_candidate.get() == this )
			{
				weak_ptr<IfcMaterialConstituentSet>& self_candidate_weak = m_MaterialConstituents[i]->m_ToMaterialConstituentSet_inverse;
				self_candidate_weak.reset();
			}
		}
	}
}
