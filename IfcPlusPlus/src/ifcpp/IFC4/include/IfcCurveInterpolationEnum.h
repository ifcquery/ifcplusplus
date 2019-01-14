/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcCurveInterpolationEnum = ENUMERATION OF	(LINEAR	,LOG_LINEAR	,LOG_LOG	,NOTDEFINED);
class IFCQUERY_EXPORT IfcCurveInterpolationEnum : virtual public BuildingObject
{
public:
	enum IfcCurveInterpolationEnumEnum
	{
		ENUM_LINEAR,
		ENUM_LOG_LINEAR,
		ENUM_LOG_LOG,
		ENUM_NOTDEFINED
	};

	IfcCurveInterpolationEnum() = default;
	IfcCurveInterpolationEnum( IfcCurveInterpolationEnumEnum e ) { m_enum = e; }
	~IfcCurveInterpolationEnum();
	virtual const char* className() const { return "IfcCurveInterpolationEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcCurveInterpolationEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcCurveInterpolationEnumEnum m_enum;
};

