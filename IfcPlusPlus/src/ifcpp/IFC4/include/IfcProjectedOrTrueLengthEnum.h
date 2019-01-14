/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcProjectedOrTrueLengthEnum = ENUMERATION OF	(PROJECTED_LENGTH	,TRUE_LENGTH);
class IFCQUERY_EXPORT IfcProjectedOrTrueLengthEnum : virtual public BuildingObject
{
public:
	enum IfcProjectedOrTrueLengthEnumEnum
	{
		ENUM_PROJECTED_LENGTH,
		ENUM_TRUE_LENGTH
	};

	IfcProjectedOrTrueLengthEnum() = default;
	IfcProjectedOrTrueLengthEnum( IfcProjectedOrTrueLengthEnumEnum e ) { m_enum = e; }
	~IfcProjectedOrTrueLengthEnum();
	virtual const char* className() const { return "IfcProjectedOrTrueLengthEnum"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcProjectedOrTrueLengthEnum> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcProjectedOrTrueLengthEnumEnum m_enum;
};

