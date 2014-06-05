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
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"
#include "include/IfcWindowLiningProperties.h"

// ENTITY IfcWindowLiningProperties 
IfcWindowLiningProperties::IfcWindowLiningProperties() {}
IfcWindowLiningProperties::IfcWindowLiningProperties( int id ) { m_id = id; }
IfcWindowLiningProperties::~IfcWindowLiningProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcWindowLiningProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcWindowLiningProperties> other = dynamic_pointer_cast<IfcWindowLiningProperties>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_LiningDepth = other->m_LiningDepth;
	m_LiningThickness = other->m_LiningThickness;
	m_TransomThickness = other->m_TransomThickness;
	m_MullionThickness = other->m_MullionThickness;
	m_FirstTransomOffset = other->m_FirstTransomOffset;
	m_SecondTransomOffset = other->m_SecondTransomOffset;
	m_FirstMullionOffset = other->m_FirstMullionOffset;
	m_SecondMullionOffset = other->m_SecondMullionOffset;
	m_ShapeAspectStyle = other->m_ShapeAspectStyle;
	m_LiningOffset = other->m_LiningOffset;
	m_LiningToPanelOffsetX = other->m_LiningToPanelOffsetX;
	m_LiningToPanelOffsetY = other->m_LiningToPanelOffsetY;
}
void IfcWindowLiningProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCWINDOWLININGPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningDepth ) { m_LiningDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningThickness ) { m_LiningThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransomThickness ) { m_TransomThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MullionThickness ) { m_MullionThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FirstTransomOffset ) { m_FirstTransomOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondTransomOffset ) { m_SecondTransomOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FirstMullionOffset ) { m_FirstMullionOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SecondMullionOffset ) { m_SecondMullionOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ShapeAspectStyle ) { stream << "#" << m_ShapeAspectStyle->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_LiningOffset ) { m_LiningOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningToPanelOffsetX ) { m_LiningToPanelOffsetX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningToPanelOffsetY ) { m_LiningToPanelOffsetY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcWindowLiningProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcWindowLiningProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<16 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcWindowLiningProperties, expecting 16, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>16 ){ std::cout << "Wrong parameter count for entity IfcWindowLiningProperties, expecting 16, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_LiningDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_LiningThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[5] );
	m_TransomThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[6] );
	m_MullionThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[7] );
	m_FirstTransomOffset = IfcNormalisedRatioMeasure::createObjectFromStepData( args[8] );
	m_SecondTransomOffset = IfcNormalisedRatioMeasure::createObjectFromStepData( args[9] );
	m_FirstMullionOffset = IfcNormalisedRatioMeasure::createObjectFromStepData( args[10] );
	m_SecondMullionOffset = IfcNormalisedRatioMeasure::createObjectFromStepData( args[11] );
	readEntityReference( args[12], m_ShapeAspectStyle, map );
	m_LiningOffset = IfcLengthMeasure::createObjectFromStepData( args[13] );
	m_LiningToPanelOffsetX = IfcLengthMeasure::createObjectFromStepData( args[14] );
	m_LiningToPanelOffsetY = IfcLengthMeasure::createObjectFromStepData( args[15] );
}
void IfcWindowLiningProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedPropertySet::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LiningDepth", m_LiningDepth ) );
	vec_attributes.push_back( std::make_pair( "LiningThickness", m_LiningThickness ) );
	vec_attributes.push_back( std::make_pair( "TransomThickness", m_TransomThickness ) );
	vec_attributes.push_back( std::make_pair( "MullionThickness", m_MullionThickness ) );
	vec_attributes.push_back( std::make_pair( "FirstTransomOffset", m_FirstTransomOffset ) );
	vec_attributes.push_back( std::make_pair( "SecondTransomOffset", m_SecondTransomOffset ) );
	vec_attributes.push_back( std::make_pair( "FirstMullionOffset", m_FirstMullionOffset ) );
	vec_attributes.push_back( std::make_pair( "SecondMullionOffset", m_SecondMullionOffset ) );
	vec_attributes.push_back( std::make_pair( "ShapeAspectStyle", m_ShapeAspectStyle ) );
	vec_attributes.push_back( std::make_pair( "LiningOffset", m_LiningOffset ) );
	vec_attributes.push_back( std::make_pair( "LiningToPanelOffsetX", m_LiningToPanelOffsetX ) );
	vec_attributes.push_back( std::make_pair( "LiningToPanelOffsetY", m_LiningToPanelOffsetY ) );
}
void IfcWindowLiningProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcWindowLiningProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcWindowLiningProperties::unlinkSelf()
{
	IfcPreDefinedPropertySet::unlinkSelf();
}
