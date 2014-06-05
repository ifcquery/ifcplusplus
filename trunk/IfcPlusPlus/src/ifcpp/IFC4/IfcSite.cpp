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
#include "include/IfcCompoundPlaneAngleMeasure.h"
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
#include "include/IfcSite.h"
#include "include/IfcText.h"

// ENTITY IfcSite 
IfcSite::IfcSite() {}
IfcSite::IfcSite( int id ) { m_id = id; }
IfcSite::~IfcSite() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSite::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSite> other = dynamic_pointer_cast<IfcSite>(other_entity);
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
	m_RefLatitude = other->m_RefLatitude;
	m_RefLongitude = other->m_RefLongitude;
	m_RefElevation = other->m_RefElevation;
	m_LandTitleNumber = other->m_LandTitleNumber;
	m_SiteAddress = other->m_SiteAddress;
}
void IfcSite::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSITE" << "(";
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
	if( m_RefLatitude ) { m_RefLatitude->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RefLongitude ) { m_RefLongitude->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_RefElevation ) { m_RefElevation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LandTitleNumber ) { m_LandTitleNumber->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SiteAddress ) { stream << "#" << m_SiteAddress->getId(); } else { stream << "$"; }
	stream << ");";
}
void IfcSite::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSite::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<14 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSite, expecting 14, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>14 ){ std::cout << "Wrong parameter count for entity IfcSite, expecting 14, having " << num_args << ". Object id: " << getId() << std::endl; }
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
	m_RefLatitude = IfcCompoundPlaneAngleMeasure::createObjectFromStepData( args[9] );
	m_RefLongitude = IfcCompoundPlaneAngleMeasure::createObjectFromStepData( args[10] );
	m_RefElevation = IfcLengthMeasure::createObjectFromStepData( args[11] );
	m_LandTitleNumber = IfcLabel::createObjectFromStepData( args[12] );
	readEntityReference( args[13], m_SiteAddress, map );
}
void IfcSite::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcSpatialStructureElement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RefLatitude", m_RefLatitude ) );
	vec_attributes.push_back( std::make_pair( "RefLongitude", m_RefLongitude ) );
	vec_attributes.push_back( std::make_pair( "RefElevation", m_RefElevation ) );
	vec_attributes.push_back( std::make_pair( "LandTitleNumber", m_LandTitleNumber ) );
	vec_attributes.push_back( std::make_pair( "SiteAddress", m_SiteAddress ) );
}
void IfcSite::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSite::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcSpatialStructureElement::setInverseCounterparts( ptr_self_entity );
}
void IfcSite::unlinkSelf()
{
	IfcSpatialStructureElement::unlinkSelf();
}
