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
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcMaterialDefinition.h"
#include "include/IfcMaterialSelect.h"
#include "include/IfcMaterialUsageDefinition.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesMaterial.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesMaterial 
IfcRelAssociatesMaterial::IfcRelAssociatesMaterial() {}
IfcRelAssociatesMaterial::IfcRelAssociatesMaterial( int id ) { m_id = id; }
IfcRelAssociatesMaterial::~IfcRelAssociatesMaterial() {}
shared_ptr<IfcPPObject> IfcRelAssociatesMaterial::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssociatesMaterial> copy_self( new IfcRelAssociatesMaterial() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcDefinitionSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingMaterial ) { copy_self->m_RelatingMaterial = dynamic_pointer_cast<IfcMaterialSelect>( m_RelatingMaterial->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelAssociatesMaterial::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSOCIATESMATERIAL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_RelatingMaterial ) { m_RelatingMaterial->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcRelAssociatesMaterial::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesMaterial::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesMaterial, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	m_RelatingMaterial = IfcMaterialSelect::createObjectFromSTEP( args[5], map );
}
void IfcRelAssociatesMaterial::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingMaterial", m_RelatingMaterial ) );
}
void IfcRelAssociatesMaterial::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelAssociates::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelAssociatesMaterial::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssociates::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociatesMaterial> ptr_self = dynamic_pointer_cast<IfcRelAssociatesMaterial>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociatesMaterial::setInverseCounterparts: type mismatch" ); }
	shared_ptr<IfcMaterialDefinition>  RelatingMaterial_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatingMaterial );
	if( RelatingMaterial_IfcMaterialDefinition )
	{
		RelatingMaterial_IfcMaterialDefinition->m_AssociatedTo_inverse.push_back( ptr_self );
	}
	shared_ptr<IfcMaterialUsageDefinition>  RelatingMaterial_IfcMaterialUsageDefinition = dynamic_pointer_cast<IfcMaterialUsageDefinition>( m_RelatingMaterial );
	if( RelatingMaterial_IfcMaterialUsageDefinition )
	{
		RelatingMaterial_IfcMaterialUsageDefinition->m_AssociatedTo_inverse.push_back( ptr_self );
	}
}
void IfcRelAssociatesMaterial::unlinkSelf()
{
	IfcRelAssociates::unlinkSelf();
	shared_ptr<IfcMaterialDefinition>  RelatingMaterial_IfcMaterialDefinition = dynamic_pointer_cast<IfcMaterialDefinition>( m_RelatingMaterial );
	if( RelatingMaterial_IfcMaterialDefinition )
	{
		std::vector<weak_ptr<IfcRelAssociatesMaterial> >& AssociatedTo_inverse = RelatingMaterial_IfcMaterialDefinition->m_AssociatedTo_inverse;
		for( auto it_AssociatedTo_inverse = AssociatedTo_inverse.begin(); it_AssociatedTo_inverse != AssociatedTo_inverse.end(); ++it_AssociatedTo_inverse)
		{
			shared_ptr<IfcRelAssociatesMaterial> self_candidate( *it_AssociatedTo_inverse );
			if( self_candidate.get() == this )
			{
				AssociatedTo_inverse.erase( it_AssociatedTo_inverse );
				break;
			}
		}
	}
	shared_ptr<IfcMaterialUsageDefinition>  RelatingMaterial_IfcMaterialUsageDefinition = dynamic_pointer_cast<IfcMaterialUsageDefinition>( m_RelatingMaterial );
	if( RelatingMaterial_IfcMaterialUsageDefinition )
	{
		std::vector<weak_ptr<IfcRelAssociatesMaterial> >& AssociatedTo_inverse = RelatingMaterial_IfcMaterialUsageDefinition->m_AssociatedTo_inverse;
		for( auto it_AssociatedTo_inverse = AssociatedTo_inverse.begin(); it_AssociatedTo_inverse != AssociatedTo_inverse.end(); ++it_AssociatedTo_inverse)
		{
			shared_ptr<IfcRelAssociatesMaterial> self_candidate( *it_AssociatedTo_inverse );
			if( self_candidate.get() == this )
			{
				AssociatedTo_inverse.erase( it_AssociatedTo_inverse );
				break;
			}
		}
	}
}
