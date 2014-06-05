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
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPObject.h"
#include "IfcDistributionFlowElement.h"
class IfcDistributionChamberElementTypeEnum;
//ENTITY
class IfcDistributionChamberElement : public IfcDistributionFlowElement
{
public:
	IfcDistributionChamberElement();
	IfcDistributionChamberElement( int id );
	~IfcDistributionChamberElement();

	// method setEntity takes over all attributes from another instance of the class
	virtual void setEntity( shared_ptr<IfcPPEntity> other );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkSelf();
	virtual const char* classname() const { return "IfcDistributionChamberElement"; }


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>						m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>							m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>									m_Name;						//optional
	//  shared_ptr<IfcText>									m_Description;				//optional

	// IfcObjectDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssigns> >				m_HasAssignments_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_Nests_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_IsNestedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDeclares> >				m_HasContext_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >				m_IsDecomposedBy_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >				m_Decomposes_inverse;
	//  std::vector<weak_ptr<IfcRelAssociates> >				m_HasAssociations_inverse;

	// IfcObject -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>									m_ObjectType;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >		m_IsDeclaredBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >		m_Declares_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByType> >			m_IsTypedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByProperties> >	m_IsDefinedBy_inverse;

	// IfcProduct -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcObjectPlacement>						m_ObjectPlacement;			//optional
	//  shared_ptr<IfcProductRepresentation>					m_Representation;			//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssignsToProduct> >		m_ReferencedBy_inverse;

	// IfcElement -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcIdentifier>							m_Tag;						//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelFillsElement> >			m_FillsVoids_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsElements> >		m_ConnectedTo_inverse;
	//  std::vector<weak_ptr<IfcRelInterferesElements> >		m_IsInterferedByElements_inverse;
	//  std::vector<weak_ptr<IfcRelInterferesElements> >		m_InterferesElements_inverse;
	//  std::vector<weak_ptr<IfcRelProjectsElement> >		m_HasProjections_inverse;
	//  std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >	m_ReferencedInStructures_inverse;
	//  std::vector<weak_ptr<IfcRelVoidsElement> >			m_HasOpenings_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsWithRealizingElements> >	m_IsConnectionRealization_inverse;
	//  std::vector<weak_ptr<IfcRelSpaceBoundary> >			m_ProvidesBoundaries_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsElements> >		m_ConnectedFrom_inverse;
	//  std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >	m_ContainedInStructure_inverse;

	// IfcDistributionElement -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelConnectsPortToElement> >	m_HasPorts_inverse;

	// IfcDistributionFlowElement -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelFlowControlElements> >	m_HasControlElements_inverse;

	// IfcDistributionChamberElement -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcDistributionChamberElementTypeEnum>	m_PredefinedType;			//optional
};

