/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"
#include "IfcDerivedMeasureValue.h"

// TYPE IfcVolumetricFlowRateMeasure = REAL;
class IFCQUERY_EXPORT IfcVolumetricFlowRateMeasure : public IfcDerivedMeasureValue
{
public:
	IfcVolumetricFlowRateMeasure() = default;
	IfcVolumetricFlowRateMeasure( double value );
	~IfcVolumetricFlowRateMeasure();
	virtual const char* className() const { return "IfcVolumetricFlowRateMeasure"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcVolumetricFlowRateMeasure> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	double m_value;
};

