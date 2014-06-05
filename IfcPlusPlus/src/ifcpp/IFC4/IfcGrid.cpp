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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcGrid.h"
#include "include/IfcGridAxis.h"
#include "include/IfcGridTypeEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcText.h"

// ENTITY IfcGrid 
IfcGrid::IfcGrid() {}
IfcGrid::IfcGrid( int id ) { m_id = id; }
IfcGrid::~IfcGrid() {}

// method setEntity takes over all attributes from another instance of the class
void IfcGrid::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcGrid> other = dynamic_pointer_cast<IfcGrid>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_UAxes = other->m_UAxes;
	m_VAxes = other->m_VAxes;
	m_WAxes = other->m_WAxes;
	m_PredefinedType = other->m_PredefinedType;
}
void IfcGrid::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCGRID" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_UAxes );
	stream << ",";
	writeEntityList( stream, m_VAxes );
	stream << ",";
	writeEntityList( stream, m_WAxes );
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcGrid::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGrid::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGrid, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcGrid, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	readEntityReferenceList( args[7], m_UAxes, map );
	readEntityReferenceList( args[8], m_VAxes, map );
	readEntityReferenceList( args[9], m_WAxes, map );
	m_PredefinedType = IfcGridTypeEnum::createObjectFromStepData( args[10] );
}
void IfcGrid::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcProduct::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcGrid::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> ContainedInStructure_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_ContainedInStructure_inverse.size(); ++i ) { ContainedInStructure_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelContainedInSpatialStructure>( m_ContainedInStructure_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "ContainedInStructure_inverse", ContainedInStructure_inverse_vec_obj ) );
}
void IfcGrid::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcProduct::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcGrid> ptr_self = dynamic_pointer_cast<IfcGrid>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcGrid::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_UAxes.size(); ++i )
	{
		if( m_UAxes[i] )
		{
			m_UAxes[i]->m_PartOfU_inverse.push_back( ptr_self );
		}
	}
	for( int i=0; i<m_VAxes.size(); ++i )
	{
		if( m_VAxes[i] )
		{
			m_VAxes[i]->m_PartOfV_inverse.push_back( ptr_self );
		}
	}
	for( int i=0; i<m_WAxes.size(); ++i )
	{
		if( m_WAxes[i] )
		{
			m_WAxes[i]->m_PartOfW_inverse.push_back( ptr_self );
		}
	}
}
void IfcGrid::unlinkSelf()
{
	IfcProduct::unlinkSelf();
	for( int i=0; i<m_UAxes.size(); ++i )
	{
		if( m_UAxes[i] )
		{
			std::vector<weak_ptr<IfcGrid> >& PartOfU_inverse = m_UAxes[i]->m_PartOfU_inverse;
			std::vector<weak_ptr<IfcGrid> >::iterator it_PartOfU_inverse;
			for( it_PartOfU_inverse = PartOfU_inverse.begin(); it_PartOfU_inverse != PartOfU_inverse.end(); ++it_PartOfU_inverse)
			{
				shared_ptr<IfcGrid> self_candidate( *it_PartOfU_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfU_inverse.erase( it_PartOfU_inverse );
					break;
				}
			}
		}
	}
	for( int i=0; i<m_VAxes.size(); ++i )
	{
		if( m_VAxes[i] )
		{
			std::vector<weak_ptr<IfcGrid> >& PartOfV_inverse = m_VAxes[i]->m_PartOfV_inverse;
			std::vector<weak_ptr<IfcGrid> >::iterator it_PartOfV_inverse;
			for( it_PartOfV_inverse = PartOfV_inverse.begin(); it_PartOfV_inverse != PartOfV_inverse.end(); ++it_PartOfV_inverse)
			{
				shared_ptr<IfcGrid> self_candidate( *it_PartOfV_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfV_inverse.erase( it_PartOfV_inverse );
					break;
				}
			}
		}
	}
	for( int i=0; i<m_WAxes.size(); ++i )
	{
		if( m_WAxes[i] )
		{
			std::vector<weak_ptr<IfcGrid> >& PartOfW_inverse = m_WAxes[i]->m_PartOfW_inverse;
			std::vector<weak_ptr<IfcGrid> >::iterator it_PartOfW_inverse;
			for( it_PartOfW_inverse = PartOfW_inverse.begin(); it_PartOfW_inverse != PartOfW_inverse.end(); ++it_PartOfW_inverse)
			{
				shared_ptr<IfcGrid> self_candidate( *it_PartOfW_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					PartOfW_inverse.erase( it_PartOfW_inverse );
					break;
				}
			}
		}
	}
}
