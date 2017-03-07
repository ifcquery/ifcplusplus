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
#include "include/IfcFootingTypeEnum.h"

// TYPE IfcFootingTypeEnum = ENUMERATION OF	(CAISSON_FOUNDATION	,FOOTING_BEAM	,PAD_FOOTING	,PILE_CAP	,STRIP_FOOTING	,USERDEFINED	,NOTDEFINED);
IfcFootingTypeEnum::IfcFootingTypeEnum() {}
IfcFootingTypeEnum::~IfcFootingTypeEnum() {}
shared_ptr<IfcPPObject> IfcFootingTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFootingTypeEnum> copy_self( new IfcFootingTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcFootingTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCFOOTINGTYPEENUM("; }
	if( m_enum == ENUM_CAISSON_FOUNDATION )
	{
		stream << ".CAISSON_FOUNDATION.";
	}
	else if( m_enum == ENUM_FOOTING_BEAM )
	{
		stream << ".FOOTING_BEAM.";
	}
	else if( m_enum == ENUM_PAD_FOOTING )
	{
		stream << ".PAD_FOOTING.";
	}
	else if( m_enum == ENUM_PILE_CAP )
	{
		stream << ".PILE_CAP.";
	}
	else if( m_enum == ENUM_STRIP_FOOTING )
	{
		stream << ".STRIP_FOOTING.";
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
shared_ptr<IfcFootingTypeEnum> IfcFootingTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcFootingTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcFootingTypeEnum>(); }
	shared_ptr<IfcFootingTypeEnum> type_object( new IfcFootingTypeEnum() );
	if( boost::iequals( arg, L".CAISSON_FOUNDATION." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_CAISSON_FOUNDATION;
	}
	else if( boost::iequals( arg, L".FOOTING_BEAM." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_FOOTING_BEAM;
	}
	else if( boost::iequals( arg, L".PAD_FOOTING." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_PAD_FOOTING;
	}
	else if( boost::iequals( arg, L".PILE_CAP." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_PILE_CAP;
	}
	else if( boost::iequals( arg, L".STRIP_FOOTING." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_STRIP_FOOTING;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcFootingTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
