/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcTimeSeries.h"
class IFCQUERY_EXPORT IfcIrregularTimeSeriesValue;
//ENTITY
class IFCQUERY_EXPORT IfcIrregularTimeSeries : public IfcTimeSeries
{ 
public:
	IfcIrregularTimeSeries() = default;
	IfcIrregularTimeSeries( int id );
	~IfcIrregularTimeSeries();
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepLine( std::stringstream& stream ) const;
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual void readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<BuildingEntity> >& map );
	virtual void setInverseCounterparts( shared_ptr<BuildingEntity> ptr_self );
	virtual size_t getNumAttributes() { return 9; }
	virtual void getAttributes( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<BuildingObject> > >& vec_attributes ) const;
	virtual void unlinkFromInverseCounterparts();
	virtual const char* className() const { return "IfcIrregularTimeSeries"; }
	virtual const std::wstring toString() const;


	// IfcTimeSeries -----------------------------------------------------------
	// attributes:
	//  shared_ptr<IfcLabel>										m_Name;
	//  shared_ptr<IfcText>											m_Description;				//optional
	//  shared_ptr<IfcDateTime>										m_StartTime;
	//  shared_ptr<IfcDateTime>										m_EndTime;
	//  shared_ptr<IfcTimeSeriesDataTypeEnum>						m_TimeSeriesDataType;
	//  shared_ptr<IfcDataOriginEnum>								m_DataOrigin;
	//  shared_ptr<IfcLabel>										m_UserDefinedDataOrigin;	//optional
	//  shared_ptr<IfcUnit>											m_Unit;						//optional
	// inverse attributes:
	//  std::vector<weak_ptr<IfcExternalReferenceRelationship> >	m_HasExternalReference_inverse;

	// IfcIrregularTimeSeries -----------------------------------------------------------
	// attributes:
	std::vector<shared_ptr<IfcIrregularTimeSeriesValue> >		m_Values;
};

