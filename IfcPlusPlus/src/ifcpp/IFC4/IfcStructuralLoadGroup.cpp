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
#include "include/IfcActionSourceTypeEnum.h"
#include "include/IfcActionTypeEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLoadGroupTypeEnum.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRatioMeasure.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcStructuralAnalysisModel.h"
#include "include/IfcStructuralLoadGroup.h"
#include "include/IfcStructuralResultGroup.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralLoadGroup 
IfcStructuralLoadGroup::IfcStructuralLoadGroup() {}
IfcStructuralLoadGroup::IfcStructuralLoadGroup( int id ) { m_id = id; }
IfcStructuralLoadGroup::~IfcStructuralLoadGroup() {}
shared_ptr<IfcPPObject> IfcStructuralLoadGroup::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadGroup> copy_self( new IfcStructuralLoadGroup() );
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
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcLoadGroupTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_ActionType ) { copy_self->m_ActionType = dynamic_pointer_cast<IfcActionTypeEnum>( m_ActionType->getDeepCopy(options) ); }
	if( m_ActionSource ) { copy_self->m_ActionSource = dynamic_pointer_cast<IfcActionSourceTypeEnum>( m_ActionSource->getDeepCopy(options) ); }
	if( m_Coefficient ) { copy_self->m_Coefficient = dynamic_pointer_cast<IfcRatioMeasure>( m_Coefficient->getDeepCopy(options) ); }
	if( m_Purpose ) { copy_self->m_Purpose = dynamic_pointer_cast<IfcLabel>( m_Purpose->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadGroup::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADGROUP" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActionType ) { m_ActionType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ActionSource ) { m_ActionSource->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Coefficient ) { m_Coefficient->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Purpose ) { m_Purpose->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralLoadGroup::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadGroup::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralLoadGroup, expecting 10, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	m_PredefinedType = IfcLoadGroupTypeEnum::createObjectFromSTEP( args[5] );
	m_ActionType = IfcActionTypeEnum::createObjectFromSTEP( args[6] );
	m_ActionSource = IfcActionSourceTypeEnum::createObjectFromSTEP( args[7] );
	m_Coefficient = IfcRatioMeasure::createObjectFromSTEP( args[8] );
	m_Purpose = IfcLabel::createObjectFromSTEP( args[9] );
}
void IfcStructuralLoadGroup::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGroup::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "ActionType", m_ActionType ) );
	vec_attributes.push_back( std::make_pair( "ActionSource", m_ActionSource ) );
	vec_attributes.push_back( std::make_pair( "Coefficient", m_Coefficient ) );
	vec_attributes.push_back( std::make_pair( "Purpose", m_Purpose ) );
}
void IfcStructuralLoadGroup::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGroup::getAttributesInverse( vec_attributes_inverse );
	if( m_SourceOfResultGroup_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> SourceOfResultGroup_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_SourceOfResultGroup_inverse.size(); ++i )
		{
			if( !m_SourceOfResultGroup_inverse[i].expired() )
			{
				SourceOfResultGroup_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralResultGroup>( m_SourceOfResultGroup_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "SourceOfResultGroup_inverse", SourceOfResultGroup_inverse_vec_obj ) );
	}
	if( m_LoadGroupFor_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LoadGroupFor_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_LoadGroupFor_inverse.size(); ++i )
		{
			if( !m_LoadGroupFor_inverse[i].expired() )
			{
				LoadGroupFor_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcStructuralAnalysisModel>( m_LoadGroupFor_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "LoadGroupFor_inverse", LoadGroupFor_inverse_vec_obj ) );
	}
}
void IfcStructuralLoadGroup::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGroup::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadGroup::unlinkSelf()
{
	IfcGroup::unlinkSelf();
}
