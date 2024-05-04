/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4X3/include/IfcAreaMeasure.h"
#include "ifcpp/IFC4X3/include/IfcBendingParameterSelect.h"
#include "ifcpp/IFC4X3/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4X3/include/IfcIdentifier.h"
#include "ifcpp/IFC4X3/include/IfcLabel.h"
#include "ifcpp/IFC4X3/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4X3/include/IfcPositiveLengthMeasure.h"
#include "ifcpp/IFC4X3/include/IfcPropertySetDefinition.h"
#include "ifcpp/IFC4X3/include/IfcReinforcingMeshType.h"
#include "ifcpp/IFC4X3/include/IfcReinforcingMeshTypeEnum.h"
#include "ifcpp/IFC4X3/include/IfcRelAggregates.h"
#include "ifcpp/IFC4X3/include/IfcRelAssigns.h"
#include "ifcpp/IFC4X3/include/IfcRelAssignsToProduct.h"
#include "ifcpp/IFC4X3/include/IfcRelAssociates.h"
#include "ifcpp/IFC4X3/include/IfcRelDeclares.h"
#include "ifcpp/IFC4X3/include/IfcRelDefinesByType.h"
#include "ifcpp/IFC4X3/include/IfcRelNests.h"
#include "ifcpp/IFC4X3/include/IfcRepresentationMap.h"
#include "ifcpp/IFC4X3/include/IfcText.h"

// ENTITY IfcReinforcingMeshType 
IFC4X3::IfcReinforcingMeshType::IfcReinforcingMeshType( int tag ) { m_tag = tag; }
void IFC4X3::IfcReinforcingMeshType::getStepLine( std::stringstream& stream, size_t precision ) const
{
	stream << "#" << m_tag << "=IFCREINFORCINGMESHTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_tag; } else { stream << "$"; }
	stream << ",";
	if( m_Name ) { m_Name->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_Description ) { m_Description->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_ApplicableOccurrence ) { m_ApplicableOccurrence->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_HasPropertySets );
	stream << ",";
	writeEntityList( stream, m_RepresentationMaps );
	stream << ",";
	if( m_Tag ) { m_Tag->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_ElementType ) { m_ElementType->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshLength ) { m_MeshLength->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_MeshWidth ) { m_MeshWidth->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarNominalDiameter ) { m_LongitudinalBarNominalDiameter->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarNominalDiameter ) { m_TransverseBarNominalDiameter->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarCrossSectionArea ) { m_LongitudinalBarCrossSectionArea->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarCrossSectionArea ) { m_TransverseBarCrossSectionArea->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_LongitudinalBarSpacing ) { m_LongitudinalBarSpacing->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_TransverseBarSpacing ) { m_TransverseBarSpacing->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_BendingShapeCode ) { m_BendingShapeCode->getStepParameter( stream, false, precision ); } else { stream << "$"; }
	stream << ",";
	if( m_BendingParameters.size() > 0 )
	{
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
				type_object->getStepParameter( stream, true, precision );
			}
			else
			{
				stream << "$";
			}
		}
		stream << ")";
	}
	else { stream << "$"; }
	stream << ");";
}
void IFC4X3::IfcReinforcingMeshType::getStepParameter( std::stringstream& stream, bool /*is_select_type*/, size_t /*precision*/ ) const { stream << "#" << m_tag; }
void IFC4X3::IfcReinforcingMeshType::readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound )
{
	const size_t num_args = args.size();
	if( num_args > 0 ){m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map, errorStream, entityIdNotFound );}
	if( num_args > 1 ){readEntityReference( args[1], m_OwnerHistory, map, errorStream, entityIdNotFound );}
	if( num_args > 2 ){m_Name = IfcLabel::createObjectFromSTEP( args[2], map, errorStream, entityIdNotFound );}
	if( num_args > 3 ){m_Description = IfcText::createObjectFromSTEP( args[3], map, errorStream, entityIdNotFound );}
	if( num_args > 4 ){m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4], map, errorStream, entityIdNotFound );}
	if( num_args > 5 ){readEntityReferenceList( args[5], m_HasPropertySets, map, errorStream, entityIdNotFound );}
	if( num_args > 6 ){readEntityReferenceList( args[6], m_RepresentationMaps, map, errorStream, entityIdNotFound );}
	if( num_args > 7 ){m_Tag = IfcLabel::createObjectFromSTEP( args[7], map, errorStream, entityIdNotFound );}
	if( num_args > 8 ){m_ElementType = IfcLabel::createObjectFromSTEP( args[8], map, errorStream, entityIdNotFound );}
	if( num_args > 9 ){m_PredefinedType = IfcReinforcingMeshTypeEnum::createObjectFromSTEP( args[9], map, errorStream, entityIdNotFound );}
	if( num_args > 10 ){m_MeshLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[10], map, errorStream, entityIdNotFound );}
	if( num_args > 11 ){m_MeshWidth = IfcPositiveLengthMeasure::createObjectFromSTEP( args[11], map, errorStream, entityIdNotFound );}
	if( num_args > 12 ){m_LongitudinalBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[12], map, errorStream, entityIdNotFound );}
	if( num_args > 13 ){m_TransverseBarNominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[13], map, errorStream, entityIdNotFound );}
	if( num_args > 14 ){m_LongitudinalBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[14], map, errorStream, entityIdNotFound );}
	if( num_args > 15 ){m_TransverseBarCrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[15], map, errorStream, entityIdNotFound );}
	if( num_args > 16 ){m_LongitudinalBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[16], map, errorStream, entityIdNotFound );}
	if( num_args > 17 ){m_TransverseBarSpacing = IfcPositiveLengthMeasure::createObjectFromSTEP( args[17], map, errorStream, entityIdNotFound );}
	if( num_args > 18 ){m_BendingShapeCode = IfcLabel::createObjectFromSTEP( args[18], map, errorStream, entityIdNotFound );}
	if( num_args > 19 ){readSelectList( args[19], m_BendingParameters, map, errorStream, entityIdNotFound );}
	if( num_args != 20 ){ errorStream << "Wrong parameter count for entity IfcReinforcingMeshType, expecting 20, having " << num_args << ". Entity ID: " << m_tag << std::endl; }
}
void IFC4X3::IfcReinforcingMeshType::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IFC4X3::IfcReinforcingElementType::getAttributes( vec_attributes );
	vec_attributes.emplace_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.emplace_back( std::make_pair( "MeshLength", m_MeshLength ) );
	vec_attributes.emplace_back( std::make_pair( "MeshWidth", m_MeshWidth ) );
	vec_attributes.emplace_back( std::make_pair( "LongitudinalBarNominalDiameter", m_LongitudinalBarNominalDiameter ) );
	vec_attributes.emplace_back( std::make_pair( "TransverseBarNominalDiameter", m_TransverseBarNominalDiameter ) );
	vec_attributes.emplace_back( std::make_pair( "LongitudinalBarCrossSectionArea", m_LongitudinalBarCrossSectionArea ) );
	vec_attributes.emplace_back( std::make_pair( "TransverseBarCrossSectionArea", m_TransverseBarCrossSectionArea ) );
	vec_attributes.emplace_back( std::make_pair( "LongitudinalBarSpacing", m_LongitudinalBarSpacing ) );
	vec_attributes.emplace_back( std::make_pair( "TransverseBarSpacing", m_TransverseBarSpacing ) );
	vec_attributes.emplace_back( std::make_pair( "BendingShapeCode", m_BendingShapeCode ) );
	shared_ptr<AttributeObjectVector> BendingParameters_vec_object( new AttributeObjectVector() );
	std::copy( m_BendingParameters.begin(), m_BendingParameters.end(), std::back_inserter( BendingParameters_vec_object->m_vec ) );
	vec_attributes.emplace_back( std::make_pair( "BendingParameters", BendingParameters_vec_object ) );
}
void IFC4X3::IfcReinforcingMeshType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IFC4X3::IfcReinforcingElementType::getAttributesInverse( vec_attributes_inverse );
}
void IFC4X3::IfcReinforcingMeshType::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcReinforcingElementType::setInverseCounterparts( ptr_self_entity );
}
void IFC4X3::IfcReinforcingMeshType::unlinkFromInverseCounterparts()
{
	IfcReinforcingElementType::unlinkFromInverseCounterparts();
}
