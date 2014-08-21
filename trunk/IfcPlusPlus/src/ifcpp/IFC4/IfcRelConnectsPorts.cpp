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
#include "include/IfcElement.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPort.h"
#include "include/IfcRelConnectsPorts.h"
#include "include/IfcText.h"

// ENTITY IfcRelConnectsPorts 
IfcRelConnectsPorts::IfcRelConnectsPorts() {}
IfcRelConnectsPorts::IfcRelConnectsPorts( int id ) { m_id = id; }
IfcRelConnectsPorts::~IfcRelConnectsPorts() {}
shared_ptr<IfcPPObject> IfcRelConnectsPorts::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelConnectsPorts> copy_self( new IfcRelConnectsPorts() );
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
	if( m_RelatingPort ) { copy_self->m_RelatingPort = dynamic_pointer_cast<IfcPort>( m_RelatingPort->getDeepCopy(options) ); }
	if( m_RelatedPort ) { copy_self->m_RelatedPort = dynamic_pointer_cast<IfcPort>( m_RelatedPort->getDeepCopy(options) ); }
	if( m_RealizingElement ) { copy_self->m_RealizingElement = dynamic_pointer_cast<IfcElement>( m_RealizingElement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelConnectsPorts::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELCONNECTSPORTS" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RelatingPort ) { stream << "#" << m_RelatingPort->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_RelatedPort ) { stream << "#" << m_RelatedPort->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_RealizingElement ) { stream << "#" << m_RealizingElement->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelConnectsPorts::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelConnectsPorts::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelConnectsPorts, expecting 7, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_RelatingPort, map );
	readEntityReference( args[5], m_RelatedPort, map );
	readEntityReference( args[6], m_RealizingElement, map );
}
void IfcRelConnectsPorts::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelConnects::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingPort", m_RelatingPort ) );
	vec_attributes.push_back( std::make_pair( "RelatedPort", m_RelatedPort ) );
	vec_attributes.push_back( std::make_pair( "RealizingElement", m_RealizingElement ) );
}
void IfcRelConnectsPorts::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRelConnects::getAttributesInverse( vec_attributes_inverse );
}
void IfcRelConnectsPorts::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelConnects::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelConnectsPorts> ptr_self = dynamic_pointer_cast<IfcRelConnectsPorts>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelConnectsPorts::setInverseCounterparts: type mismatch" ); }
	if( m_RelatedPort )
	{
		m_RelatedPort->m_ConnectedFrom_inverse.push_back( ptr_self );
	}
	if( m_RelatingPort )
	{
		m_RelatingPort->m_ConnectedTo_inverse.push_back( ptr_self );
	}
}
void IfcRelConnectsPorts::unlinkSelf()
{
	IfcRelConnects::unlinkSelf();
	if( m_RelatedPort )
	{
		std::vector<weak_ptr<IfcRelConnectsPorts> >& ConnectedFrom_inverse = m_RelatedPort->m_ConnectedFrom_inverse;
		for( auto it_ConnectedFrom_inverse = ConnectedFrom_inverse.begin(); it_ConnectedFrom_inverse != ConnectedFrom_inverse.end(); ++it_ConnectedFrom_inverse)
		{
			shared_ptr<IfcRelConnectsPorts> self_candidate( *it_ConnectedFrom_inverse );
			if( self_candidate.get() == this )
			{
				ConnectedFrom_inverse.erase( it_ConnectedFrom_inverse );
				break;
			}
		}
	}
	if( m_RelatingPort )
	{
		std::vector<weak_ptr<IfcRelConnectsPorts> >& ConnectedTo_inverse = m_RelatingPort->m_ConnectedTo_inverse;
		for( auto it_ConnectedTo_inverse = ConnectedTo_inverse.begin(); it_ConnectedTo_inverse != ConnectedTo_inverse.end(); ++it_ConnectedTo_inverse)
		{
			shared_ptr<IfcRelConnectsPorts> self_candidate( *it_ConnectedTo_inverse );
			if( self_candidate.get() == this )
			{
				ConnectedTo_inverse.erase( it_ConnectedTo_inverse );
				break;
			}
		}
	}
}
