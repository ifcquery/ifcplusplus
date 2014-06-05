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
#include "include/IfcDoorPanelOperationEnum.h"
#include "include/IfcDoorPanelPositionEnum.h"
#include "include/IfcDoorPanelProperties.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
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

// ENTITY IfcDoorPanelProperties 
IfcDoorPanelProperties::IfcDoorPanelProperties() {}
IfcDoorPanelProperties::IfcDoorPanelProperties( int id ) { m_id = id; }
IfcDoorPanelProperties::~IfcDoorPanelProperties() {}

// method setEntity takes over all attributes from another instance of the class
void IfcDoorPanelProperties::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcDoorPanelProperties> other = dynamic_pointer_cast<IfcDoorPanelProperties>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_PanelDepth = other->m_PanelDepth;
	m_PanelOperation = other->m_PanelOperation;
	m_PanelWidth = other->m_PanelWidth;
	m_PanelPosition = other->m_PanelPosition;
	m_ShapeAspectStyle = other->m_ShapeAspectStyle;
}
void IfcDoorPanelProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCDOORPANELPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelDepth ) { m_PanelDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelOperation ) { m_PanelOperation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelWidth ) { m_PanelWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelPosition ) { m_PanelPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ShapeAspectStyle ) { stream << "#" << m_ShapeAspectStyle->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcDoorPanelProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDoorPanelProperties::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDoorPanelProperties, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>9 ){ std::cout << "Wrong parameter count for entity IfcDoorPanelProperties, expecting 9, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_PanelDepth = IfcPositiveLengthMeasure::createObjectFromStepData( args[4] );
	m_PanelOperation = IfcDoorPanelOperationEnum::createObjectFromStepData( args[5] );
	m_PanelWidth = IfcNormalisedRatioMeasure::createObjectFromStepData( args[6] );
	m_PanelPosition = IfcDoorPanelPositionEnum::createObjectFromStepData( args[7] );
	readEntityReference( args[8], m_ShapeAspectStyle, map );
}
void IfcDoorPanelProperties::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedPropertySet::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PanelDepth", m_PanelDepth ) );
	vec_attributes.push_back( std::make_pair( "PanelOperation", m_PanelOperation ) );
	vec_attributes.push_back( std::make_pair( "PanelWidth", m_PanelWidth ) );
	vec_attributes.push_back( std::make_pair( "PanelPosition", m_PanelPosition ) );
	vec_attributes.push_back( std::make_pair( "ShapeAspectStyle", m_ShapeAspectStyle ) );
}
void IfcDoorPanelProperties::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcDoorPanelProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcDoorPanelProperties::unlinkSelf()
{
	IfcPreDefinedPropertySet::unlinkSelf();
}
