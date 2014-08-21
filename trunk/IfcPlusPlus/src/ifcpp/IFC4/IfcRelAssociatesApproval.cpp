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
shared_ptr<IfcPPObject> IfcRelAssociatesApproval::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRelAssociatesApproval> copy_self( new IfcRelAssociatesApproval() );
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
	for( size_t ii=0; ii<m_RelatedObjects.size(); ++ii )
	{
		auto item_ii = m_RelatedObjects[ii];
		if( item_ii )
		{
			copy_self->m_RelatedObjects.push_back( dynamic_pointer_cast<IfcDefinitionSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_RelatingApproval ) { copy_self->m_RelatingApproval = dynamic_pointer_cast<IfcApproval>( m_RelatingApproval->getDeepCopy(options) ); }
	return copy_self;
}
void IfcRelAssociatesApproval::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCRELASSOCIATESAPPROVAL" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeTypeList( stream, m_RelatedObjects, true );
	stream << ",";
	if( m_RelatingApproval ) { stream << "#" << m_RelatingApproval->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcRelAssociatesApproval::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcRelAssociatesApproval::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcRelAssociatesApproval, expecting 6, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
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
	IfcRelAssociates::getAttributesInverse( vec_attributes_inverse );
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
		for( auto it_ApprovedObjects_inverse = ApprovedObjects_inverse.begin(); it_ApprovedObjects_inverse != ApprovedObjects_inverse.end(); ++it_ApprovedObjects_inverse)
		{
			shared_ptr<IfcRelAssociatesApproval> self_candidate( *it_ApprovedObjects_inverse );
			if( self_candidate.get() == this )
			{
				ApprovedObjects_inverse.erase( it_ApprovedObjects_inverse );
				break;
			}
		}
	}
}
