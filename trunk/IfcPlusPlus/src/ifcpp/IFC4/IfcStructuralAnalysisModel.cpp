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
#include "include/IfcAnalysisModelTypeEnum.h"
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToGroup.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcStructuralAnalysisModel.h"
#include "include/IfcStructuralLoadGroup.h"
#include "include/IfcStructuralResultGroup.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralAnalysisModel 
IfcStructuralAnalysisModel::IfcStructuralAnalysisModel() {}
IfcStructuralAnalysisModel::IfcStructuralAnalysisModel( int id ) { m_id = id; }
IfcStructuralAnalysisModel::~IfcStructuralAnalysisModel() {}
shared_ptr<IfcPPObject> IfcStructuralAnalysisModel::getDeepCopy()
{
	shared_ptr<IfcStructuralAnalysisModel> copy_self( new IfcStructuralAnalysisModel() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy() ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcAnalysisModelTypeEnum>( m_PredefinedType->getDeepCopy() ); }
	if( m_OrientationOf2DPlane ) { copy_self->m_OrientationOf2DPlane = dynamic_pointer_cast<IfcAxis2Placement3D>( m_OrientationOf2DPlane->getDeepCopy() ); }
	for( size_t ii=0; ii<m_LoadedBy.size(); ++ii )
	{
		auto item_ii = m_LoadedBy[ii];
		if( item_ii )
		{
			copy_self->m_LoadedBy.push_back( dynamic_pointer_cast<IfcStructuralLoadGroup>(item_ii->getDeepCopy() ) );
		}
	}
	for( size_t ii=0; ii<m_HasResults.size(); ++ii )
	{
		auto item_ii = m_HasResults[ii];
		if( item_ii )
		{
			copy_self->m_HasResults.push_back( dynamic_pointer_cast<IfcStructuralResultGroup>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_SharedPlacement ) { copy_self->m_SharedPlacement = dynamic_pointer_cast<IfcObjectPlacement>( m_SharedPlacement->getDeepCopy() ); }
	return copy_self;
}
void IfcStructuralAnalysisModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALANALYSISMODEL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OrientationOf2DPlane ) { stream << "#" << m_OrientationOf2DPlane->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_LoadedBy );
	stream << ",";
	writeEntityList( stream, m_HasResults );
	stream << ",";
	if( m_SharedPlacement ) { stream << "#" << m_SharedPlacement->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralAnalysisModel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralAnalysisModel::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralAnalysisModel, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcStructuralAnalysisModel, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	m_PredefinedType = IfcAnalysisModelTypeEnum::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_OrientationOf2DPlane, map );
	readEntityReferenceList( args[7], m_LoadedBy, map );
	readEntityReferenceList( args[8], m_HasResults, map );
	readEntityReference( args[9], m_SharedPlacement, map );
}
void IfcStructuralAnalysisModel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSystem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "OrientationOf2DPlane", m_OrientationOf2DPlane ) );
	if( m_LoadedBy.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> LoadedBy_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_LoadedBy.begin(), m_LoadedBy.end(), std::back_inserter( LoadedBy_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "LoadedBy", LoadedBy_vec_object ) );
	}
	if( m_HasResults.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasResults_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_HasResults.begin(), m_HasResults.end(), std::back_inserter( HasResults_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "HasResults", HasResults_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "SharedPlacement", m_SharedPlacement ) );
}
void IfcStructuralAnalysisModel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSystem::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralAnalysisModel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSystem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcStructuralAnalysisModel> ptr_self = dynamic_pointer_cast<IfcStructuralAnalysisModel>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcStructuralAnalysisModel::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_HasResults.size(); ++i )
	{
		if( m_HasResults[i] )
		{
			m_HasResults[i]->m_ResultGroupFor_inverse.push_back( ptr_self );
		}
	}
	for( int i=0; i<m_LoadedBy.size(); ++i )
	{
		if( m_LoadedBy[i] )
		{
			m_LoadedBy[i]->m_LoadGroupFor_inverse.push_back( ptr_self );
		}
	}
}
void IfcStructuralAnalysisModel::unlinkSelf()
{
	IfcSystem::unlinkSelf();
	for( int i=0; i<m_HasResults.size(); ++i )
	{
		if( m_HasResults[i] )
		{
			std::vector<weak_ptr<IfcStructuralAnalysisModel> >& ResultGroupFor_inverse = m_HasResults[i]->m_ResultGroupFor_inverse;
			std::vector<weak_ptr<IfcStructuralAnalysisModel> >::iterator it_ResultGroupFor_inverse;
			for( it_ResultGroupFor_inverse = ResultGroupFor_inverse.begin(); it_ResultGroupFor_inverse != ResultGroupFor_inverse.end(); ++it_ResultGroupFor_inverse)
			{
				shared_ptr<IfcStructuralAnalysisModel> self_candidate( *it_ResultGroupFor_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					ResultGroupFor_inverse.erase( it_ResultGroupFor_inverse );
					break;
				}
			}
		}
	}
	for( int i=0; i<m_LoadedBy.size(); ++i )
	{
		if( m_LoadedBy[i] )
		{
			std::vector<weak_ptr<IfcStructuralAnalysisModel> >& LoadGroupFor_inverse = m_LoadedBy[i]->m_LoadGroupFor_inverse;
			std::vector<weak_ptr<IfcStructuralAnalysisModel> >::iterator it_LoadGroupFor_inverse;
			for( it_LoadGroupFor_inverse = LoadGroupFor_inverse.begin(); it_LoadGroupFor_inverse != LoadGroupFor_inverse.end(); ++it_LoadGroupFor_inverse)
			{
				shared_ptr<IfcStructuralAnalysisModel> self_candidate( *it_LoadGroupFor_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					LoadGroupFor_inverse.erase( it_LoadGroupFor_inverse );
					break;
				}
			}
		}
	}
}
