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
#include "include/IfcColourRgb.h"
#include "include/IfcDirection.h"
#include "include/IfcLabel.h"
#include "include/IfcLightSourceDirectional.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcLightSourceDirectional 
IfcLightSourceDirectional::IfcLightSourceDirectional() {}
IfcLightSourceDirectional::IfcLightSourceDirectional( int id ) { m_id = id; }
IfcLightSourceDirectional::~IfcLightSourceDirectional() {}

// method setEntity takes over all attributes from another instance of the class
void IfcLightSourceDirectional::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcLightSourceDirectional> other = dynamic_pointer_cast<IfcLightSourceDirectional>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_LightColour = other->m_LightColour;
	m_AmbientIntensity = other->m_AmbientIntensity;
	m_Intensity = other->m_Intensity;
	m_Orientation = other->m_Orientation;
}
void IfcLightSourceDirectional::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCLIGHTSOURCEDIRECTIONAL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LightColour ) { stream << "#" << m_LightColour->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_AmbientIntensity ) { m_AmbientIntensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Intensity ) { m_Intensity->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Orientation ) { stream << "#" << m_Orientation->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcLightSourceDirectional::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightSourceDirectional::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightSourceDirectional, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcLightSourceDirectional, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_LightColour, map );
	m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[2] );
	m_Intensity = IfcNormalisedRatioMeasure::createObjectFromStepData( args[3] );
	readEntityReference( args[4], m_Orientation, map );
}
void IfcLightSourceDirectional::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLightSource::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Orientation", m_Orientation ) );
}
void IfcLightSourceDirectional::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcLightSourceDirectional::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLightSource::setInverseCounterparts( ptr_self_entity );
}
void IfcLightSourceDirectional::unlinkSelf()
{
	IfcLightSource::unlinkSelf();
}
