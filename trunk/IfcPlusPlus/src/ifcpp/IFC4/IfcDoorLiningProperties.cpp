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
#include "include/IfcDoorLiningProperties.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcNonNegativeLengthMeasure.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByTemplate.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcText.h"
#include "include/IfcTypeObject.h"

// ENTITY IfcDoorLiningProperties 
IfcDoorLiningProperties::IfcDoorLiningProperties() {}
IfcDoorLiningProperties::IfcDoorLiningProperties( int id ) { m_id = id; }
IfcDoorLiningProperties::~IfcDoorLiningProperties() {}
shared_ptr<IfcPPObject> IfcDoorLiningProperties::getDeepCopy()
{
	shared_ptr<IfcDoorLiningProperties> copy_self( new IfcDoorLiningProperties() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_LiningDepth ) { copy_self->m_LiningDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LiningDepth->getDeepCopy() ); }
	if( m_LiningThickness ) { copy_self->m_LiningThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_LiningThickness->getDeepCopy() ); }
	if( m_ThresholdDepth ) { copy_self->m_ThresholdDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_ThresholdDepth->getDeepCopy() ); }
	if( m_ThresholdThickness ) { copy_self->m_ThresholdThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_ThresholdThickness->getDeepCopy() ); }
	if( m_TransomThickness ) { copy_self->m_TransomThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_TransomThickness->getDeepCopy() ); }
	if( m_TransomOffset ) { copy_self->m_TransomOffset = dynamic_pointer_cast<IfcLengthMeasure>( m_TransomOffset->getDeepCopy() ); }
	if( m_LiningOffset ) { copy_self->m_LiningOffset = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningOffset->getDeepCopy() ); }
	if( m_ThresholdOffset ) { copy_self->m_ThresholdOffset = dynamic_pointer_cast<IfcLengthMeasure>( m_ThresholdOffset->getDeepCopy() ); }
	if( m_CasingThickness ) { copy_self->m_CasingThickness = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_CasingThickness->getDeepCopy() ); }
	if( m_CasingDepth ) { copy_self->m_CasingDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_CasingDepth->getDeepCopy() ); }
	if( m_ShapeAspectStyle ) { copy_self->m_ShapeAspectStyle = dynamic_pointer_cast<IfcShapeAspect>( m_ShapeAspectStyle->getDeepCopy() ); }
	if( m_LiningToPanelOffsetX ) { copy_self->m_LiningToPanelOffsetX = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningToPanelOffsetX->getDeepCopy() ); }
	if( m_LiningToPanelOffsetY ) { copy_self->m_LiningToPanelOffsetY = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningToPanelOffsetY->getDeepCopy() ); }
	return copy_self;
}
void IfcDoorLiningProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDOORLININGPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_LiningDepth ) { m_LiningDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningThickness ) { m_LiningThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ThresholdDepth ) { m_ThresholdDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ThresholdThickness ) { m_ThresholdThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransomThickness ) { m_TransomThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransomOffset ) { m_TransomOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningOffset ) { m_LiningOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ThresholdOffset ) { m_ThresholdOffset->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CasingThickness ) { m_CasingThickness->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CasingDepth ) { m_CasingDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ShapeAspectStyle ) { stream << "#" << m_ShapeAspectStyle->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_LiningToPanelOffsetX ) { m_LiningToPanelOffsetX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LiningToPanelOffsetY ) { m_LiningToPanelOffsetY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcDoorLiningProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDoorLiningProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<17 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDoorLiningProperties, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>17 ){ std::cout << "Wrong parameter count for entity IfcDoorLiningProperties, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_LiningDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_LiningThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[5] );
	m_ThresholdDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[6] );
	m_ThresholdThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[7] );
	m_TransomThickness = IfcNonNegativeLengthMeasure::createObjectFromStepData( args[8] );
	m_TransomOffset = IfcLengthMeasure::createObjectFromStepData( args[9] );
	m_LiningOffset = IfcLengthMeasure::createObjectFromStepData( args[10] );
	m_ThresholdOffset = IfcLengthMeasure::createObjectFromStepData( args[11] );
	m_CasingThickness = IfcPositiveLengthMeasure::createObjectFromStepData( args[12] );
	m_CasingDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[13] );
	readEntityReference( args[14], m_ShapeAspectStyle, map );
	m_LiningToPanelOffsetX = IfcLengthMeasure::createObjectFromStepData( args[15] );
	m_LiningToPanelOffsetY = IfcLengthMeasure::createObjectFromStepData( args[16] );
}
void IfcDoorLiningProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedPropertySet::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LiningDepth", m_LiningDepth ) );
	vec_attributes.push_back( std::make_pair( "LiningThickness", m_LiningThickness ) );
	vec_attributes.push_back( std::make_pair( "ThresholdDepth", m_ThresholdDepth ) );
	vec_attributes.push_back( std::make_pair( "ThresholdThickness", m_ThresholdThickness ) );
	vec_attributes.push_back( std::make_pair( "TransomThickness", m_TransomThickness ) );
	vec_attributes.push_back( std::make_pair( "TransomOffset", m_TransomOffset ) );
	vec_attributes.push_back( std::make_pair( "LiningOffset", m_LiningOffset ) );
	vec_attributes.push_back( std::make_pair( "ThresholdOffset", m_ThresholdOffset ) );
	vec_attributes.push_back( std::make_pair( "CasingThickness", m_CasingThickness ) );
	vec_attributes.push_back( std::make_pair( "CasingDepth", m_CasingDepth ) );
	vec_attributes.push_back( std::make_pair( "ShapeAspectStyle", m_ShapeAspectStyle ) );
	vec_attributes.push_back( std::make_pair( "LiningToPanelOffsetX", m_LiningToPanelOffsetX ) );
	vec_attributes.push_back( std::make_pair( "LiningToPanelOffsetY", m_LiningToPanelOffsetY ) );
}
void IfcDoorLiningProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedPropertySet::getAttributesInverse( vec_attributes_inverse );
}
void IfcDoorLiningProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcDoorLiningProperties::unlinkSelf()
{
	IfcPreDefinedPropertySet::unlinkSelf();
}
