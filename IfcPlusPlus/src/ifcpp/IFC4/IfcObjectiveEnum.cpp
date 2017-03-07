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

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/IfcPPBasicTypes.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcObjectiveEnum.h"

// TYPE IfcObjectiveEnum = ENUMERATION OF	(CODECOMPLIANCE	,CODEWAIVER	,DESIGNINTENT	,EXTERNAL	,HEALTHANDSAFETY	,MERGECONFLICT	,MODELVIEW	,PARAMETER	,REQUIREMENT	,SPECIFICATION	,TRIGGERCONDITION	,USERDEFINED	,NOTDEFINED);
IfcObjectiveEnum::IfcObjectiveEnum() {}
IfcObjectiveEnum::~IfcObjectiveEnum() {}
shared_ptr<IfcPPObject> IfcObjectiveEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcObjectiveEnum> copy_self( new IfcObjectiveEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcObjectiveEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCOBJECTIVEENUM("; }
	if( m_enum == ENUM_CODECOMPLIANCE )
	{
		stream << ".CODECOMPLIANCE.";
	}
	else if( m_enum == ENUM_CODEWAIVER )
	{
		stream << ".CODEWAIVER.";
	}
	else if( m_enum == ENUM_DESIGNINTENT )
	{
		stream << ".DESIGNINTENT.";
	}
	else if( m_enum == ENUM_EXTERNAL )
	{
		stream << ".EXTERNAL.";
	}
	else if( m_enum == ENUM_HEALTHANDSAFETY )
	{
		stream << ".HEALTHANDSAFETY.";
	}
	else if( m_enum == ENUM_MERGECONFLICT )
	{
		stream << ".MERGECONFLICT.";
	}
	else if( m_enum == ENUM_MODELVIEW )
	{
		stream << ".MODELVIEW.";
	}
	else if( m_enum == ENUM_PARAMETER )
	{
		stream << ".PARAMETER.";
	}
	else if( m_enum == ENUM_REQUIREMENT )
	{
		stream << ".REQUIREMENT.";
	}
	else if( m_enum == ENUM_SPECIFICATION )
	{
		stream << ".SPECIFICATION.";
	}
	else if( m_enum == ENUM_TRIGGERCONDITION )
	{
		stream << ".TRIGGERCONDITION.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	else if( m_enum == ENUM_NOTDEFINED )
	{
		stream << ".NOTDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcObjectiveEnum> IfcObjectiveEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcObjectiveEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcObjectiveEnum>(); }
	shared_ptr<IfcObjectiveEnum> type_object( new IfcObjectiveEnum() );
	if( boost::iequals( arg, L".CODECOMPLIANCE." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_CODECOMPLIANCE;
	}
	else if( boost::iequals( arg, L".CODEWAIVER." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_CODEWAIVER;
	}
	else if( boost::iequals( arg, L".DESIGNINTENT." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_DESIGNINTENT;
	}
	else if( boost::iequals( arg, L".EXTERNAL." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_EXTERNAL;
	}
	else if( boost::iequals( arg, L".HEALTHANDSAFETY." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_HEALTHANDSAFETY;
	}
	else if( boost::iequals( arg, L".MERGECONFLICT." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_MERGECONFLICT;
	}
	else if( boost::iequals( arg, L".MODELVIEW." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_MODELVIEW;
	}
	else if( boost::iequals( arg, L".PARAMETER." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_PARAMETER;
	}
	else if( boost::iequals( arg, L".REQUIREMENT." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_REQUIREMENT;
	}
	else if( boost::iequals( arg, L".SPECIFICATION." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_SPECIFICATION;
	}
	else if( boost::iequals( arg, L".TRIGGERCONDITION." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_TRIGGERCONDITION;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcObjectiveEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
