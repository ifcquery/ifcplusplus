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
#include "include/IfcConnectionGeometry.h"
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcInternalOrExternalEnum.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPhysicalOrVirtualEnum.h"
#include "include/IfcRelSpaceBoundary1stLevel.h"
#include "include/IfcSpaceBoundarySelect.h"
#include "include/IfcText.h"

// ENTITY IfcRelSpaceBoundary1stLevel 
IfcRelSpaceBoundary1stLevel::IfcRelSpaceBoundary1stLevel() {}
IfcRelSpaceBoundary1stLevel::IfcRelSpaceBoundary1stLevel( int id ) { m_id = id; }
IfcRelSpaceBoundary1stLevel::~IfcRelSpaceBoundary1stLevel() {}
shared_ptr<IfcPPObject> IfcRelSpaceBoundary1stLevel::getDeepCopy()
{
	shared_ptr<IfcRelSpaceBoundary1stLevel> copy_self( new IfcRelSpaceBoundary1stLevel() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_RelatingSpace ) { copy_self->m_RelatingSpace = dynamic_pointer_cast<IfcSpaceBoundarySelect>( m_RelatingSpace->getDeepCopy() ); }
	if( m_RelatedBuildingElement ) { copy_self->m_RelatedBuildingElement = dynamic_pointer_cast<IfcElement>( m_RelatedBuildingElement->getDeepCopy() ); }
	if( m_ConnectionGeometry ) { copy_self->m_ConnectionGeometry = dynamic_pointer_cast<IfcConnectionGeometry>( m_ConnectionGeometry->getDeepCopy() ); }
	if( m_PhysicalOrVirtualBoundary ) { copy_self->m_PhysicalOrVirtualBoundary = dynamic_pointer_cast<IfcPhysicalOrVirtualEnum>( m_PhysicalOrVirtualBoundary->getDeepCopy() ); }
	if( m_InternalOrExternalBoundary ) { copy_self->m_InternalOrExternalBoundary = dynamic_pointer_cast<IfcInternalOrExternalEnum>( m_InternalOrExternalBoundary->getDeepCopy() ); }
	if( m_ParentBoundary ) { copy_self->m_ParentBoundary = dynamic_pointer_cast<IfcRelSpaceBoundary1stLevel>( m_ParentBoundary->getDeepCopy() ); }
	return copy_self;
}
void IfcRelSpaceBoundary1stLevel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELSPACEBOUNDARY1STLEVEL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingSpace ) { m_RelatingSpace->getStepParameter( stream, true ); } else { stream << "*" ; }
	stream << ",";
	if( m_RelatedBuildingElement ) { stream << "#" << m_RelatedBuildingElement->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_ConnectionGeometry ) { stream << "#" << m_ConnectionGeometry->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_PhysicalOrVirtualBoundary ) { m_PhysicalOrVirtualBoundary->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_InternalOrExternalBoundary ) { m_InternalOrExternalBoundary->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ParentBoundary ) { stream << "#" << m_ParentBoundary->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelSpaceBoundary1stLevel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelSpaceBoundary1stLevel::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<10 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelSpaceBoundary1stLevel, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>10 ){ std::cout << "Wrong parameter count for entity IfcRelSpaceBoundary1stLevel, expecting 10, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_RelatingSpace = IfcSpaceBoundarySelect::createObjectFromStepData( args[4], map );
	readEntityReference( args[5], m_RelatedBuildingElement, map );
	readEntityReference( args[6], m_ConnectionGeometry, map );
	m_PhysicalOrVirtualBoundary = IfcPhysicalOrVirtualEnum::createObjectFromStepData( args[7] );
	m_InternalOrExternalBoundary = IfcInternalOrExternalEnum::createObjectFromStepData( args[8] );
	readEntityReference( args[9], m_ParentBoundary, map );
}
void IfcRelSpaceBoundary1stLevel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelSpaceBoundary::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ParentBoundary", m_ParentBoundary ) );
}
void IfcRelSpaceBoundary1stLevel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelSpaceBoundary::getAttributesInverse( vec_attributes_inverse );
	if( m_InnerBoundaries_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> InnerBoundaries_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_InnerBoundaries_inverse.size(); ++i )
		{
			if( !m_InnerBoundaries_inverse[i].expired() )
			{
				InnerBoundaries_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSpaceBoundary1stLevel>( m_InnerBoundaries_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "InnerBoundaries_inverse", InnerBoundaries_inverse_vec_obj ) );
	}
}
void IfcRelSpaceBoundary1stLevel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelSpaceBoundary::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelSpaceBoundary1stLevel> ptr_self = dynamic_pointer_cast<IfcRelSpaceBoundary1stLevel>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelSpaceBoundary1stLevel::setInverseCounterparts: type mismatch" ); }
	if( m_ParentBoundary )
	{
		m_ParentBoundary->m_InnerBoundaries_inverse.push_back( ptr_self );
	}
}
void IfcRelSpaceBoundary1stLevel::unlinkSelf()
{
	IfcRelSpaceBoundary::unlinkSelf();
	if( m_ParentBoundary )
	{
		std::vector<weak_ptr<IfcRelSpaceBoundary1stLevel> >& InnerBoundaries_inverse = m_ParentBoundary->m_InnerBoundaries_inverse;
		std::vector<weak_ptr<IfcRelSpaceBoundary1stLevel> >::iterator it_InnerBoundaries_inverse;
		for( it_InnerBoundaries_inverse = InnerBoundaries_inverse.begin(); it_InnerBoundaries_inverse != InnerBoundaries_inverse.end(); ++it_InnerBoundaries_inverse)
		{
			shared_ptr<IfcRelSpaceBoundary1stLevel> self_candidate( *it_InnerBoundaries_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				InnerBoundaries_inverse.erase( it_InnerBoundaries_inverse );
				break;
			}
		}
	}
}
