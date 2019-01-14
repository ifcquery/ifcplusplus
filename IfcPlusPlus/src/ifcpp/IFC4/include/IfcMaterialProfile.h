/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcMaterialDefinition.h"
class IFCQUERY_EXPORT IfcLabel;
class IFCQUERY_EXPORT IfcText;
class IFCQUERY_EXPORT IfcMaterial;
class IFCQUERY_EXPORT IfcProfileDef;
class IFCQUERY_EXPORT IfcInteger;
class IFCQUERY_EXPORT IfcMaterialProfileSet;
//ENTITY
class IFCQUERY_EXPORT IfcMaterialProfile : public IfcMaterialDefinition
{ 
public:
	IfcMaterialProfile() = default;
	IfcMaterialProfile( int id );
	~IfcMaterialProfile();
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
	virtual size_t getNumAttributes() { return 6; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcMaterialProfile"; }
	virtual const std::wstring toString() const;


	// IfcMaterialDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssociatesMaterial> >			m_AssociatedTo_inverse;
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;
	//  std::vector<weak_ptr<IfcMaterialProperties> >				m_HasProperties_inverse;

	// IfcMaterialProfile -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>										m_Name;						//optional
	shared_ptr<IfcText>											m_Description;				//optional
	shared_ptr<IfcMaterial>										m_Material;					//optional
	shared_ptr<IfcProfileDef>									m_Profile;
	shared_ptr<IfcInteger>										m_Priority;					//optional
	shared_ptr<IfcLabel>										m_Category;					//optional
	// inverse attributes:
	weak_ptr<IfcMaterialProfileSet>								m_ToMaterialProfileSet_inverse;
};

