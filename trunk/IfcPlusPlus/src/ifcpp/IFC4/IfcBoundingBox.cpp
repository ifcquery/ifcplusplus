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
#include "include/IfcBoundingBox.h"
#include "include/IfcCartesianPoint.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBoundingBox 
IfcBoundingBox::IfcBoundingBox() {}
IfcBoundingBox::IfcBoundingBox( int id ) { m_id = id; }
IfcBoundingBox::~IfcBoundingBox() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBoundingBox::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBoundingBox> other = dynamic_pointer_cast<IfcBoundingBox>(other_entity);
	if( !other) { return; }
	m_Corner = other->m_Corner;
	m_XDim = other->m_XDim;
	m_YDim = other->m_YDim;
	m_ZDim = other->m_ZDim;
}
void IfcBoundingBox::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBOUNDINGBOX" << "(";
	if( m_Corner ) { stream << "#" << m_Corner->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_XDim ) { m_XDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_YDim ) { m_YDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ZDim ) { m_ZDim->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBoundingBox::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundingBox::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBoundingBox, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcBoundingBox, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Corner, map );
	m_XDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[1] );
	m_YDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
	m_ZDim = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
}
void IfcBoundingBox::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Corner", m_Corner ) );
	vec_attributes.push_back( std::make_pair( "XDim", m_XDim ) );
	vec_attributes.push_back( std::make_pair( "YDim", m_YDim ) );
	vec_attributes.push_back( std::make_pair( "ZDim", m_ZDim ) );
}
void IfcBoundingBox::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBoundingBox::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundingBox::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
