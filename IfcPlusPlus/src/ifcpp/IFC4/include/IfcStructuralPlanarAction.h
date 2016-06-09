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
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcStructuralSurfaceAction.h"
//ENTITY
class IFCPP_EXPORT IfcStructuralPlanarAction : public IfcStructuralSurfaceAction
{ 
public:
	IfcStructuralPlanarAction();
	IfcStructuralPlanarAction( int id );
	~IfcStructuralPlanarAction();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcStructuralPlanarAction"; }


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>								m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>									m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>										m_Name;						//optional
	//  shared_ptr<IfcText>											m_Description;				//optional

	// IfcObjectDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssigns> >						m_HasAssignments_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >							m_Nests_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >							m_IsNestedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDeclares> >						m_HasContext_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >					m_IsDecomposedBy_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >					m_Decomposes_inverse;
	//  std::vector<weak_ptr<IfcRelAssociates> >					m_HasAssociations_inverse;

	// IfcObject -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>										m_ObjectType;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >				m_IsDeclaredBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByObject> >				m_Declares_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByType> >					m_IsTypedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDefinesByProperties> >			m_IsDefinedBy_inverse;

	// IfcProduct -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcObjectPlacement>								m_ObjectPlacement;			//optional
	//  shared_ptr<IfcProductRepresentation>						m_Representation;			//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssignsToProduct> >				m_ReferencedBy_inverse;

	// IfcStructuralActivity -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcStructuralLoad>								m_AppliedLoad;
	//  shared_ptr<IfcGlobalOrLocalEnum>							m_GlobalOrLocal;
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelConnectsStructuralActivity> >	m_AssignedToStructuralItem_inverse;

	// IfcStructuralAction -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcBoolean>										m_DestabilizingLoad;		//optional

	// IfcStructuralSurfaceAction -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcProjectedOrTrueLengthEnum>					m_ProjectedOrTrue;			//optional
	//  shared_ptr<IfcStructuralSurfaceActivityTypeEnum>			m_PredefinedType;

	// IfcStructuralPlanarAction -----------------------------------------------------------
};

