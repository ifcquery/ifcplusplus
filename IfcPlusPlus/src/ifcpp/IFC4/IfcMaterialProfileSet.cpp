/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcCompositeProfileDef.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcMaterialProfile.h"
#include "include/IfcMaterialProfileSet.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcMaterialProfileSet 
IfcMaterialProfileSet::IfcMaterialProfileSet() { m_entity_enum = IFCMATERIALPROFILESET; }
IfcMaterialProfileSet::IfcMaterialProfileSet( int id ) { m_id = id; m_entity_enum = IFCMATERIALPROFILESET; }
IfcMaterialProfileSet::~IfcMaterialProfileSet() {}
shared_ptr<IfcPPObject> IfcMaterialProfileSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialProfileSet> copy_self( new IfcMaterialProfileSet() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_MaterialProfiles.size(); ++ii )
	{
		auto item_ii = m_MaterialProfiles[ii];
		if( item_ii )
		{
			copy_self->m_MaterialProfiles.push_back( dynamic_pointer_cast<IfcMaterialProfile>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_CompositeProfile ) { copy_self->m_CompositeProfile = dynamic_pointer_cast<IfcCompositeProfileDef>( m_CompositeProfile->getDeepCopy(options) ); }
	return copy_self;
}
void IfcMaterialProfileSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALPROFILESET" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_MaterialProfiles );
	stream << ",";
	if( m_CompositeProfile ) { stream << "#" << m_CompositeProfile->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcMaterialProfileSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialProfileSet::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcMaterialProfileSet, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Description = IfcText::createObjectFromSTEP( args[1] );
	readEntityReferenceList( args[2], m_MaterialProfiles, map );
	readEntityReference( args[3], m_CompositeProfile, map );
}
void IfcMaterialProfileSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcMaterialDefinition::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Name", m_Name ) );
	vec_attributes.push_back( std::make_pair( "Description", m_Description ) );
	if( m_MaterialProfiles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> MaterialProfiles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_MaterialProfiles.begin(), m_MaterialProfiles.end(), std::back_inserter( MaterialProfiles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "MaterialProfiles", MaterialProfiles_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "CompositeProfile", m_CompositeProfile ) );
}
void IfcMaterialProfileSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcMaterialDefinition::getAttributesInverse( vec_attributes_inverse );
}
void IfcMaterialProfileSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcMaterialDefinition::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcMaterialProfileSet> ptr_self = dynamic_pointer_cast<IfcMaterialProfileSet>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcMaterialProfileSet::setInverseCounterparts: type mismatch" ); }
	for( size_t i=0; i<m_MaterialProfiles.size(); ++i )
	{
		if( m_MaterialProfiles[i] )
		{
			m_MaterialProfiles[i]->m_ToMaterialProfileSet_inverse = ptr_self;
		}
	}
}
void IfcMaterialProfileSet::unlinkFromInverseCounterparts()
{
	IfcMaterialDefinition::unlinkFromInverseCounterparts();
	for( size_t i=0; i<m_MaterialProfiles.size(); ++i )
	{
		if( m_MaterialProfiles[i] )
		{
			shared_ptr<IfcMaterialProfileSet> self_candidate( m_MaterialProfiles[i]->m_ToMaterialProfileSet_inverse );
			if( self_candidate.get() == this )
			{
				weak_ptr<IfcMaterialProfileSet>& self_candidate_weak = m_MaterialProfiles[i]->m_ToMaterialProfileSet_inverse;
				self_candidate_weak.reset();
			}
		}
	}
}
