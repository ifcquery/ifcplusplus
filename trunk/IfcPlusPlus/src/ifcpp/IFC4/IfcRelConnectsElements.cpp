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
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelConnectsElements.h"
#include "include/IfcText.h"

// ENTITY IfcRelConnectsElements 
IfcRelConnectsElements::IfcRelConnectsElements() {}
IfcRelConnectsElements::IfcRelConnectsElements( int id ) { m_id = id; }
IfcRelConnectsElements::~IfcRelConnectsElements() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelConnectsElements::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelConnectsElements> other = dynamic_pointer_cast<IfcRelConnectsElements>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ConnectionGeometry = other->m_ConnectionGeometry;
	m_RelatingElement = other->m_RelatingElement;
	m_RelatedElement = other->m_RelatedElement;
}
void IfcRelConnectsElements::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELCONNECTSELEMENTS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ConnectionGeometry ) { stream << "#" << m_ConnectionGeometry->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatingElement ) { stream << "#" << m_RelatingElement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_RelatedElement ) { stream << "#" << m_RelatedElement->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelConnectsElements::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelConnectsElements::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelConnectsElements, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcRelConnectsElements, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_ConnectionGeometry, map );
	readEntityReference( args[5], m_RelatingElement, map );
	readEntityReference( args[6], m_RelatedElement, map );
}
void IfcRelConnectsElements::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ConnectionGeometry", m_ConnectionGeometry ) );
	vec_attributes.push_back( std::make_pair( "RelatingElement", m_RelatingElement ) );
	vec_attributes.push_back( std::make_pair( "RelatedElement", m_RelatedElement ) );
}
void IfcRelConnectsElements::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelConnectsElements::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelConnectsElements> ptr_self = dynamic_pointer_cast<IfcRelConnectsElements>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelConnectsElements::setInverseCounterparts: type mismatch" ); }
	if( m_RelatedElement )
	{
		m_RelatedElement->m_ConnectedFrom_inverse.push_back( ptr_self );
	}
	if( m_RelatingElement )
	{
		m_RelatingElement->m_ConnectedTo_inverse.push_back( ptr_self );
	}
}
void IfcRelConnectsElements::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	if( m_RelatedElement )
	{
		std::vector<weak_ptr<IfcRelConnectsElements> >& ConnectedFrom_inverse = m_RelatedElement->m_ConnectedFrom_inverse;
		std::vector<weak_ptr<IfcRelConnectsElements> >::iterator it_ConnectedFrom_inverse;
		for( it_ConnectedFrom_inverse = ConnectedFrom_inverse.begin(); it_ConnectedFrom_inverse != ConnectedFrom_inverse.end(); ++it_ConnectedFrom_inverse)
		{
			shared_ptr<IfcRelConnectsElements> self_candidate( *it_ConnectedFrom_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ConnectedFrom_inverse.erase( it_ConnectedFrom_inverse );
				break;
			}
		}
	}
	if( m_RelatingElement )
	{
		std::vector<weak_ptr<IfcRelConnectsElements> >& ConnectedTo_inverse = m_RelatingElement->m_ConnectedTo_inverse;
		std::vector<weak_ptr<IfcRelConnectsElements> >::iterator it_ConnectedTo_inverse;
		for( it_ConnectedTo_inverse = ConnectedTo_inverse.begin(); it_ConnectedTo_inverse != ConnectedTo_inverse.end(); ++it_ConnectedTo_inverse)
		{
			shared_ptr<IfcRelConnectsElements> self_candidate( *it_ConnectedTo_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ConnectedTo_inverse.erase( it_ConnectedTo_inverse );
				break;
			}
		}
	}
}
