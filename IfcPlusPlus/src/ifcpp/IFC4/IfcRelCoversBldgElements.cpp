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
#include "include/IfcBuildingElement.h"
#include "include/IfcCovering.h"
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelCoversBldgElements.h"
#include "include/IfcText.h"

// ENTITY IfcRelCoversBldgElements 
IfcRelCoversBldgElements::IfcRelCoversBldgElements() {}
IfcRelCoversBldgElements::IfcRelCoversBldgElements( int id ) { m_id = id; }
IfcRelCoversBldgElements::~IfcRelCoversBldgElements() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelCoversBldgElements::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelCoversBldgElements> other = dynamic_pointer_cast<IfcRelCoversBldgElements>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingBuildingElement = other->m_RelatingBuildingElement;
	m_RelatedCoverings = other->m_RelatedCoverings;
}
void IfcRelCoversBldgElements::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELCOVERSBLDGELEMENTS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingBuildingElement ) { stream << "#" << m_RelatingBuildingElement->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedCoverings );
	stream << ");";
}
void IfcRelCoversBldgElements::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelCoversBldgElements::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelCoversBldgElements, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelCoversBldgElements, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingBuildingElement, map );
	readEntityReferenceList( args[5], m_RelatedCoverings, map );
}
void IfcRelCoversBldgElements::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingBuildingElement", m_RelatingBuildingElement ) );
}
void IfcRelCoversBldgElements::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelCoversBldgElements::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelCoversBldgElements> ptr_self = dynamic_pointer_cast<IfcRelCoversBldgElements>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelCoversBldgElements::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedCoverings.size(); ++i )
	{
		if( m_RelatedCoverings[i] )
		{
			m_RelatedCoverings[i]->m_CoversElements_inverse.push_back( ptr_self );
		}
	}
	shared_ptr<IfcBuildingElement>  RelatingBuildingElement_IfcBuildingElement = dynamic_pointer_cast<IfcBuildingElement>( m_RelatingBuildingElement );
	if( RelatingBuildingElement_IfcBuildingElement )
	{
		RelatingBuildingElement_IfcBuildingElement->m_HasCoverings_inverse.push_back( ptr_self );
	}
}
void IfcRelCoversBldgElements::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	for( int i=0; i<m_RelatedCoverings.size(); ++i )
	{
		if( m_RelatedCoverings[i] )
		{
			std::vector<weak_ptr<IfcRelCoversBldgElements> >& CoversElements_inverse = m_RelatedCoverings[i]->m_CoversElements_inverse;
			std::vector<weak_ptr<IfcRelCoversBldgElements> >::iterator it_CoversElements_inverse;
			for( it_CoversElements_inverse = CoversElements_inverse.begin(); it_CoversElements_inverse != CoversElements_inverse.end(); ++it_CoversElements_inverse)
			{
				shared_ptr<IfcRelCoversBldgElements> self_candidate( *it_CoversElements_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					CoversElements_inverse.erase( it_CoversElements_inverse );
					break;
				}
			}
		}
	}
	shared_ptr<IfcBuildingElement>  RelatingBuildingElement_IfcBuildingElement = dynamic_pointer_cast<IfcBuildingElement>( m_RelatingBuildingElement );
	if( RelatingBuildingElement_IfcBuildingElement )
	{
		std::vector<weak_ptr<IfcRelCoversBldgElements> >& HasCoverings_inverse = RelatingBuildingElement_IfcBuildingElement->m_HasCoverings_inverse;
		std::vector<weak_ptr<IfcRelCoversBldgElements> >::iterator it_HasCoverings_inverse;
		for( it_HasCoverings_inverse = HasCoverings_inverse.begin(); it_HasCoverings_inverse != HasCoverings_inverse.end(); ++it_HasCoverings_inverse)
		{
			shared_ptr<IfcRelCoversBldgElements> self_candidate( *it_HasCoverings_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasCoverings_inverse.erase( it_HasCoverings_inverse );
				break;
			}
		}
	}
}
