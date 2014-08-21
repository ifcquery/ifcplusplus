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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcGroup.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectDefinition.h"
#include "include/IfcObjectTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssignsToGroup 
IfcRelAssignsToGroup::IfcRelAssignsToGroup() {}
IfcRelAssignsToGroup::IfcRelAssignsToGroup( int id ) { m_id = id; }
IfcRelAssignsToGroup::~IfcRelAssignsToGroup() {}
shared_ptr<IfcPPObject> IfcRelAssignsToGroup::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssignsToGroup> copy_self( new IfcRelAssignsToGroup() );
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
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcObjectDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatedObjectsType ) { copy_self->m_RelatedObjectsType = dynamic_pointer_cast<IfcObjectTypeEnum>( m_RelatedObjectsType->getDeepCopy(options) ); }
	if( m_RelatingGroup ) { copy_self->m_RelatingGroup = dynamic_pointer_cast<IfcGroup>( m_RelatingGroup->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelAssignsToGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSIGNSTOGROUP" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_RelatedObjects );
	stream << ",";
	if( m_RelatedObjectsType ) { m_RelatedObjectsType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingGroup ) { stream << "#" << m_RelatingGroup->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssignsToGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssignsToGroup::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssignsToGroup, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReferenceList( args[4], m_RelatedObjects, map );
	m_RelatedObjectsType = IfcObjectTypeEnum::createObjectFromSTEP( args[5] );
	readEntityReference( args[6], m_RelatingGroup, map );
}
void IfcRelAssignsToGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssigns::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingGroup", m_RelatingGroup ) );
}
void IfcRelAssignsToGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelAssigns::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelAssignsToGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssigns::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssignsToGroup> ptr_self = dynamic_pointer_cast<IfcRelAssignsToGroup>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssignsToGroup::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingGroup )
	{
		m_RelatingGroup->m_IsGroupedBy_inverse.push_back( ptr_self );
	}
}
void IfcRelAssignsToGroup::unlinkSelf()
{
	IfcRelAssigns::unlinkSelf();
	if( m_RelatingGroup )
	{
		std::vector<weak_ptr<IfcRelAssignsToGroup> >& IsGroupedBy_inverse = m_RelatingGroup->m_IsGroupedBy_inverse;
		for( auto it_IsGroupedBy_inverse = IsGroupedBy_inverse.begin(); it_IsGroupedBy_inverse != IsGroupedBy_inverse.end(); ++it_IsGroupedBy_inverse)
		{
			shared_ptr<IfcRelAssignsToGroup> self_candidate( *it_IsGroupedBy_inverse );
			if( self_candidate.get() == this )
			{
				IsGroupedBy_inverse.erase( it_IsGroupedBy_inverse );
				break;
			}
		}
	}
}
