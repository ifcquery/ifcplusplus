/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcReinforcingElementType.h"
class IFCQUERY_EXPORT IfcReinforcingMeshTypeEnum;
class IFCQUERY_EXPORT IfcPositiveLengthMeasure;
class IFCQUERY_EXPORT IfcAreaMeasure;
class IFCQUERY_EXPORT IfcLabel;
class IFCQUERY_EXPORT IfcBendingParameterSelect;
//ENTITY
class IFCQUERY_EXPORT IfcReinforcingMeshType : public IfcReinforcingElementType
{ 
public:
	IfcReinforcingMeshType() = default;
	IfcReinforcingMeshType( int id );
	~IfcReinforcingMeshType();
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
	virtual size_t getNumAttributes() { return 20; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcReinforcingMeshType"; }
	virtual const std::wstring toString() const;


	// IfcRoot -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcGloballyUniqueId>						m_GlobalId;
	//  shared_ptr<IfcOwnerHistory>							m_OwnerHistory;				//optional
	//  shared_ptr<IfcLabel>								m_Name;						//optional
	//  shared_ptr<IfcText>									m_Description;				//optional

	// IfcObjectDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssigns> >				m_HasAssignments_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_Nests_inverse;
	//  std::vector<weak_ptr<IfcRelNests> >					m_IsNestedBy_inverse;
	//  std::vector<weak_ptr<IfcRelDeclares> >				m_HasContext_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >			m_IsDecomposedBy_inverse;
	//  std::vector<weak_ptr<IfcRelAggregates> >			m_Decomposes_inverse;
	//  std::vector<weak_ptr<IfcRelAssociates> >			m_HasAssociations_inverse;

	// IfcTypeObject -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcIdentifier>							m_ApplicableOccurrence;		//optional
	//  std::vector<shared_ptr<IfcPropertySetDefinition> >	m_HasPropertySets;			//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelDefinesByType> >			m_Types_inverse;

	// IfcTypeProduct -----------------------------------------------------------
	// attributes:
	//  std::vector<shared_ptr<IfcRepresentationMap> >		m_RepresentationMaps;		//optional
	//  shared_ptr<IfcLabel>								m_Tag;						//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssignsToProduct> >		m_ReferencedBy_inverse;

	// IfcElementType -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>								m_ElementType;				//optional

	// IfcElementComponentType -----------------------------------------------------------

	// IfcReinforcingElementType -----------------------------------------------------------

	// IfcReinforcingMeshType -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcReinforcingMeshTypeEnum>				m_PredefinedType;
	shared_ptr<IfcPositiveLengthMeasure>				m_MeshLength;				//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_MeshWidth;				//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_LongitudinalBarNominalDiameter;	//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_TransverseBarNominalDiameter;	//optional
	shared_ptr<IfcAreaMeasure>							m_LongitudinalBarCrossSectionArea;	//optional
	shared_ptr<IfcAreaMeasure>							m_TransverseBarCrossSectionArea;	//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_LongitudinalBarSpacing;	//optional
	shared_ptr<IfcPositiveLengthMeasure>				m_TransverseBarSpacing;		//optional
	shared_ptr<IfcLabel>								m_BendingShapeCode;			//optional
	std::vector<shared_ptr<IfcBendingParameterSelect> >	m_BendingParameters;		//optional
};

