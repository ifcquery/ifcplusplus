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
#include "include/IfcAxis2Placement2D.h"
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcLabel.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcProfileProperties.h"
#include "include/IfcProfileTypeEnum.h"
#include "include/IfcZShapeProfileDef.h"

// ENTITY IfcZShapeProfileDef 
IfcZShapeProfileDef::IfcZShapeProfileDef() {}
IfcZShapeProfileDef::IfcZShapeProfileDef( int id ) { m_id = id; }
IfcZShapeProfileDef::~IfcZShapeProfileDef() {}

// method setEntity takes over all attributes from another instance of the class
void IfcZShapeProfileDef::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcZShapeProfileDef> other = dynamic_pointer_cast<IfcZShapeProfileDef>(other_entity);
	if( !other) { return; }
	m_ProfileType = other->m_ProfileType;
	m_ProfileName = other->m_ProfileName;
	m_Position = other->m_Position;
	m_Depth = other->m_Depth;
	m_FlangeWidth = other->m_FlangeWidth;
	m_WebThickness = other->m_WebThickness;
	m_FlangeThickness = other->m_FlangeThickness;
	m_FilletRadius = other->m_FilletRadius;
	m_EdgeRadius = other->m_EdgeRadius;
}
void IfcZShapeProfileDef::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCZSHAPEPROFILEDEF" << "(";
	if( m_ProfileType ) { m_ProfileType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ProfileName ) { m_ProfileName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Depth ) { m_Depth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FlangeWidth ) { m_FlangeWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_WebThickness ) { m_WebThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FlangeThickness ) { m_FlangeThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FilletRadius ) { m_FilletRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_EdgeRadius ) { m_EdgeRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcZShapeProfileDef::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcZShapeProfileDef::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcZShapeProfileDef, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcZShapeProfileDef, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_ProfileType = IfcProfileTypeEnum::createObjectFromStepData( args[0] );
	m_ProfileName = IfcLabel::createObjectFromStepData( args[1] );
	readEntityReference( args[2], m_Position, map );
	m_Depth = IfcPositiveLengthMeasure::createObjectFromStepData( args[3] );
	m_FlangeWidth = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_WebThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[5] );
	m_FlangeThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[6] );
	m_FilletRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[7] );
	m_EdgeRadius = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[8] );
}
void IfcZShapeProfileDef::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcParameterizedProfileDef::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Depth", m_Depth ) );
	vec_attributes.push_back( std::make_pair( "FlangeWidth", m_FlangeWidth ) );
	vec_attributes.push_back( std::make_pair( "WebThickness", m_WebThickness ) );
	vec_attributes.push_back( std::make_pair( "FlangeThickness", m_FlangeThickness ) );
	vec_attributes.push_back( std::make_pair( "FilletRadius", m_FilletRadius ) );
	vec_attributes.push_back( std::make_pair( "EdgeRadius", m_EdgeRadius ) );
}
void IfcZShapeProfileDef::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcZShapeProfileDef::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcParameterizedProfileDef::setInverseCounterparts( ptr_self_entity );
}
void IfcZShapeProfileDef::unlinkSelf()
{
	IfcParameterizedProfileDef::unlinkSelf();
}
