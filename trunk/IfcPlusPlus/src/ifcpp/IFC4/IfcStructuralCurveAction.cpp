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
#include "include/IfcGlobalOrLocalEnum.h"
#include "include/IfcGloballyUniqueId.h"
#include "include/IfcLabel.h"
#include "include/IfcObjectPlacement.h"
#include "include/IfcOwnerHistory.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcProjectedOrTrueLengthEnum.h"
#include "include/IfcRelAggregates.h"
#include "include/IfcRelAssigns.h"
#include "include/IfcRelAssignsToProduct.h"
#include "include/IfcRelAssociates.h"
#include "include/IfcRelConnectsStructuralActivity.h"
#include "include/IfcRelDeclares.h"
#include "include/IfcRelDefinesByObject.h"
#include "include/IfcRelDefinesByProperties.h"
#include "include/IfcRelDefinesByType.h"
#include "include/IfcRelNests.h"
#include "include/IfcStructuralCurveAction.h"
#include "include/IfcStructuralCurveActivityTypeEnum.h"
#include "include/IfcStructuralLoad.h"
#include "include/IfcText.h"

// ENTITY IfcStructuralCurveAction 
IfcStructuralCurveAction::IfcStructuralCurveAction() {}
IfcStructuralCurveAction::IfcStructuralCurveAction( int id ) { m_id = id; }
IfcStructuralCurveAction::~IfcStructuralCurveAction() {}

// method setEntity takes over all attributes from another instance of the class
void IfcStructuralCurveAction::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcStructuralCurveAction> other = dynamic_pointer_cast<IfcStructuralCurveAction>(other_entity);
	if( !other) { return; }
	m_GlobalId = other->m_GlobalId;
	m_OwnerHistory = other->m_OwnerHistory;
	m_Name = other->m_Name;
	m_Description = other->m_Description;
	m_ObjectType = other->m_ObjectType;
	m_ObjectPlacement = other->m_ObjectPlacement;
	m_Representation = other->m_Representation;
	m_AppliedLoad = other->m_AppliedLoad;
	m_GlobalOrLocal = other->m_GlobalOrLocal;
	m_DestabilizingLoad = other->m_DestabilizingLoad;
	m_ProjectedOrTrue = other->m_ProjectedOrTrue;
	m_PredefinedType = other->m_PredefinedType;
}
void IfcStructuralCurveAction::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSTRUCTURALCURVEACTION" << "(";
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
	if( m_AppliedLoad ) { stream << "#" << m_AppliedLoad->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_GlobalOrLocal ) { m_GlobalOrLocal->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DestabilizingLoad == false ) { stream << ".F."; }
	else if( m_DestabilizingLoad == true ) { stream << ".T."; }
	stream << ",";
	if( m_ProjectedOrTrue ) { m_ProjectedOrTrue->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_PredefinedType ) { m_PredefinedType->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcStructuralCurveAction::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralCurveAction::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<12 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcStructuralCurveAction, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>12 ){ std::cout << "Wrong parameter count for entity IfcStructuralCurveAction, expecting 12, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_GlobalId = IfcGloballyUniqueId::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_OwnerHistory, map );
	m_Name = IfcLabel::createObjectFromStepData( args[2] );
	m_Description = IfcText::createObjectFromStepData( args[3] );
	m_ObjectType = IfcLabel::createObjectFromStepData( args[4] );
	readEntityReference( args[5], m_ObjectPlacement, map );
	readEntityReference( args[6], m_Representation, map );
	readEntityReference( args[7], m_AppliedLoad, map );
	m_GlobalOrLocal = IfcGlobalOrLocalEnum::createObjectFromStepData( args[8] );
	if( _stricmp( args[9].c_str(), ".F." ) == 0 ) { m_DestabilizingLoad = false; }
	else if( _stricmp( args[9].c_str(), ".T." ) == 0 ) { m_DestabilizingLoad = true; }
	m_ProjectedOrTrue = IfcProjectedOrTrueLengthEnum::createObjectFromStepData( args[10] );
	m_PredefinedType = IfcStructuralCurveActivityTypeEnum::createObjectFromStepData( args[11] );
}
void IfcStructuralCurveAction::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralAction::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "ProjectedOrTrue", m_ProjectedOrTrue ) );
	vec_attributes.push_back( std::make_pair( "PredefinedType", m_PredefinedType ) );
}
void IfcStructuralCurveAction::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcStructuralCurveAction::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralAction::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralCurveAction::unlinkSelf()
{
	IfcStructuralAction::unlinkSelf();
}
