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
#include "include/IfcBuilding.h"
#include "include/IfcElementCompositionEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPostalAddress.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcText.h"

// ENTITY IfcBuilding 
IfcBuilding::IfcBuilding() {}
IfcBuilding::IfcBuilding( int id ) { m_id = id; }
IfcBuilding::~IfcBuilding() {}
shared_ptr<IfcPPObject> IfcBuilding::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBuilding> copy_self( new IfcBuilding() );
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
	if( m_ObjectType ) { copy_self->m_ObjectType = dynamic_pointer_cast<IfcLabel>( m_ObjectType->getDeepCopy(options) ); }
	if( m_ObjectPlacement ) { copy_self->m_ObjectPlacement = dynamic_pointer_cast<IfcObjectPlacement>( m_ObjectPlacement->getDeepCopy(options) ); }
	if( m_Representation ) { copy_self->m_Representation = dynamic_pointer_cast<IfcProductRepresentation>( m_Representation->getDeepCopy(options) ); }
	if( m_LongName ) { copy_self->m_LongName = dynamic_pointer_cast<IfcLabel>( m_LongName->getDeepCopy(options) ); }
	if( m_CompositionType ) { copy_self->m_CompositionType = dynamic_pointer_cast<IfcElementCompositionEnum>( m_CompositionType->getDeepCopy(options) ); }
	if( m_ElevationOfRefHeight ) { copy_self->m_ElevationOfRefHeight = dynamic_pointer_cast<IfcLengthMeasure>( m_ElevationOfRefHeight->getDeepCopy(options) ); }
	if( m_ElevationOfTerrain ) { copy_self->m_ElevationOfTerrain = dynamic_pointer_cast<IfcLengthMeasure>( m_ElevationOfTerrain->getDeepCopy(options) ); }
	if( m_BuildingAddress ) { copy_self->m_BuildingAddress = dynamic_pointer_cast<IfcPostalAddress>( m_BuildingAddress->getDeepCopy(options) ); }
	return copy_self;
}
void IfcBuilding::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBUILDING" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_LongName ) { m_LongName->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_CompositionType ) { m_CompositionType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ElevationOfRefHeight ) { m_ElevationOfRefHeight->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ElevationOfTerrain ) { m_ElevationOfTerrain->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BuildingAddress ) { stream << "#" << m_BuildingAddress->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcBuilding::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBuilding::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 12 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcBuilding, expecting 12, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ObjectType = IfcLabel::createObjectFromSTEP( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_LongName = IfcLabel::createObjectFromSTEP( args[7] );
	m_CompositionType = IfcElementCompositionEnum::createObjectFromSTEP( args[8] );
	m_ElevationOfRefHeight = IfcLengthMeasure::createObjectFromSTEP( args[9] );
	m_ElevationOfTerrain = IfcLengthMeasure::createObjectFromSTEP( args[10] );
	readEntityReference( args[11], m_BuildingAddress, map );
}
void IfcBuilding::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSpatialStructureElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ElevationOfRefHeight", m_ElevationOfRefHeight ) );
	vec_attributes.push_back( std::make_pair( "ElevationOfTerrain", m_ElevationOfTerrain ) );
	vec_attributes.push_back( std::make_pair( "BuildingAddress", m_BuildingAddress ) );
}
void IfcBuilding::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcSpatialStructureElement::getAttributesInverse( vec_attributes_inverse );
}
void IfcBuilding::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSpatialStructureElement::setInverseCounterparts( ptr_self_entity );
}
void IfcBuilding::unlinkSelf()
{
	IfcSpatialStructureElement::unlinkSelf();
}
