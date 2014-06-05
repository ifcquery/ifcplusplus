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
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcSpatialElement.h"
#include "include/IfcSystem.h"
#include "include/IfcText.h"

// ENTITY IfcRelServicesBuildings 
IfcRelServicesBuildings::IfcRelServicesBuildings() {}
IfcRelServicesBuildings::IfcRelServicesBuildings( int id ) { m_id = id; }
IfcRelServicesBuildings::~IfcRelServicesBuildings() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelServicesBuildings::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelServicesBuildings> other = dynamic_pointer_cast<IfcRelServicesBuildings>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatingSystem = other->m_RelatingSystem;
	m_RelatedBuildings = other->m_RelatedBuildings;
}
void IfcRelServicesBuildings::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELSERVICESBUILDINGS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingSystem ) { stream << "#" << m_RelatingSystem->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_RelatedBuildings );
	stream << ");";
}
void IfcRelServicesBuildings::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelServicesBuildings::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelServicesBuildings, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelServicesBuildings, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_RelatingSystem, map );
	readEntityReferenceList( args[5], m_RelatedBuildings, map );
}
void IfcRelServicesBuildings::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingSystem", m_RelatingSystem ) );
}
void IfcRelServicesBuildings::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelServicesBuildings::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelServicesBuildings> ptr_self = dynamic_pointer_cast<IfcRelServicesBuildings>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelServicesBuildings::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_RelatedBuildings.size(); ++i )
	{
		if( m_RelatedBuildings[i] )
		{
			m_RelatedBuildings[i]->m_ServicedBySystems_inverse.push_back( ptr_self );
		}
	}
	if( m_RelatingSystem )
	{
		m_RelatingSystem->m_ServicesBuildings_inverse.push_back( ptr_self );
	}
}
void IfcRelServicesBuildings::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	for( int i=0; i<m_RelatedBuildings.size(); ++i )
	{
		if( m_RelatedBuildings[i] )
		{
			std::vector<weak_ptr<IfcRelServicesBuildings> >& ServicedBySystems_inverse = m_RelatedBuildings[i]->m_ServicedBySystems_inverse;
			std::vector<weak_ptr<IfcRelServicesBuildings> >::iterator it_ServicedBySystems_inverse;
			for( it_ServicedBySystems_inverse = ServicedBySystems_inverse.begin(); it_ServicedBySystems_inverse != ServicedBySystems_inverse.end(); ++it_ServicedBySystems_inverse)
			{
				shared_ptr<IfcRelServicesBuildings> self_candidate( *it_ServicedBySystems_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					ServicedBySystems_inverse.erase( it_ServicedBySystems_inverse );
					break;
				}
			}
		}
	}
	if( m_RelatingSystem )
	{
		std::vector<weak_ptr<IfcRelServicesBuildings> >& ServicesBuildings_inverse = m_RelatingSystem->m_ServicesBuildings_inverse;
		std::vector<weak_ptr<IfcRelServicesBuildings> >::iterator it_ServicesBuildings_inverse;
		for( it_ServicesBuildings_inverse = ServicesBuildings_inverse.begin(); it_ServicesBuildings_inverse != ServicesBuildings_inverse.end(); ++it_ServicesBuildings_inverse)
		{
			shared_ptr<IfcRelServicesBuildings> self_candidate( *it_ServicesBuildings_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ServicesBuildings_inverse.erase( it_ServicesBuildings_inverse );
				break;
			}
		}
	}
}
