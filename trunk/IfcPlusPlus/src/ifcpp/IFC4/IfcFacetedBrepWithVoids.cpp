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
#include "include/IfcClosedShell.h"
#include "include/IfcFacetedBrepWithVoids.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFacetedBrepWithVoids 
IfcFacetedBrepWithVoids::IfcFacetedBrepWithVoids() {}
IfcFacetedBrepWithVoids::IfcFacetedBrepWithVoids( int id ) { m_id = id; }
IfcFacetedBrepWithVoids::~IfcFacetedBrepWithVoids() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFacetedBrepWithVoids::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFacetedBrepWithVoids> other = dynamic_pointer_cast<IfcFacetedBrepWithVoids>(other_entity);
	if( !other) { return; }
	m_Outer = other->m_Outer;
	m_Voids = other->m_Voids;
}
void IfcFacetedBrepWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFACETEDBREPWITHVOIDS" << "(";
	if( m_Outer ) { stream << "#" << m_Outer->getId(); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Voids );
	stream << ");";
}
void IfcFacetedBrepWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFacetedBrepWithVoids::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFacetedBrepWithVoids, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcFacetedBrepWithVoids, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Outer, map );
	readEntityReferenceList( args[1], m_Voids, map );
}
void IfcFacetedBrepWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFacetedBrep::getAttributes( vec_attributes );
}
void IfcFacetedBrepWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcFacetedBrepWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFacetedBrep::setInverseCounterparts( ptr_self_entity );
}
void IfcFacetedBrepWithVoids::unlinkSelf()
{
	IfcFacetedBrep::unlinkSelf();
}
