/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcOpeningElementTypeEnum = ENUMERATION OF	(OPENING	,RECESS	,USERDEFINED	,NOTDEFINED);
class IFCQUERY_EXPORT IfcOpeningElementTypeEnum : virtual public BuildingObject
{
public:
	enum IfcOpeningElementTypeEnumEnum
	{
		ENUM_OPENING,
		ENUM_RECESS,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcOpeningElementTypeEnum() = default;
	IfcOpeningElementTypeEnum( IfcOpeningElementTypeEnumEnum e ) { m_enum = e; }
	~IfcOpeningElementTypeEnum();
	virtual const char* className() const { return "IfcOpeningElementTypeEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcOpeningElementTypeEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcOpeningElementTypeEnumEnum m_enum;
};

