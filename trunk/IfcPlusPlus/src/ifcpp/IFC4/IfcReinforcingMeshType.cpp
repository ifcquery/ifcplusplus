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
#include "include/IfcAreaMeasure.h"
#include "include/IfcBendingParameterSelect.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcReinforcingMeshType.h"
#include "include/IfcReinforcingMeshTypeEnum.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcText.h"

// ENTITY IfcReinforcingMeshType 
IfcReinforcingMeshType::IfcReinforcingMeshType() {}
IfcReinforcingMeshType::IfcReinforcingMeshType( int id ) { m_id = id; }
IfcReinforcingMeshType::~IfcReinforcingMeshType() {}

// method setEntity takes over all attributes from another instance of the class
void IfcReinforcingMeshType::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcReinforcingMeshType> other = dynamic_pointer_cast<IfcReinforcingMeshType>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ApplicableOccurrence = other->m_ApplicableOccurrence;
	m_HasPropertySets = other->m_HasPropertySets;
	m_RepresentationMaps = other->m_RepresentationMaps;
	m_Tag = other->m_Tag;
	m_ElementType = other->m_ElementType;
	m_PredefinedType = other->m_PredefinedType;
	m_MeshLength = other->m_MeshLength;
	m_MeshWidth = other->m_MeshWidth;
	m_LongitudinalBarNominalDiameter = other->m_LongitudinalBarNominalDiameter;
	m_TransverseBarNominalDiameter = other->m_TransverseBarNominalDiameter;
	m_LongitudinalBarCrossSectionArea = other->m_LongitudinalBarCrossSectionArea;
	m_TransverseBarCrossSectionArea = other->m_TransverseBarCrossSectionArea;
	m_LongitudinalBarSpacing = other->m_LongitudinalBarSpacing;
	m_TransverseBarSpacing = other->m_TransverseBarSpacing;
	m_BendingShapeCode = other->m_BendingShapeCode;
	m_BendingParameters = other->m_BendingParameters;
}
void IfcReinforcingMeshType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCREINFORCINGMESHTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ",";
	writeEntityList( stream, m_RepresentationMaps );
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_ElementType ) { m_ElementType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshLength ) { m_MeshLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshWidth ) { m_MeshWidth->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarNominalDiameter ) { m_LongitudinalBarNominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarNominalDiameter ) { m_TransverseBarNominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarCrossSectionArea ) { m_LongitudinalBarCrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarCrossSectionArea ) { m_TransverseBarCrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarSpacing ) { m_LongitudinalBarSpacing->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarSpacing ) { m_TransverseBarSpacing->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BendingShapeCode ) { m_BendingShapeCode->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_BendingParameters, true );
	stream << ");";
}
void IfcReinforcingMeshType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReinforcingMeshType::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<20 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcReinforcingMeshType, expecting 20, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>20 ){ std::cout << "Wrong parameter count for entity IfcReinforcingMeshType, expecting 20, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromStepData( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromStepData( args[7] );
	m_ElementType = IfcLabel::createObjectFromStepData( args[8] );
	m_PredefinedType = IfcReinforcingMeshTypeEnum::createObjectFromStepData( args[9] );
	m_MeshLength = IfcPositiveLengthMeasure::createObjectFromStepData( args[10] );
	m_MeshWidth = IfcPositiveLengthMeasure::createObjectFromStepData( args[11] );
	m_LongitudinalBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromStepData( args[12] );
	m_TransverseBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromStepData( args[13] );
	m_LongitudinalBarCrossSectionArea = IfcAreaMeasure::createObjectFromStepData( args[14] );
	m_TransverseBarCrossSectionArea = IfcAreaMeasure::createObjectFromStepData( args[15] );
	m_LongitudinalBarSpacing = IfcPositiveLengthMeasure::createObjectFromStepData( args[16] );
	m_TransverseBarSpacing = IfcPositiveLengthMeasure::createObjectFromStepData( args[17] );
	m_BendingShapeCode = IfcLabel::createObjectFromStepData( args[18] );
	readSelectList( args[19], m_BendingParameters, map );
}
void IfcReinforcingMeshType::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcReinforcingElementType::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "MeshLength", m_MeshLength ) );
	vec_attributes.push_back( std::make_pair( "MeshWidth", m_MeshWidth ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarNominalDiameter", m_LongitudinalBarNominalDiameter ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarNominalDiameter", m_TransverseBarNominalDiameter ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarCrossSectionArea", m_LongitudinalBarCrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarCrossSectionArea", m_TransverseBarCrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "LongitudinalBarSpacing", m_LongitudinalBarSpacing ) );
	vec_attributes.push_back( std::make_pair( "TransverseBarSpacing", m_TransverseBarSpacing ) );
	vec_attributes.push_back( std::make_pair( "BendingShapeCode", m_BendingShapeCode ) );
	shared_ptr<IfcPPAttributeObjectVector> BendingParameters_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_BendingParameters.begin(), m_BendingParameters.end(), std::back_inserter( BendingParameters_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "BendingParameters", BendingParameters_vec_object ) );
}
void IfcReinforcingMeshType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcReinforcingMeshType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcReinforcingElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcReinforcingMeshType::unlinkSelf()
{
	IfcReinforcingElementType::unlinkSelf();
}
