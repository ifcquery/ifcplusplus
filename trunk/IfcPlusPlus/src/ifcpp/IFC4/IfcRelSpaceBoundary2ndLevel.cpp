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
#include "include/IfcRelSpaceBoundary2ndLevel.h"
#include "include/IfcSpaceBoundarySelect.h"
#include "include/IfcText.h"

// ENTITY IfcRelSpaceBoundary2ndLevel 
IfcRelSpaceBoundary2ndLevel::IfcRelSpaceBoundary2ndLevel() {}
IfcRelSpaceBoundary2ndLevel::IfcRelSpaceBoundary2ndLevel( int id ) { m_id = id; }
IfcRelSpaceBoundary2ndLevel::~IfcRelSpaceBoundary2ndLevel() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelSpaceBoundary2ndLevel::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelSpaceBoundary2ndLevel> other = dynamic_pointer_cast<IfcRelSpaceBoundary2ndLevel>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingSpace = other->m_RelatingSpace;
	m_RelatedBuildingElement = other->m_RelatedBuildingElement;
	m_ConnectionGeometry = other->m_ConnectionGeometry;
	m_PhysicalOrVirtualBoundary = other->m_PhysicalOrVirtualBoundary;
	m_InternalOrExternalBoundary = other->m_InternalOrExternalBoundary;
	m_ParentBoundary = other->m_ParentBoundary;
	m_CorrespondingBoundary = other->m_CorrespondingBoundary;
}
void IfcRelSpaceBoundary2ndLevel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELSPACEBOUNDARY2NDLEVEL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingSpace ) { m_RelatingSpace->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedBuildingElement ) { stream << "#" << m_RelatedBuildingElement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_ConnectionGeometry ) { stream << "#" << m_ConnectionGeometry->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PhysicalOrVirtualBoundary ) { m_PhysicalOrVirtualBoundary->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_InternalOrExternalBoundary ) { m_InternalOrExternalBoundary->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ParentBoundary ) { stream << "#" << m_ParentBoundary->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_CorrespondingBoundary ) { stream << "#" << m_CorrespondingBoundary->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelSpaceBoundary2ndLevel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelSpaceBoundary2ndLevel::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelSpaceBoundary2ndLevel, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcRelSpaceBoundary2ndLevel, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
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
	readEntityReference( args[10], m_CorrespondingBoundary, map );
}
void IfcRelSpaceBoundary2ndLevel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelSpaceBoundary1stLevel::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "CorrespondingBoundary", m_CorrespondingBoundary ) );
}
void IfcRelSpaceBoundary2ndLevel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> Corresponds_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_Corresponds_inverse.size(); ++i ) { Corresponds_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSpaceBoundary2ndLevel>( m_Corresponds_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "Corresponds_inverse", Corresponds_inverse_vec_obj ) );
}
void IfcRelSpaceBoundary2ndLevel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelSpaceBoundary1stLevel::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelSpaceBoundary2ndLevel> ptr_self = dynamic_pointer_cast<IfcRelSpaceBoundary2ndLevel>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelSpaceBoundary2ndLevel::setInverseCounterparts: type mismatch" ); }
	if( m_CorrespondingBoundary )
	{
		m_CorrespondingBoundary->m_Corresponds_inverse.push_back( ptr_self );
	}
}
void IfcRelSpaceBoundary2ndLevel::unlinkSelf()
{
	IfcRelSpaceBoundary1stLevel::unlinkSelf();
	if( m_CorrespondingBoundary )
	{
		std::vector<weak_ptr<IfcRelSpaceBoundary2ndLevel> >& Corresponds_inverse = m_CorrespondingBoundary->m_Corresponds_inverse;
		std::vector<weak_ptr<IfcRelSpaceBoundary2ndLevel> >::iterator it_Corresponds_inverse;
		for( it_Corresponds_inverse = Corresponds_inverse.begin(); it_Corresponds_inverse != Corresponds_inverse.end(); ++it_Corresponds_inverse)
		{
			shared_ptr<IfcRelSpaceBoundary2ndLevel> self_candidate( *it_Corresponds_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				Corresponds_inverse.erase( it_Corresponds_inverse );
				break;
			}
		}
	}
}
