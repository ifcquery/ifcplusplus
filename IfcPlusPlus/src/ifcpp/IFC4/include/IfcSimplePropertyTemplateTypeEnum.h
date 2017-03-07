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

// TYPE IfcSimplePropertyTemplateTypeEnum = ENUMERATION OF	(P_SINGLEVALUE	,P_ENUMERATEDVALUE	,P_BOUNDEDVALUE	,P_LISTVALUE	,P_TABLEVALUE	,P_REFERENCEVALUE	,Q_LENGTH	,Q_AREA	,Q_VOLUME	,Q_COUNT	,Q_WEIGHT	,Q_TIME);
class IFCPP_EXPORT IfcSimplePropertyTemplateTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcSimplePropertyTemplateTypeEnumEnum
	{
		ENUM_P_SINGLEVALUE,
		ENUM_P_ENUMERATEDVALUE,
		ENUM_P_BOUNDEDVALUE,
		ENUM_P_LISTVALUE,
		ENUM_P_TABLEVALUE,
		ENUM_P_REFERENCEVALUE,
		ENUM_Q_LENGTH,
		ENUM_Q_AREA,
		ENUM_Q_VOLUME,
		ENUM_Q_COUNT,
		ENUM_Q_WEIGHT,
		ENUM_Q_TIME
	};

	IfcSimplePropertyTemplateTypeEnum();
	IfcSimplePropertyTemplateTypeEnum( IfcSimplePropertyTemplateTypeEnumEnum e ) { m_enum = e; }
	~IfcSimplePropertyTemplateTypeEnum();
	virtual const char* className() const { return "IfcSimplePropertyTemplateTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcSimplePropertyTemplateTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcSimplePropertyTemplateTypeEnumEnum m_enum;
};

