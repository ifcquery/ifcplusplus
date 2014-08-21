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
#include "include/IfcAsymmetricIShapeProfileDef.h"
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPlaneAngleMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"

// ENTITY IfcAsymmetricIShapeProfileDef 
IfcAsymmetricIShapeProfileDef::IfcAsymmetricIShapeProfileDef() {}
IfcAsymmetricIShapeProfileDef::IfcAsymmetricIShapeProfileDef( int id ) { m_id = id; }
IfcAsymmetricIShapeProfileDef::~IfcAsymmetricIShapeProfileDef() {}
shared_ptr<IfcPPObject> IfcAsymmetricIShapeProfileDef::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAsymmetricIShapeProfileDef> copy_self( new IfcAsymmetricIShapeProfileDef() );
	if( m_ProfileType ) { copy_self->m_ProfileType = dynamic_pointer_cast<IfcProfileTypeEnum>( m_ProfileType->getDeepCopy(options) ); }
	if( m_ProfileName ) { copy_self->m_ProfileName = dynamic_pointer_cast<IfcLabel>( m_ProfileName->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement2D>( m_Position->getDeepCopy(options) ); }
	if( m_BottomFlangeWidth ) { copy_self->m_BottomFlangeWidth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_BottomFlangeWidth->getDeepCopy(options) ); }
	if( m_OverallDepth ) { copy_self->m_OverallDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_OverallDepth->getDeepCopy(options) ); }
	if( m_WebThickness ) { copy_self->m_WebThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_WebThickness->getDeepCopy(options) ); }
	if( m_BottomFlangeThickness ) { copy_self->m_BottomFlangeThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_BottomFlangeThickness->getDeepCopy(options) ); }
	if( m_BottomFlangeFilletRadius ) { copy_self->m_BottomFlangeFilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_BottomFlangeFilletRadius->getDeepCopy(options) ); }
	if( m_TopFlangeWidth ) { copy_self->m_TopFlangeWidth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TopFlangeWidth->getDeepCopy(options) ); }
	if( m_TopFlangeThickness ) { copy_self->m_TopFlangeThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TopFlangeThickness->getDeepCopy(options) ); }
	if( m_TopFlangeFilletRadius ) { copy_self->m_TopFlangeFilletRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_TopFlangeFilletRadius->getDeepCopy(options) ); }
	if( m_BottomFlangeEdgeRadius ) { copy_self->m_BottomFlangeEdgeRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_BottomFlangeEdgeRadius->getDeepCopy(options) ); }
	if( m_BottomFlangeSlope ) { copy_self->m_BottomFlangeSlope = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_BottomFlangeSlope->getDeepCopy(options) ); }
	if( m_TopFlangeEdgeRadius ) { copy_self->m_TopFlangeEdgeRadius = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_TopFlangeEdgeRadius->getDeepCopy(options) ); }
	if( m_TopFlangeSlope ) { copy_self->m_TopFlangeSlope = dynamic_pointer_cast<IfcPlaneAngleMeasure>( m_TopFlangeSlope->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAsymmetricIShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCASYMMETRICISHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_BottomFlangeWidth ) { m_BottomFlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OverallDepth ) { m_OverallDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WebThickness ) { m_WebThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeThickness ) { m_BottomFlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeFilletRadius ) { m_BottomFlangeFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeWidth ) { m_TopFlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeThickness ) { m_TopFlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeFilletRadius ) { m_TopFlangeFilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeEdgeRadius ) { m_BottomFlangeEdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BottomFlangeSlope ) { m_BottomFlangeSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeEdgeRadius ) { m_TopFlangeEdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TopFlangeSlope ) { m_TopFlangeSlope->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcAsymmetricIShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAsymmetricIShapeProfileDef::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 15 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcAsymmetricIShapeProfileDef, expecting 15, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_ProfileType = IfcProfileTypeEnum::createObjectFromSTEP( args[0] );
	m_ProfileName = IfcLabel::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_BottomFlangeWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[3] );
	m_OverallDepth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_WebThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[5] );
	m_BottomFlangeThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[6] );
	m_BottomFlangeFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[7] );
	m_TopFlangeWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[8] );
	m_TopFlangeThickness = IfcPositiveLengthMeasure::createObjectFromSTEP( args[9] );
	m_TopFlangeFilletRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[10] );
	m_BottomFlangeEdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[11] );
	m_BottomFlangeSlope = IfcPlaneAngleMeasure::createObjectFromSTEP( args[12] );
	m_TopFlangeEdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromSTEP( args[13] );
	m_TopFlangeSlope = IfcPlaneAngleMeasure::createObjectFromSTEP( args[14] );
}
void IfcAsymmetricIShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BottomFlangeWidth", m_BottomFlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "OverallDepth", m_OverallDepth ) );
	vec_attributes.push_back( std::make_pair( "WebThickness", m_WebThickness ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeThickness", m_BottomFlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeFilletRadius", m_BottomFlangeFilletRadius ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeWidth", m_TopFlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeThickness", m_TopFlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeFilletRadius", m_TopFlangeFilletRadius ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeEdgeRadius", m_BottomFlangeEdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "BottomFlangeSlope", m_BottomFlangeSlope ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeEdgeRadius", m_TopFlangeEdgeRadius ) );
	vec_attributes.push_back( std::make_pair( "TopFlangeSlope", m_TopFlangeSlope ) );
}
void IfcAsymmetricIShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcParameterizedProfileDef::getAttributesInverse( vec_attributes_inverse );
}
void IfcAsymmetricIShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcAsymmetricIShapeProfileDef::unlinkSelf()
{
	IfcParameterizedProfileDef::unlinkSelf();
}
