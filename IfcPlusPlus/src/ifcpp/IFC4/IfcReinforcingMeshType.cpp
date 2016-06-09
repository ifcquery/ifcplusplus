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
IfcReinforcingMeshType::IfcReinforcingMeshType() { m_entity_enum = IFCREINFORCINGMESHTYPE; }
IfcReinforcingMeshType::IfcReinforcingMeshType( int id ) { m_id = id; m_entity_enum = IFCREINFORCINGMESHTYPE; }
IfcReinforcingMeshType::~IfcReinforcingMeshType() {}
shared_ptr<IfcPPObject> IfcReinforcingMeshType::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReinforcingMeshType> copy_self( new IfcReinforcingMeshType() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = shared_ptr<IfcGloballyUniqueId>(new IfcGloballyUniqueId( createGUID32_wstr().c_str() ) ); }
		else { copy_self->m_GlobalId = dynamic_pointer_cast<IfcGloballyUniqueId>( m_GlobalId->getDeepCopy(options) ); }
	}
	if( m_OwnerHistory )
	{
		if( options.shallow_copy_IfcOwnerHistory ) { copy_self->m_OwnerHistory = m_OwnerHistory; }
		else { copy_self->m_OwnerHistory = dynamic_pointer_cast<IfcOwnerHistory>( m_OwnerHistory->getDeepCopy(options) ); }
	}
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Description ) { copy_self->m_Description = dynamic_pointer_cast<IfcText>( m_Description->getDeepCopy(options) ); }
	if( m_ApplicableOccurrence ) { copy_self->m_ApplicableOccurrence = dynamic_pointer_cast<IfcIdentifier>( m_ApplicableOccurrence->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_HasPropertySets.size(); ++ii )
	{
		auto item_ii = m_HasPropertySets[ii];
		if( item_ii )
		{
			copy_self->m_HasPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	for( size_t ii=0; ii<m_RepresentationMaps.size(); ++ii )
	{
		auto item_ii = m_RepresentationMaps[ii];
		if( item_ii )
		{
			copy_self->m_RepresentationMaps.push_back( dynamic_pointer_cast<IfcRepresentationMap>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcLabel>( m_Tag->getDeepCopy(options) ); }
	if( m_ElementType ) { copy_self->m_ElementType = dynamic_pointer_cast<IfcLabel>( m_ElementType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcReinforcingMeshTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_MeshLength ) { copy_self->m_MeshLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MeshLength->getDeepCopy(options) ); }
	if( m_MeshWidth ) { copy_self->m_MeshWidth = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_MeshWidth->getDeepCopy(options) ); }
	if( m_LongitudinalBarNominalDiameter ) { copy_self->m_LongitudinalBarNominalDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LongitudinalBarNominalDiameter->getDeepCopy(options) ); }
	if( m_TransverseBarNominalDiameter ) { copy_self->m_TransverseBarNominalDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TransverseBarNominalDiameter->getDeepCopy(options) ); }
	if( m_LongitudinalBarCrossSectionArea ) { copy_self->m_LongitudinalBarCrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_LongitudinalBarCrossSectionArea->getDeepCopy(options) ); }
	if( m_TransverseBarCrossSectionArea ) { copy_self->m_TransverseBarCrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_TransverseBarCrossSectionArea->getDeepCopy(options) ); }
	if( m_LongitudinalBarSpacing ) { copy_self->m_LongitudinalBarSpacing = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_LongitudinalBarSpacing->getDeepCopy(options) ); }
	if( m_TransverseBarSpacing ) { copy_self->m_TransverseBarSpacing = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_TransverseBarSpacing->getDeepCopy(options) ); }
	if( m_BendingShapeCode ) { copy_self->m_BendingShapeCode = dynamic_pointer_cast<IfcLabel>( m_BendingShapeCode->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_BendingParameters.size(); ++ii )
	{
		auto item_ii = m_BendingParameters[ii];
		if( item_ii )
		{
			copy_self->m_BendingParameters.push_back( dynamic_pointer_cast<IfcBendingParameterSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcReinforcingMeshType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCREINFORCINGMESHTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ",";
	writeEntityList( stream, m_RepresentationMaps );
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_ElementType ) { m_ElementType->getStepParameter( stream ); } else { stream << "*"; }
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
	stream << "(";
	for( size_t ii = 0; ii < m_BendingParameters.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcBendingParameterSelect>& type_object = m_BendingParameters[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcReinforcingMeshType::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcReinforcingMeshType::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 20 ){ std::stringstream err; err << "Wrong parameter count for entity IfcReinforcingMeshType, expecting 20, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2] );
	m_Description = IfcText::createObjectFromSTEP( args[3] );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4] );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7] );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8] );
	m_PredefinedType = IfcReinforcingMeshTypeEnum::createObjectFromSTEP( args[9] );
	m_MeshLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[10] );
	m_MeshWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[11] );
	m_LongitudinalBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[12] );
	m_TransverseBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[13] );
	m_LongitudinalBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[14] );
	m_TransverseBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[15] );
	m_LongitudinalBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[16] );
	m_TransverseBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[17] );
	m_BendingShapeCode = IfcLabel::createObjectFromSTEP( args[18] );
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
	if( m_BendingParameters.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> BendingParameters_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_BendingParameters.begin(), m_BendingParameters.end(), std::back_inserter( BendingParameters_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "BendingParameters", BendingParameters_vec_object ) );
	}
}
void IfcReinforcingMeshType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcReinforcingElementType::getAttributesInverse( vec_attributes_inverse );
}
void IfcReinforcingMeshType::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcReinforcingElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcReinforcingMeshType::unlinkFromInverseCounterparts()
{
	IfcReinforcingElementType::unlinkFromInverseCounterparts();
}
