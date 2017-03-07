/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"
#include "IfcElementComponent.h"
class IFCPP_EXPORT IfcLabel;
//ENTITY
class IFCPP_EXPORT IfcReinforcingElement : public IfcElementComponent
{ 
public:
	IfcReinforcingElement();
	IfcReinforcingElement( int id );
	~IfcReinforcingElement();
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self );
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes );
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcReinforcingElement"; }


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

	// IfcElement -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcIdentifier>									m_Tag;						//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelFillsElement> >					m_FillsVoids_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsElements> >				m_ConnectedTo_inverse;
	//  std::vector<weak_ptr<IfcRelInterferesElements> >			m_IsInterferedByElements_inverse;
	//  std::vector<weak_ptr<IfcRelInterferesElements> >			m_InterferesElements_inverse;
	//  std::vector<weak_ptr<IfcRelProjectsElement> >				m_HasProjections_inverse;
	//  std::vector<weak_ptr<IfcRelReferencedInSpatialStructure> >	m_ReferencedInStructures_inverse;
	//  std::vector<weak_ptr<IfcRelVoidsElement> >					m_HasOpenings_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsWithRealizingElements> >	m_IsConnectionRealization_inverse;
	//  std::vector<weak_ptr<IfcRelSpaceBoundary> >					m_ProvidesBoundaries_inverse;
	//  std::vector<weak_ptr<IfcRelConnectsElements> >				m_ConnectedFrom_inverse;
	//  std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >	m_ContainedInStructure_inverse;
	//  std::vector<weak_ptr<IfcRelCoversBldgElements> >			m_HasCoverings_inverse;

	// IfcElementComponent -----------------------------------------------------------

	// IfcReinforcingElement -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>										m_SteelGrade;				//optional
};

