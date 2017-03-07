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
#include "include/IfcBeamTypeEnum.h"

// TYPE IfcBeamTypeEnum = ENUMERATION OF	(BEAM	,JOIST	,HOLLOWCORE	,LINTEL	,SPANDREL	,T_BEAM	,USERDEFINED	,NOTDEFINED);
IfcBeamTypeEnum::IfcBeamTypeEnum() {}
IfcBeamTypeEnum::~IfcBeamTypeEnum() {}
shared_ptr<IfcPPObject> IfcBeamTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBeamTypeEnum> copy_self( new IfcBeamTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcBeamTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCBEAMTYPEENUM("; }
	if( m_enum == ENUM_BEAM )
	{
		stream << ".BEAM.";
	}
	else if( m_enum == ENUM_JOIST )
	{
		stream << ".JOIST.";
	}
	else if( m_enum == ENUM_HOLLOWCORE )
	{
		stream << ".HOLLOWCORE.";
	}
	else if( m_enum == ENUM_LINTEL )
	{
		stream << ".LINTEL.";
	}
	else if( m_enum == ENUM_SPANDREL )
	{
		stream << ".SPANDREL.";
	}
	else if( m_enum == ENUM_T_BEAM )
	{
		stream << ".T_BEAM.";
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
shared_ptr<IfcBeamTypeEnum> IfcBeamTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcBeamTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcBeamTypeEnum>(); }
	shared_ptr<IfcBeamTypeEnum> type_object( new IfcBeamTypeEnum() );
	if( boost::iequals( arg, L".BEAM." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_BEAM;
	}
	else if( boost::iequals( arg, L".JOIST." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_JOIST;
	}
	else if( boost::iequals( arg, L".HOLLOWCORE." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_HOLLOWCORE;
	}
	else if( boost::iequals( arg, L".LINTEL." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_LINTEL;
	}
	else if( boost::iequals( arg, L".SPANDREL." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_SPANDREL;
	}
	else if( boost::iequals( arg, L".T_BEAM." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_T_BEAM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcBeamTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
