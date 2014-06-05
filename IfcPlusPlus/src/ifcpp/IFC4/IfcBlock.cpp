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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcBlock.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBlock 
IfcBlock::IfcBlock() {}
IfcBlock::IfcBlock( int id ) { m_id = id; }
IfcBlock::~IfcBlock() {}

// method setEntity takes over all attributes from another instance of the class
void IfcBlock::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcBlock> other = dynamic_pointer_cast<IfcBlock>(other_entity);
	if( !other) { return; }
	m_Position = other->m_Position;
	m_XLength = other->m_XLength;
	m_YLength = other->m_YLength;
	m_ZLength = other->m_ZLength;
}
void IfcBlock::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCBLOCK" << "(";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_XLength ) { m_XLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_YLength ) { m_YLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ZLength ) { m_ZLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcBlock::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBlock::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBlock, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcBlock, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_Position, map );
	m_XLength = IfcPositiveLengthMeasure::createObjectFromStepData( args[1] );
	m_YLength = IfcPositiveLengthMeasure::createObjectFromStepData( args[2] );
	m_ZLength = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
}
void IfcBlock::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCsgPrimitive3D::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "XLength", m_XLength ) );
	vec_attributes.push_back( std::make_pair( "YLength", m_YLength ) );
	vec_attributes.push_back( std::make_pair( "ZLength", m_ZLength ) );
}
void IfcBlock::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcBlock::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCsgPrimitive3D::setInverseCounterparts( ptr_self_entity );
}
void IfcBlock::unlinkSelf()
{
	IfcCsgPrimitive3D::unlinkSelf();
}
