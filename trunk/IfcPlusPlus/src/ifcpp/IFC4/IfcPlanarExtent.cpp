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
#include "include/IfcLengthMeasure.h"
#include "include/IfcPlanarExtent.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPlanarExtent 
IfcPlanarExtent::IfcPlanarExtent() {}
IfcPlanarExtent::IfcPlanarExtent( int id ) { m_id = id; }
IfcPlanarExtent::~IfcPlanarExtent() {}

// method setEntity takes over all attributes from another instance of the class
void IfcPlanarExtent::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcPlanarExtent> other = dynamic_pointer_cast<IfcPlanarExtent>(other_entity);
	if( !other) { return; }
	m_SizeInX = other->m_SizeInX;
	m_SizeInY = other->m_SizeInY;
}
void IfcPlanarExtent::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPLANAREXTENT" << "(";
	if( m_SizeInX ) { m_SizeInX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SizeInY ) { m_SizeInY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPlanarExtent::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPlanarExtent::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPlanarExtent, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcPlanarExtent, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_SizeInX = IfcLengthMeasure::createObjectFromStepData( args[0] );
	m_SizeInY = IfcLengthMeasure::createObjectFromStepData( args[1] );
}
void IfcPlanarExtent::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SizeInX", m_SizeInX ) );
	vec_attributes.push_back( std::make_pair( "SizeInY", m_SizeInY ) );
}
void IfcPlanarExtent::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcPlanarExtent::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcPlanarExtent::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
