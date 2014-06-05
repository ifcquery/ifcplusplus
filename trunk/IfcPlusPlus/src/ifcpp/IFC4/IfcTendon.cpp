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
#include "include/IfcForceMeasure.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcIdentifier.h"
#include "include/IfcLabel.h"
#include "include/IfcNormalisedRatioMeasure.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcPositiveLengthMeasure.h"
#include "include/IfcPressureMeasure.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsElements.h"
#include "include/IfcRelConnectsWithRealizingElements.h"
#include "include/IfcRelContainedInSpatialStructure.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelFillsElement.h"
#include "include/IfcRelInterferesElements.h"
#include "include/IfcRelNests.h"
#include "include/IfcRelProjectsElement.h"
#include "include/IfcRelReferencedInSpatialStructure.h"
#include "include/IfcRelSpaceBoundary.h"
#include "include/IfcRelVoidsElement.h"
#include "include/IfcTendon.h"
#include "include/IfcTendonTypeEnum.h"
#include "include/IfcText.h"

// ENTITY IfcTendon 
IfcTendon::IfcTendon() {}
IfcTendon::IfcTendon( int id ) { m_id = id; }
IfcTendon::~IfcTendon() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTendon::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTendon> other = dynamic_pointer_cast<IfcTendon>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_Tag = other->m_Tag;
	m_SteelGrade = other->m_SteelGrade;
	m_PredefinedType = other->m_PredefinedType;
	m_NominalDiameter = other->m_NominalDiameter;
	m_CrossSectionArea = other->m_CrossSectionArea;
	m_TensionForce = other->m_TensionForce;
	m_PreStress = other->m_PreStress;
	m_FrictionCoefficient = other->m_FrictionCoefficient;
	m_AnchorageSlip = other->m_AnchorageSlip;
	m_MinCurvatureRadius = other->m_MinCurvatureRadius;
}
void IfcTendon::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTENDON" << "(";
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
	if( m_Tag ) { m_Tag->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SteelGrade ) { m_SteelGrade->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_NominalDiameter ) { m_NominalDiameter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_CrossSectionArea ) { m_CrossSectionArea->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TensionForce ) { m_TensionForce->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PreStress ) { m_PreStress->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FrictionCoefficient ) { m_FrictionCoefficient->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_AnchorageSlip ) { m_AnchorageSlip->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_MinCurvatureRadius ) { m_MinCurvatureRadius->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTendon::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTendon::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<17 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTendon, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>17 ){ std::cout << "Wrong parameter count for entity IfcTendon, expecting 17, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	m_Tag = IfcIdentifier::createObjectFromStepData( args[7] );
	m_SteelGrade = IfcLabel::createObjectFromStepData( args[8] );
	m_PredefinedType = IfcTendonTypeEnum::createObjectFromStepData( args[9] );
	m_NominalDiameter = IfcPositiveLengthMeasure::createObjectFromStepData( args[10] );
	m_CrossSectionArea = IfcAreaMeasure::createObjectFromStepData( args[11] );
	m_TensionForce = IfcForceMeasure::createObjectFromStepData( args[12] );
	m_PreStress = IfcPressureMeasure::createObjectFromStepData( args[13] );
	m_FrictionCoefficient = IfcNormalisedRatioMeasure::createObjectFromStepData( args[14] );
	m_AnchorageSlip = IfcPositiveLengthMeasure::createObjectFromStepData( args[15] );
	m_MinCurvatureRadius = IfcPositiveLengthMeasure::createObjectFromStepData( args[16] );
}
void IfcTendon::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcReinforcingElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
	vec_attributes.push_back( std::make_pair( "NominalDiameter", m_NominalDiameter ) );
	vec_attributes.push_back( std::make_pair( "CrossSectionArea", m_CrossSectionArea ) );
	vec_attributes.push_back( std::make_pair( "TensionForce", m_TensionForce ) );
	vec_attributes.push_back( std::make_pair( "PreStress", m_PreStress ) );
	vec_attributes.push_back( std::make_pair( "FrictionCoefficient", m_FrictionCoefficient ) );
	vec_attributes.push_back( std::make_pair( "AnchorageSlip", m_AnchorageSlip ) );
	vec_attributes.push_back( std::make_pair( "MinCurvatureRadius", m_MinCurvatureRadius ) );
}
void IfcTendon::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTendon::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcReinforcingElement::setInverseCounterparts( ptr_self_entity );
}
void IfcTendon::unlinkSelf()
{
	IfcReinforcingElement::unlinkSelf();
}
