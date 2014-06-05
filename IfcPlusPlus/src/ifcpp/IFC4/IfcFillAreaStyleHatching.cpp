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
#include "include/IfcCartesianPoint.h"
#include "include/IfcCurveStyle.h"
#include "include/IfcFillAreaStyleHatching.h"
#include "include/IfcHatchLineDistanceSelect.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFillAreaStyleHatching 
IfcFillAreaStyleHatching::IfcFillAreaStyleHatching() {}
IfcFillAreaStyleHatching::IfcFillAreaStyleHatching( int id ) { m_id = id; }
IfcFillAreaStyleHatching::~IfcFillAreaStyleHatching() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFillAreaStyleHatching::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFillAreaStyleHatching> other = dynamic_pointer_cast<IfcFillAreaStyleHatching>(other_entity);
	if( !other) { return; }
	m_HatchLineAppearance = other->m_HatchLineAppearance;
	m_StartOfNextHatchLine = other->m_StartOfNextHatchLine;
	m_PointOfReferenceHatchLine = other->m_PointOfReferenceHatchLine;
	m_PatternStart = other->m_PatternStart;
	m_HatchLineAngle = other->m_HatchLineAngle;
}
void IfcFillAreaStyleHatching::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFILLAREASTYLEHATCHING" << "(";
	if( m_HatchLineAppearance ) { stream << "#" << m_HatchLineAppearance->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_StartOfNextHatchLine ) { m_StartOfNextHatchLine->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_PointOfReferenceHatchLine ) { stream << "#" << m_PointOfReferenceHatchLine->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_PatternStart ) { stream << "#" << m_PatternStart->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_HatchLineAngle ) { m_HatchLineAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFillAreaStyleHatching::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFillAreaStyleHatching::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFillAreaStyleHatching, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcFillAreaStyleHatching, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_HatchLineAppearance, map );
	m_StartOfNextHatchLine = IfcHatchLineDistanceSelect::createObjectFromStepData( args[1], map );
	readEntityReference( args[2], m_PointOfReferenceHatchLine, map );
	readEntityReference( args[3], m_PatternStart, map );
	m_HatchLineAngle = IfcPlaneAngleMeasure::createObjectFromStepData( args[4] );
}
void IfcFillAreaStyleHatching::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "HatchLineAppearance", m_HatchLineAppearance ) );
	vec_attributes.push_back( std::make_pair( "StartOfNextHatchLine", m_StartOfNextHatchLine ) );
	vec_attributes.push_back( std::make_pair( "PointOfReferenceHatchLine", m_PointOfReferenceHatchLine ) );
	vec_attributes.push_back( std::make_pair( "PatternStart", m_PatternStart ) );
	vec_attributes.push_back( std::make_pair( "HatchLineAngle", m_HatchLineAngle ) );
}
void IfcFillAreaStyleHatching::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcFillAreaStyleHatching::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFillAreaStyleHatching::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
