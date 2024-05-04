/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcRelConnects.h"
namespace IFC4X3
{
	class IFCQUERY_EXPORT IfcStructuralMember;
	class IFCQUERY_EXPORT IfcStructuralConnection;
	class IFCQUERY_EXPORT IfcBoundaryCondition;
	class IFCQUERY_EXPORT IfcStructuralConnectionCondition;
	class IFCQUERY_EXPORT IfcLengthMeasure;
	class IFCQUERY_EXPORT IfcAxis2Placement3D;
	//ENTITY
	class IFCQUERY_EXPORT IfcRelConnectsStructuralMember : public IfcRelConnects
	{
	public:
		IfcRelConnectsStructuralMember() = default;
		IfcRelConnectsStructuralMember( int id );
		virtual void getStepLine( std::stringstream& stream, size_t precision ) const;
		virtual void getStepParameter( std::stringstream& stream, bool is_select_type, size_t precision ) const;
		virtual void readStepArguments( const std::vector<std::string>& args, const BuildingModelMapType<int,shared_ptr<BuildingEntity> >& map, std::stringstream& errorStream, std::unordered_set<int>& entityIdNotFound );
		virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
		virtual uint8_t getNumAttributes() const { return 10; }
		virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
		virtual void unlinkFromInverseCounterparts();
		virtual uint32_t classID() const { return 1638771189; }

		// IfcRoot -----------------------------------------------------------
		// attributes:
		//  shared_ptr<IfcGloballyUniqueId>					m_GlobalId;
		//  shared_ptr<IfcOwnerHistory>						m_OwnerHistory;				//optional
		//  shared_ptr<IfcLabel>							m_Name;						//optional
		//  shared_ptr<IfcText>								m_Description;				//optional

		// IfcRelationship -----------------------------------------------------------

		// IfcRelConnects -----------------------------------------------------------

		// IfcRelConnectsStructuralMember -----------------------------------------------------------
		// attributes:
		shared_ptr<IfcStructuralMember>					m_RelatingStructuralMember;
		shared_ptr<IfcStructuralConnection>				m_RelatedStructuralConnection;
		shared_ptr<IfcBoundaryCondition>				m_AppliedCondition;			//optional
		shared_ptr<IfcStructuralConnectionCondition>	m_AdditionalConditions;		//optional
		shared_ptr<IfcLengthMeasure>					m_SupportedLength;			//optional
		shared_ptr<IfcAxis2Placement3D>					m_ConditionCoordinateSystem;	//optional
	};
}
