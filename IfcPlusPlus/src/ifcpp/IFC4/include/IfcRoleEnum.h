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

// TYPE IfcRoleEnum = ENUMERATION OF	(SUPPLIER	,MANUFACTURER	,CONTRACTOR	,SUBCONTRACTOR	,ARCHITECT	,STRUCTURALENGINEER	,COSTENGINEER	,CLIENT	,BUILDINGOWNER	,BUILDINGOPERATOR	,MECHANICALENGINEER	,ELECTRICALENGINEER	,PROJECTMANAGER	,FACILITIESMANAGER	,CIVILENGINEER	,COMMISSIONINGENGINEER	,ENGINEER	,OWNER	,CONSULTANT	,CONSTRUCTIONMANAGER	,FIELDCONSTRUCTIONMANAGER	,RESELLER	,USERDEFINED);
class IFCPP_EXPORT IfcRoleEnum : virtual public IfcPPObject
{
public:
	enum IfcRoleEnumEnum
	{
		ENUM_SUPPLIER,
		ENUM_MANUFACTURER,
		ENUM_CONTRACTOR,
		ENUM_SUBCONTRACTOR,
		ENUM_ARCHITECT,
		ENUM_STRUCTURALENGINEER,
		ENUM_COSTENGINEER,
		ENUM_CLIENT,
		ENUM_BUILDINGOWNER,
		ENUM_BUILDINGOPERATOR,
		ENUM_MECHANICALENGINEER,
		ENUM_ELECTRICALENGINEER,
		ENUM_PROJECTMANAGER,
		ENUM_FACILITIESMANAGER,
		ENUM_CIVILENGINEER,
		ENUM_COMMISSIONINGENGINEER,
		ENUM_ENGINEER,
		ENUM_OWNER,
		ENUM_CONSULTANT,
		ENUM_CONSTRUCTIONMANAGER,
		ENUM_FIELDCONSTRUCTIONMANAGER,
		ENUM_RESELLER,
		ENUM_USERDEFINED
	};

	IfcRoleEnum();
	IfcRoleEnum( IfcRoleEnumEnum e ) { m_enum = e; }
	~IfcRoleEnum();
	virtual const char* className() const { return "IfcRoleEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcRoleEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcRoleEnumEnum m_enum;
};

