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
shared_ptr<IfcPPObject> IfcDoorPanelProperties::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDoorPanelProperties> copy_self( new IfcDoorPanelProperties() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( CreateCompressedGuidString22() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_PanelDepth ) { copy_self->m_PanelDepth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_PanelDepth->getDeepCopy(options) ); }
	if( m_PanelOperation ) { copy_self->m_PanelOperation = dynamic_pointer_cast<IfcDoorPanelOperationEnum>( m_PanelOperation->getDeepCopy(options) ); }
	if( m_PanelWidth ) { copy_self->m_PanelWidth = dynamic_pointer_cast<IfcNormalisedRatioMeasure>( m_PanelWidth->getDeepCopy(options) ); }
	if( m_PanelPosition ) { copy_self->m_PanelPosition = dynamic_pointer_cast<IfcDoorPanelPositionEnum>( m_PanelPosition->getDeepCopy(options) ); }
	if( m_ShapeAspectStyle ) { copy_self->m_ShapeAspectStyle = dynamic_pointer_cast<IfcShapeAspect>( m_ShapeAspectStyle->getDeepCopy(options) ); }
	return copy_self;
}
void IfcDoorPanelProperties::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCDOORPANELPROPERTIES" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_PanelDepth ) { m_PanelDepth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelOperation ) { m_PanelOperation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelWidth ) { m_PanelWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PanelPosition ) { m_PanelPosition->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ShapeAspectStyle ) { stream << "#" << m_ShapeAspectStyle->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcDoorPanelProperties::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcDoorPanelProperties::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 9 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcDoorPanelProperties, expecting 9, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_PanelDepth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[4] );
	m_PanelOperation = IfcDoorPanelOperationEnum::createObjectFromSTEP( args[5] );
	m_PanelWidth = IfcNormalisedRatioMeasure::createObjectFromSTEP( args[6] );
	m_PanelPosition = IfcDoorPanelPositionEnum::createObjectFromSTEP( args[7] );
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
	IfcPreDefinedPropertySet::getAttributesInverse( vec_attributes_inverse );
}
void IfcDoorPanelProperties::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedPropertySet::setInverseCounterparts( ptr_self_entity );
}
void IfcDoorPanelProperties::unlinkSelf()
{
	IfcPreDefinedPropertySet::unlinkSelf();
}
