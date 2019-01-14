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
class IFCQUERY_EXPORT IfcMaterialDefinitionRepresentation;
class IFCQUERY_EXPORT IfcMaterialRelationship;
//ENTITY
class IFCQUERY_EXPORT IfcMaterial : public IfcMaterialDefinition
{ 
public:
	IfcMaterial() = default;
	IfcMaterial( int id );
	~IfcMaterial();
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
	virtual size_t getNumAttributes() { return 3; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcMaterial"; }
	virtual const std::wstring toString() const;


	// IfcMaterialDefinition -----------------------------------------------------------
	// inverse attributes:
	//  std::vector<weak_ptr<IfcRelAssociatesMaterial> >			m_AssociatedTo_inverse;
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;
	//  std::vector<weak_ptr<IfcMaterialProperties> >				m_HasProperties_inverse;

	// IfcMaterial -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcLabel>										m_Name;
	shared_ptr<IfcText>											m_Description;				//optional
	shared_ptr<IfcLabel>										m_Category;					//optional
	// inverse attributes:
	std::vector<weak_ptr<IfcMaterialDefinitionRepresentation> >	m_HasRepresentation_inverse;
	std::vector<weak_ptr<IfcMaterialRelationship> >				m_IsRelatedWith_inverse;
	std::vector<weak_ptr<IfcMaterialRelationship> >				m_RelatesTo_inverse;
};

