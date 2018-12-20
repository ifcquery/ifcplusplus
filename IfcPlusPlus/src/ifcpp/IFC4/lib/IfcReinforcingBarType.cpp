/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <memory>
#include <sstream>
#include <limits>

#include "ifcpp/model/AttributeObject.h"
#include "ifcpp/model/BuildingException.h"
#include "ifcpp/model/BuildingGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcAreaMeasure.h"
#include "ifcpp/IFC4/include/IfcBendingParameterSelect.h"
#include "ifcpp/IFC4/include/IfcGloballyUniqueId.h"
#include "ifcpp/IFC4/include/IfcIdentifier.h"
#include "ifcpp/IFC4/include/IfcLabel.h"
#include "ifcpp/IFC4/include/IfcOwnerHistory.h"
#include "ifcpp/IFC4/include/IfcPositiveLengthMeasure.h"
#include "ifcpp/IFC4/include/IfcPropertySetDefinition.h"
#include "ifcpp/IFC4/include/IfcReinforcingBarSurfaceEnum.h"
#include "ifcpp/IFC4/include/IfcReinforcingBarType.h"
#include "ifcpp/IFC4/include/IfcReinforcingBarTypeEnum.h"
#include "ifcpp/IFC4/include/IfcRelAggregates.h"
#include "ifcpp/IFC4/include/IfcRelAssigns.h"
#include "ifcpp/IFC4/include/IfcRelAssignsToProduct.h"
#include "ifcpp/IFC4/include/IfcRelAssociates.h"
#include "ifcpp/IFC4/include/IfcRelDeclares.h"
#include "ifcpp/IFC4/include/IfcRelDefinesByType.h"
#include "ifcpp/IFC4/include/IfcRelNests.h"
#include "ifcpp/IFC4/include/IfcRepresentationMap.h"
#include "ifcpp/IFC4/include/IfcText.h"

// ENTITY IfcReinforcingBarType 
IfcReinforcingBarType::IfcReinforcingBarType() = default;
IfcReinforcingBarType::IfcReinforcingBarType( int id ) { m_entity_id = id; }
IfcReinforcingBarType::~IfcReinforcingBarType() = default;
shared_ptr<BuildingObject> IfcReinforcingBarType::getDeepCopy( BuildingCopyOptions& options )
{
	shared_ptr<IfcReinforcingBarType> copy_self( new IfcReinforcingBarType() );
	if( m_GlobalId )
	{
		if( options.create_new_IfcGloballyUniqueId ) { copy_self->m_GlobalId = std::make_shared<IfcGloballyUniqueId>( createBase64Uuid<wchar_t>() ); }
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
	for(auto item_ii : m_HasPropertySets)
	{
			if( item_ii )
		{
			copy_self->m_HasPropertySets.push_back( dynamic_pointer_cast<IfcPropertySetDefinition>(item_ii->getDeepCopy(options) ) );
		}
	}
	for(auto item_ii : m_RepresentationMaps)
	{
			if( item_ii )
		{
			copy_self->m_RepresentationMaps.push_back( dynamic_pointer_cast<IfcRepresentationMap>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_Tag ) { copy_self->m_Tag = dynamic_pointer_cast<IfcLabel>( m_Tag->getDeepCopy(options) ); }
	if( m_ElementType ) { copy_self->m_ElementType = dynamic_pointer_cast<IfcLabel>( m_ElementType->getDeepCopy(options) ); }
	if( m_PredefinedType ) { copy_self->m_PredefinedType = dynamic_pointer_cast<IfcReinforcingBarTypeEnum>( m_PredefinedType->getDeepCopy(options) ); }
	if( m_NominalDiameter ) { copy_self->m_NominalDiameter = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_NominalDiameter->getDeepCopy(options) ); }
	if( m_CrossSectionArea ) { copy_self->m_CrossSectionArea = dynamic_pointer_cast<IfcAreaMeasure>( m_CrossSectionArea->getDeepCopy(options) ); }
	if( m_BarLength ) { copy_self->m_BarLength = dynamic_pointer_cast<IfcPositiveLengthMeasure>( m_BarLength->getDeepCopy(options) ); }
	if( m_BarSurface ) { copy_self->m_BarSurface = dynamic_pointer_cast<IfcReinforcingBarSurfaceEnum>( m_BarSurface->getDeepCopy(options) ); }
	if( m_BendingShapeCode ) { copy_self->m_BendingShapeCode = dynamic_pointer_cast<IfcLabel>( m_BendingShapeCode->getDeepCopy(options) ); }
	for(auto item_ii : m_BendingParameters)
	{
			if( item_ii )
		{
			copy_self->m_BendingParameters.push_back( dynamic_pointer_cast<IfcBendingParameterSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcReinforcingBarType::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_entity_id << "= IFCREINFORCINGBARTYPE" << "(";
	if( m_GlobalId ) { m_GlobalId->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_OwnerHistory ) { stream << "#" << m_OwnerHistory->m_entity_id; } else { stream << "$"; }
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
	if( m_NominalDiameter ) { m_NominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CrossSectionArea ) { m_CrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BarLength ) { m_BarLength->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_BarSurface ) { m_BarSurface->getStepParameter( stream ); } else { stream << "$"; }
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
void IfcReinforcingBarType::getStepParameter( std::stringstream& stream, bool  /*is_select_type*/) const { stream << "#" << m_entity_id; }
const std::wstring IfcReinforcingBarType::toString() const { return L"IfcReinforcingBarType"; }
void IfcReinforcingBarType::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 16 ){ std::stringstream err; err << "Wrong parameter count for entity IfcReinforcingBarType, expecting 16, having " << num_args << ". Entity ID: " << m_entity_id << std::endl; throw BuildingException( err.str() ); }
	m_GlobalId = IfcGloballyUniqueId::createObjectFromSTEP( args[0], map );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromSTEP( args[2], map );
	m_Description = IfcText::createObjectFromSTEP( args[3], map );
	m_ApplicableOccurrence = IfcIdentifier::createObjectFromSTEP( args[4], map );
	readEntityReferenceList( args[5], m_HasPropertySets, map );
	readEntityReferenceList( args[6], m_RepresentationMaps, map );
	m_Tag = IfcLabel::createObjectFromSTEP( args[7], map );
	m_ElementType = IfcLabel::createObjectFromSTEP( args[8], map );
	m_PredefinedType = IfcReinforcingBarTypeEnum::createObjectFromSTEP( args[9], map );
	m_NominalDiameter = IfcPositiveLengthMeasure::createObjectFromSTEP( args[10], map );
	m_CrossSectionArea = IfcAreaMeasure::createObjectFromSTEP( args[11], map );
	m_BarLength = IfcPositiveLengthMeasure::createObjectFromSTEP( args[12], map );
	m_BarSurface = IfcReinforcingBarSurfaceEnum::createObjectFromSTEP( args[13], map );
	m_BendingShapeCode = IfcLabel::createObjectFromSTEP( args[14], map );
	readSelectList( args[15], m_BendingParameters, map );
}
void IfcReinforcingBarType::getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const
{
	IfcReinforcingElementType::getAttributes( vec_attributes );
	vec_attributes.emplace_back( "PredefinedType", m_PredefinedType );
	vec_attributes.emplace_back( "NominalDiameter", m_NominalDiameter );
	vec_attributes.emplace_back( "CrossSectionArea", m_CrossSectionArea );
	vec_attributes.emplace_back( "BarLength", m_BarLength );
	vec_attributes.emplace_back( "BarSurface", m_BarSurface );
	vec_attributes.emplace_back( "BendingShapeCode", m_BendingShapeCode );
	if( !m_BendingParameters.empty() )
	{
		shared_ptr<AttributeObjectVector> BendingParameters_vec_object( new AttributeObjectVector() );
		std::copy( m_BendingParameters.begin(), m_BendingParameters.end(), std::back_inserter( BendingParameters_vec_object->m_vec ) );
		vec_attributes.emplace_back( "BendingParameters", BendingParameters_vec_object );
	}
}
void IfcReinforcingBarType::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes_inverse ) const
{
	IfcReinforcingElementType::getAttributesInverse( vec_attributes_inverse );
}
void IfcReinforcingBarType::setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self_entity )
{
	IfcReinforcingElementType::setInverseCounterparts( ptr_self_entity );
}
void IfcReinforcingBarType::unlinkFromInverseCounterparts()
{
	IfcReinforcingElementType::unlinkFromInverseCounterparts();
}
