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
#include "include/IfcApproval.h"
#include "include/IfcDefinitionSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcRelAssociatesApproval.h"
#include "include/IfcText.h"

// ENTITY IfcRelAssociatesApproval 
IfcRelAssociatesApproval::IfcRelAssociatesApproval() {}
IfcRelAssociatesApproval::IfcRelAssociatesApproval( int id ) { m_id = id; }
IfcRelAssociatesApproval::~IfcRelAssociatesApproval() {}

// method setEntity takes over all attributes from another instance of the class
void IfcRelAssociatesApproval::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcRelAssociatesApproval> other = dynamic_pointer_cast<IfcRelAssociatesApproval>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_RelatedObjects = other->m_RelatedObjects;
	m_RelatingApproval = other->m_RelatingApproval;
}
void IfcRelAssociatesApproval::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCRELASSOCIATESAPPROVAL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_RelatingApproval ) { stream << "#" << m_RelatingApproval->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssociatesApproval::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesApproval::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesApproval, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcRelAssociatesApproval, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	readSelectList( args[4], m_RelatedObjects, map );
	readEntityReference( args[5], m_RelatingApproval, map );
}
void IfcRelAssociatesApproval::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRelAssociates::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RelatingApproval", m_RelatingApproval ) );
}
void IfcRelAssociatesApproval::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcRelAssociatesApproval::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRelAssociates::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcRelAssociatesApproval> ptr_self = dynamic_pointer_cast<IfcRelAssociatesApproval>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcRelAssociatesApproval::setInverseCounterparts: type mismatch" ); }
	if( m_RelatingApproval )
	{
		m_RelatingApproval->m_ApprovedObjects_inverse.push_back( ptr_self );
	}
}
void IfcRelAssociatesApproval::unlinkSelf()
{
	IfcRelAssociates::unlinkSelf();
	if( m_RelatingApproval )
	{
		std::vector<weak_ptr<IfcRelAssociatesApproval> >& ApprovedObjects_inverse = m_RelatingApproval->m_ApprovedObjects_inverse;
		std::vector<weak_ptr<IfcRelAssociatesApproval> >::iterator it_ApprovedObjects_inverse;
		for( it_ApprovedObjects_inverse = ApprovedObjects_inverse.begin(); it_ApprovedObjects_inverse != ApprovedObjects_inverse.end(); ++it_ApprovedObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesApproval> self_candidate( *it_ApprovedObjects_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				ApprovedObjects_inverse.erase( it_ApprovedObjects_inverse );
				break;
			}
		}
	}
}
