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
#include "include/IfcGridPlacement.h"
#include "include/IfcGridPlacementDirectionSelect.h"
#include "include/IfcLocalPlacement.h"
#include "include/IfcProduct.h"
#include "include/IfcVirtualGridIntersection.h"

// ENTITY IfcGridPlacement 
IfcGridPlacement::IfcGridPlacement() {}
IfcGridPlacement::IfcGridPlacement( int id ) { m_id = id; }
IfcGridPlacement::~IfcGridPlacement() {}

// method setEntity takes over all attributes from another instance of the class
void IfcGridPlacement::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcGridPlacement> other = dynamic_pointer_cast<IfcGridPlacement>(other_entity);
	if( !other) { return; }
	m_PlacementLocation = other->m_PlacementLocation;
	m_PlacementRefDirection = other->m_PlacementRefDirection;
}
void IfcGridPlacement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCGRIDPLACEMENT" << "(";
	if( m_PlacementLocation ) { stream << "#" << m_PlacementLocation->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PlacementRefDirection ) { m_PlacementRefDirection->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcGridPlacement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGridPlacement::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcGridPlacement, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcGridPlacement, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_PlacementLocation, map );
	m_PlacementRefDirection = IfcGridPlacementDirectionSelect::createObjectFromStepData( args[1], map );
}
void IfcGridPlacement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PlacementLocation", m_PlacementLocation ) );
	vec_attributes.push_back( std::make_pair( "PlacementRefDirection", m_PlacementRefDirection ) );
}
void IfcGridPlacement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcGridPlacement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectPlacement::setInverseCounterparts( ptr_self_entity );
}
void IfcGridPlacement::unlinkSelf()
{
	IfcObjectPlacement::unlinkSelf();
}
