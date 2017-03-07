/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPObject.h"
#include "ifcpp/model/IfcPPGlobal.h"

// TYPE IfcGeometricProjectionEnum = ENUMERATION OF	(GRAPH_VIEW	,SKETCH_VIEW	,MODEL_VIEW	,PLAN_VIEW	,REFLECTED_PLAN_VIEW	,SECTION_VIEW	,ELEVATION_VIEW	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcGeometricProjectionEnum : virtual public IfcPPObject
{
public:
	enum IfcGeometricProjectionEnumEnum
	{
		ENUM_GRAPH_VIEW,
		ENUM_SKETCH_VIEW,
		ENUM_MODEL_VIEW,
		ENUM_PLAN_VIEW,
		ENUM_REFLECTED_PLAN_VIEW,
		ENUM_SECTION_VIEW,
		ENUM_ELEVATION_VIEW,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcGeometricProjectionEnum();
	IfcGeometricProjectionEnum( IfcGeometricProjectionEnumEnum e ) { m_enum = e; }
	~IfcGeometricProjectionEnum();
	virtual const char* className() const { return "IfcGeometricProjectionEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcGeometricProjectionEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcGeometricProjectionEnumEnum m_enum;
};

