/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcProcedureTypeEnum = ENUMERATION OF	(ADVICE_CAUTION	,ADVICE_NOTE	,ADVICE_WARNING	,CALIBRATION	,DIAGNOSTIC	,SHUTDOWN	,STARTUP	,USERDEFINED	,NOTDEFINED);
class IFCQUERY_EXPORT IfcProcedureTypeEnum : virtual public BuildingObject
{
public:
	enum IfcProcedureTypeEnumEnum
	{
		ENUM_ADVICE_CAUTION,
		ENUM_ADVICE_NOTE,
		ENUM_ADVICE_WARNING,
		ENUM_CALIBRATION,
		ENUM_DIAGNOSTIC,
		ENUM_SHUTDOWN,
		ENUM_STARTUP,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcProcedureTypeEnum() = default;
	IfcProcedureTypeEnum( IfcProcedureTypeEnumEnum e ) { m_enum = e; }
	~IfcProcedureTypeEnum();
	virtual const char* className() const { return "IfcProcedureTypeEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcProcedureTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcProcedureTypeEnumEnum m_enum;
};

