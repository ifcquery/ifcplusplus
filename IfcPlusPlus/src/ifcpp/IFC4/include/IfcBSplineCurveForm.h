/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BasicTypes.h"
#include "ifcpp/model/BuildingObject.h"

// TYPE IfcBSplineCurveForm = ENUMERATION OF	(POLYLINE_FORM	,CIRCULAR_ARC	,ELLIPTIC_ARC	,PARABOLIC_ARC	,HYPERBOLIC_ARC	,UNSPECIFIED);
class IFCQUERY_EXPORT IfcBSplineCurveForm : virtual public BuildingObject
{
public:
	enum IfcBSplineCurveFormEnum
	{
		ENUM_POLYLINE_FORM,
		ENUM_CIRCULAR_ARC,
		ENUM_ELLIPTIC_ARC,
		ENUM_PARABOLIC_ARC,
		ENUM_HYPERBOLIC_ARC,
		ENUM_UNSPECIFIED
	};

	IfcBSplineCurveForm() = default;
	IfcBSplineCurveForm( IfcBSplineCurveFormEnum e ) { m_enum = e; }
	~IfcBSplineCurveForm();
	virtual const char* className() const { return "IfcBSplineCurveForm"; }
	virtual shared_ptr<BuildingObject> getDeepCopy( BuildingCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	virtual const std::wstring toString() const;
	static shared_ptr<IfcBSplineCurveForm> createObjectFromSTEP( const std::wstring& arg, const std::map<int,shared_ptr<BuildingEntity> >& map );
	IfcBSplineCurveFormEnum m_enum;
};

