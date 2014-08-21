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
#include "include/IfcAxis2Placement.h"
#include "include/IfcLocalPlacement.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcProduct.h"

// ENTITY IfcLocalPlacement 
IfcLocalPlacement::IfcLocalPlacement() {}
IfcLocalPlacement::IfcLocalPlacement( int id ) { m_id = id; }
IfcLocalPlacement::~IfcLocalPlacement() {}
shared_ptr<IfcPPObject> IfcLocalPlacement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLocalPlacement> copy_self( new IfcLocalPlacement() );
	if( m_PlacementRelTo )
	{
		if( options.shallow_copy_IfcLocalPlacement_PlacementRelTo ) { copy_self->m_PlacementRelTo = m_PlacementRelTo; }
		else { copy_self->m_PlacementRelTo = dynamic_pointer_cast<IfcObjectPlacement>( m_PlacementRelTo->getDeepCopy(options) ); }
	}
	if( m_RelativePlacement ) { copy_self->m_RelativePlacement = dynamic_pointer_cast<IfcAxis2Placement>( m_RelativePlacement->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLocalPlacement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLOCALPLACEMENT" << "(";
	if( m_PlacementRelTo ) { stream << "#" << m_PlacementRelTo->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_RelativePlacement ) { m_RelativePlacement->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcLocalPlacement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLocalPlacement::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLocalPlacement, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_PlacementRelTo, map );
	m_RelativePlacement = IfcAxis2Placement::createObjectFromSTEP( args[1], map );
}
void IfcLocalPlacement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PlacementRelTo", m_PlacementRelTo ) );
	vec_attributes.push_back( std::make_pair( "RelativePlacement", m_RelativePlacement ) );
}
void IfcLocalPlacement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObjectPlacement::getAttributesInverse( vec_attributes_inverse );
}
void IfcLocalPlacement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectPlacement::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcLocalPlacement> ptr_self = dynamic_pointer_cast<IfcLocalPlacement>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcLocalPlacement::setInverseCounterparts: type mismatch" ); }
	if( m_PlacementRelTo )
	{
		m_PlacementRelTo->m_ReferencedByPlacements_inverse.push_back( ptr_self );
	}
}
void IfcLocalPlacement::unlinkSelf()
{
	IfcObjectPlacement::unlinkSelf();
	if( m_PlacementRelTo )
	{
		std::vector<weak_ptr<IfcLocalPlacement> >& ReferencedByPlacements_inverse = m_PlacementRelTo->m_ReferencedByPlacements_inverse;
		for( auto it_ReferencedByPlacements_inverse = ReferencedByPlacements_inverse.begin(); it_ReferencedByPlacements_inverse != ReferencedByPlacements_inverse.end(); ++it_ReferencedByPlacements_inverse)
		{
			shared_ptr<IfcLocalPlacement> self_candidate( *it_ReferencedByPlacements_inverse );
			if( self_candidate.get() == this )
			{
				ReferencedByPlacements_inverse.erase( it_ReferencedByPlacements_inverse );
				break;
			}
		}
	}
}
