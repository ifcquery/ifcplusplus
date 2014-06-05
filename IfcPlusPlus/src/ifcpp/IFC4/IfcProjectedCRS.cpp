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
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcNamedUnit.h"
#include "include/IfcProjectedCRS.h"
#include "include/IfcText.h"

// ENTITY IfcProjectedCRS 
IfcProjectedCRS::IfcProjectedCRS() {}
IfcProjectedCRS::IfcProjectedCRS( int id ) { m_id = id; }
IfcProjectedCRS::~IfcProjectedCRS() {}

// method setEntity takes over all attributes from another instance of the class
void IfcProjectedCRS::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcProjectedCRS> other = dynamic_pointer_cast<IfcProjectedCRS>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_GeodeticDatum = other->m_GeodeticDatum;
	m_VerticalDatum = other->m_VerticalDatum;
	m_MapProjection = other->m_MapProjection;
	m_MapZone = other->m_MapZone;
	m_MapUnit = other->m_MapUnit;
}
void IfcProjectedCRS::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCPROJECTEDCRS" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_GeodeticDatum ) { m_GeodeticDatum->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_VerticalDatum ) { m_VerticalDatum->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MapProjection ) { m_MapProjection->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MapZone ) { m_MapZone->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MapUnit ) { stream << "#" << m_MapUnit->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcProjectedCRS::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcProjectedCRS::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcProjectedCRS, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcProjectedCRS, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Description = IfcText::createObjectFromStepData( args[1] );
	m_GeodeticDatum = IfcIdentifier::createObjectFromStepData( args[2] );
	m_VerticalDatum = IfcIdentifier::createObjectFromStepData( args[3] );
	m_MapProjection = IfcIdentifier::createObjectFromStepData( args[4] );
	m_MapZone = IfcIdentifier::createObjectFromStepData( args[5] );
	readEntityReference( args[6], m_MapUnit, map );
}
void IfcProjectedCRS::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCoordinateReferenceSystem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MapProjection", m_MapProjection ) );
	vec_attributes.push_back( std::make_pair( "MapZone", m_MapZone ) );
	vec_attributes.push_back( std::make_pair( "MapUnit", m_MapUnit ) );
}
void IfcProjectedCRS::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcProjectedCRS::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCoordinateReferenceSystem::setInverseCounterparts( ptr_self_entity );
}
void IfcProjectedCRS::unlinkSelf()
{
	IfcCoordinateReferenceSystem::unlinkSelf();
}
