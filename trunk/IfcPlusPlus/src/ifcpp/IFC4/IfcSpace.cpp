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
#include "include/IfcElementCompositionEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcLengthMeasure.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelCoversSpaces.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelServicesBuildings.h"
#include "include/IfcRelSpaceBoundary.h"
#include "include/IfcSpace.h"
#include "include/IfcSpaceTypeEnum.h"
#include "include/IfcText.h"

// ENTITY IfcSpace 
IfcSpace::IfcSpace() {}
IfcSpace::IfcSpace( int id ) { m_id = id; }
IfcSpace::~IfcSpace() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSpace::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSpace> other = dynamic_pointer_cast<IfcSpace>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_LongName = other->m_LongName;
	m_CompositionType = other->m_CompositionType;
	m_PredefinedType = other->m_PredefinedType;
	m_ElevationWithFlooring = other->m_ElevationWithFlooring;
}
void IfcSpace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSPACE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectType ) { m_ObjectType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ObjectPlacement ) { stream << "#" << m_ObjectPlacement->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Representation ) { stream << "#" << m_Representation->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_LongName ) { m_LongName->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CompositionType ) { m_CompositionType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ElevationWithFlooring ) { m_ElevationWithFlooring->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSpace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSpace::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<11 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSpace, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>11 ){ std::cout << "Wrong parameter count for entity IfcSpace, expecting 11, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_LongName = IfcLabel::createObjectFromStepData( args[7] );
	m_CompositionType = IfcElementCompositionEnum::createObjectFromStepData( args[8] );
	m_PredefinedType = IfcSpaceTypeEnum::createObjectFromStepData( args[9] );
	m_ElevationWithFlooring = IfcLengthMeasure::createObjectFromStepData( args[10] );
}
void IfcSpace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSpatialStructureElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "ElevationWithFlooring", m_ElevationWithFlooring ) );
}
void IfcSpace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> HasCoverings_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_HasCoverings_inverse.size(); ++i ) { HasCoverings_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelCoversSpaces>( m_HasCoverings_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "HasCoverings_inverse", HasCoverings_inverse_vec_obj ) );
	shared_ptr<IfcPPAttributeObjectVector> BoundedBy_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_BoundedBy_inverse.size(); ++i ) { BoundedBy_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelSpaceBoundary>( m_BoundedBy_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "BoundedBy_inverse", BoundedBy_inverse_vec_obj ) );
}
void IfcSpace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSpatialStructureElement::setInverseCounterparts( ptr_self_entity );
}
void IfcSpace::unlinkSelf()
{
	IfcSpatialStructureElement::unlinkSelf();
}
