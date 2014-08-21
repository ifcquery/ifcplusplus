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
#include "include/IfcCartesianPoint.h"
#include "include/IfcColourRgb.h"
#include "include/IfcDirection.h"
#include "include/IfcLabel.h"
#include "include/IfcLightSourceSpot.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPositivePlaneAngleMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcReal.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcLightSourceSpot 
IfcLightSourceSpot::IfcLightSourceSpot() {}
IfcLightSourceSpot::IfcLightSourceSpot( int id ) { m_id = id; }
IfcLightSourceSpot::~IfcLightSourceSpot() {}
shared_ptr<IfcPPObject> IfcLightSourceSpot::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcLightSourceSpot> copy_self( new IfcLightSourceSpot() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_LightColour ) { copy_self->m_LightColour = dynamic_pointer_cast<IfcColourRgb>( m_LightColour->getDeepCopy(options) ); }
	if( m_AmbientIntensity ) { copy_self->m_AmbientIntensity = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_AmbientIntensity->getDeepCopy(options) ); }
	if( m_Intensity ) { copy_self->m_Intensity = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_Intensity->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcCartesianPoint>( m_Position->getDeepCopy(options) ); }
	if( m_Radius ) { copy_self->m_Radius = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_Radius->getDeepCopy(options) ); }
	if( m_ConstantAttenuation ) { copy_self->m_ConstantAttenuation = dynamic_pointer_cast<IfcReal>( m_ConstantAttenuation->getDeepCopy(options) ); }
	if( m_DistanceAttenuation ) { copy_self->m_DistanceAttenuation = dynamic_pointer_cast<IfcReal>( m_DistanceAttenuation->getDeepCopy(options) ); }
	if( m_QuadricAttenuation ) { copy_self->m_QuadricAttenuation = dynamic_pointer_cast<IfcReal>( m_QuadricAttenuation->getDeepCopy(options) ); }
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcDirection>( m_Orientation->getDeepCopy(options) ); }
	if( m_ConcentrationExponent ) { copy_self->m_ConcentrationExponent = dynamic_pointer_cast<IfcReal>( m_ConcentrationExponent->getDeepCopy(options) ); }
	if( m_SpreadAngle ) { copy_self->m_SpreadAngle = dynamic_pointer_cast<IfcPositivePlaneAngleMeasure>( m_SpreadAngle->getDeepCopy(options) ); }
	if( m_BeamWidthAngle ) { copy_self->m_BeamWidthAngle = dynamic_pointer_cast<IfcPositivePlaneAngleMeasure>( m_BeamWidthAngle->getDeepCopy(options) ); }
	return copy_self;
}
void IfcLightSourceSpot::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCLIGHTSOURCESPOT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LightColour ) { stream << "#" << m_LightColour->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AmbientIntensity ) { m_AmbientIntensity->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Intensity ) { m_Intensity->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Radius ) { m_Radius->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ConstantAttenuation ) { m_ConstantAttenuation->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_DistanceAttenuation ) { m_DistanceAttenuation->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_QuadricAttenuation ) { m_QuadricAttenuation->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Orientation ) { stream << "#" << m_Orientation->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_ConcentrationExponent ) { m_ConcentrationExponent->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SpreadAngle ) { m_SpreadAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BeamWidthAngle ) { m_BeamWidthAngle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcLightSourceSpot::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcLightSourceSpot::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 13 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcLightSourceSpot, expecting 13, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_LightColour, map );
	m_AmbientIntensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[2] );
	m_Intensity = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[3] );
	readEntityReference( args[4], m_Position, map );
	m_Radius = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_ConstantAttenuation = IfcReal::createObjectFromSTEP( args[6] );
	m_DistanceAttenuation = IfcReal::createObjectFromSTEP( args[7] );
	m_QuadricAttenuation = IfcReal::createObjectFromSTEP( args[8] );
	readEntityReference( args[9], m_Orientation, map );
	m_ConcentrationExponent = IfcReal::createObjectFromSTEP( args[10] );
	m_SpreadAngle = IfcPositivePlaneAngleMeasure::createObjectFromSTEP( args[11] );
	m_BeamWidthAngle = IfcPositivePlaneAngleMeasure::createObjectFromSTEP( args[12] );
}
void IfcLightSourceSpot::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLightSourcePositional::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Orientation", m_Orientation ) );
	vec_attributes.push_back( std::make_pair( "ConcentrationExponent", m_ConcentrationExponent ) );
	vec_attributes.push_back( std::make_pair( "SpreadAngle", m_SpreadAngle ) );
	vec_attributes.push_back( std::make_pair( "BeamWidthAngle", m_BeamWidthAngle ) );
}
void IfcLightSourceSpot::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcLightSourcePositional::getAttributesInverse( vec_attributes_inverse );
}
void IfcLightSourceSpot::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLightSourcePositional::setInverseCounterparts( ptr_self_entity );
}
void IfcLightSourceSpot::unlinkSelf()
{
	IfcLightSourcePositional::unlinkSelf();
}
