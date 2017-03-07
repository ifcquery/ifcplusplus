/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
IfcLocalPlacement::IfcLocalPlacement() { m_entity_enum = IFCLOCALPLACEMENT; }
IfcLocalPlacement::IfcLocalPlacement( int id ) { m_id = id; m_entity_enum = IFCLOCALPLACEMENT; }
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
void IfcLocalPlacement::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcLocalPlacement, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
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
void IfcLocalPlacement::unlinkFromInverseCounterparts()
{
	IfcObjectPlacement::unlinkFromInverseCounterparts();
	if( m_PlacementRelTo )
	{
		std::vector<weak_ptr<IfcLocalPlacement> >& ReferencedByPlacements_inverse = m_PlacementRelTo->m_ReferencedByPlacements_inverse;
		for( auto it_ReferencedByPlacements_inverse = ReferencedByPlacements_inverse.begin(); it_ReferencedByPlacements_inverse != ReferencedByPlacements_inverse.end(); )
		{
			shared_ptr<IfcLocalPlacement> self_candidate( *it_ReferencedByPlacements_inverse );
			if( self_candidate.get() == this )
			{
				it_ReferencedByPlacements_inverse= ReferencedByPlacements_inverse.erase( it_ReferencedByPlacements_inverse );
			}
			else
			{
				++it_ReferencedByPlacements_inverse;
			}
		}
	}
}
