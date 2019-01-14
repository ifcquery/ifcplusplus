/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcFanTypeEnum = ENUMERATION OF	(CENTRIFUGALFORWARDCURVED	,CENTRIFUGALRADIAL	,CENTRIFUGALBACKWARDINCLINEDCURVED	,CENTRIFUGALAIRFOIL	,TUBEAXIAL	,VANEAXIAL	,PROPELLORAXIAL	,USERDEFINED	,NOTDEFINED);
class IFCQUERY_EXPORT IfcFanTypeEnum : virtual public BuildingObject
{
public:
	enum IfcFanTypeEnumEnum
	{
		ENUM_CENTRIFUGALFORWARDCURVED,
		ENUM_CENTRIFUGALRADIAL,
		ENUM_CENTRIFUGALBACKWARDINCLINEDCURVED,
		ENUM_CENTRIFUGALAIRFOIL,
		ENUM_TUBEAXIAL,
		ENUM_VANEAXIAL,
		ENUM_PROPELLORAXIAL,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFanTypeEnum() = default;
	IfcFanTypeEnum( IfcFanTypeEnumEnum e ) { m_enum = e; }
	~IfcFanTypeEnum();
	virtual const char* className() const { return "IfcFanTypeEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcFanTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcFanTypeEnumEnum m_enum;
};

