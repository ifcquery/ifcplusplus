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
#include "include/IfcClosedShell.h"
#include "include/IfcFacetedBrepWithVoids.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFacetedBrepWithVoids 
IfcFacetedBrepWithVoids::IfcFacetedBrepWithVoids() {}
IfcFacetedBrepWithVoids::IfcFacetedBrepWithVoids( int id ) { m_id = id; }
IfcFacetedBrepWithVoids::~IfcFacetedBrepWithVoids() {}
shared_ptr<IfcPPObject> IfcFacetedBrepWithVoids::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFacetedBrepWithVoids> copy_self( new IfcFacetedBrepWithVoids() );
	if( m_Outer ) { copy_self->m_Outer = dynamic_pointer_cast<IfcClosedShell>( m_Outer->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Voids.size(); ++ii )
	{
		auto item_ii = m_Voids[ii];
		if( item_ii )
		{
			copy_self->m_Voids.push_back( dynamic_pointer_cast<IfcClosedShell>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcFacetedBrepWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFACETEDBREPWITHVOIDS" << "(";
	if( m_Outer ) { stream << "#" << m_Outer->m_id; } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Voids );
	stream << ");";
}
void IfcFacetedBrepWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFacetedBrepWithVoids::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFacetedBrepWithVoids, expecting 2, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Outer, map );
	readEntityReferenceList( args[1], m_Voids, map );
}
void IfcFacetedBrepWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFacetedBrep::getAttributes( vec_attributes );
	if( m_Voids.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Voids_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Voids.begin(), m_Voids.end(), std::back_inserter( Voids_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Voids", Voids_vec_object ) );
	}
}
void IfcFacetedBrepWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFacetedBrep::getAttributesInverse( vec_attributes_inverse );
}
void IfcFacetedBrepWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFacetedBrep::setInverseCounterparts( ptr_self_entity );
}
void IfcFacetedBrepWithVoids::unlinkSelf()
{
	IfcFacetedBrep::unlinkSelf();
}
