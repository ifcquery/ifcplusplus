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

// TYPE IfcFootingTypeEnum = ENUMERATION OF	(CAISSON_FOUNDATION	,FOOTING_BEAM	,PAD_FOOTING	,PILE_CAP	,STRIP_FOOTING	,USERDEFINED	,NOTDEFINED);
class IFCPP_EXPORT IfcFootingTypeEnum : virtual public IfcPPObject
{
public:
	enum IfcFootingTypeEnumEnum
	{
		ENUM_CAISSON_FOUNDATION,
		ENUM_FOOTING_BEAM,
		ENUM_PAD_FOOTING,
		ENUM_PILE_CAP,
		ENUM_STRIP_FOOTING,
		ENUM_USERDEFINED,
		ENUM_NOTDEFINED
	};

	IfcFootingTypeEnum();
	IfcFootingTypeEnum( IfcFootingTypeEnumEnum e ) { m_enum = e; }
	~IfcFootingTypeEnum();
	virtual const char* className() const { return "IfcFootingTypeEnum"; }
	virtual shared_ptr<IfcPPObject> getDeepCopy( IfcPPCopyOptions& options );
	virtual void getStepParameter( std::stringstream& stream, bool is_select_type = false ) const;
	static shared_ptr<IfcFootingTypeEnum> createObjectFromSTEP( const std::wstring& arg );
	IfcFootingTypeEnumEnum m_enum;
};

