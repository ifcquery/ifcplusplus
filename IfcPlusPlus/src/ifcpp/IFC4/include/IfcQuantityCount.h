/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcPhysicalSimpleQuantity.h"
class IFCQUERY_EXPORT IfcCountMeasure;
class IFCQUERY_EXPORT IfcLabel;
//ENTITY
class IFCQUERY_EXPORT IfcQuantityCount : public IfcPhysicalSimpleQuantity
{ 
public:
	IfcQuantityCount() = default;
	IfcQuantityCount( int id );
	~IfcQuantityCount();
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
	virtual size_t getNumAttributes() { return 5; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcQuantityCount"; }
	virtual const std::wstring toString() const;


	// IfcPhysicalQuantity -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>										m_Name;
	//  shared_ptr<IfcText>											m_Description;				//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReferences_inverse;
	//  std::vector<weak_ptr<IfcPhysicalComplexQuantity> >			m_PartOfComplex_inverse;

	// IfcPhysicalSimpleQuantity -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcNamedUnit>									m_Unit;						//optional

	// IfcQuantityCount -----------------------------------------------------------
	// attributes:
	shared_ptr<IfcCountMeasure>									m_CountValue;
	shared_ptr<IfcLabel>										m_Formula;					//optional
};

