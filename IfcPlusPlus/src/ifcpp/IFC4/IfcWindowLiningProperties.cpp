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
shared_ptr<IfcPPObject> IfcWindowLiningProperties::getDeepCopy()
{
	shared_ptr<IfcWindowLiningProperties> copy_self( new IfcWindowLiningProperties() );
	if( m_GlobalId ) { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy() ); }
	if( m_OwnerHistory ) { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy() ); }
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy() ); }
	if( m_LiningDepth ) { copy_self->m_LiningDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LiningDepth->getDeepCopy() ); }
	if( m_LiningThickness ) { copy_self->m_LiningThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_LiningThickness->getDeepCopy() ); }
	if( m_TransomThickness ) { copy_self->m_TransomThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_TransomThickness->getDeepCopy() ); }
	if( m_MullionThickness ) { copy_self->m_MullionThickness = dynamic_pointer_cast<IfcNonNegativeLengthMeasure>( m_MullionThickness->getDeepCopy() ); }
	if( m_FirstTransomOffset ) { copy_self->m_FirstTransomOffset = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_FirstTransomOffset->getDeepCopy() ); }
	if( m_SecondTransomOffset ) { copy_self->m_SecondTransomOffset = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_SecondTransomOffset->getDeepCopy() ); }
	if( m_FirstMullionOffset ) { copy_self->m_FirstMullionOffset = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_FirstMullionOffset->getDeepCopy() ); }
	if( m_SecondMullionOffset ) { copy_self->m_SecondMullionOffset = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_SecondMullionOffset->getDeepCopy() ); }
	if( m_ShapeAspectStyle ) { copy_self->m_ShapeAspectStyle = dynamic_pointer_cast<IfcShapeAspect>( m_ShapeAspectStyle->getDeepCopy() ); }
	if( m_LiningOffset ) { copy_self->m_LiningOffset = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningOffset->getDeepCopy() ); }
	if( m_LiningToPanelOffsetX ) { copy_self->m_LiningToPanelOffsetX = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningToPanelOffsetX->getDeepCopy() ); }
	if( m_LiningToPanelOffsetY ) { copy_self->m_LiningToPanelOffsetY = dynamic_pointer_cast<IfcLengthMeasure>( m_LiningToPanelOffsetY->getDeepCopy() ); }
	return copy_self;
}
void IfcWindowLiningProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCWINDOWLININGPROPERTIES" << "(";
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
void IfcWindowLiningProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	IfcPreDefinedPropertySet::getAttributesInverse( vec_attributes_inverse );
}
void IfcWindowLiningProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcWindowLiningProperties::unlinkSelf()
{
	IfcPreDefinedPropertySet::unlinkSelf();
}
