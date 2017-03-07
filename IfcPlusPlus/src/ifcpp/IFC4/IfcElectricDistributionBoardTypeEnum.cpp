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
#include "include/IfcElectricDistributionBoardTypeEnum.h"

// TYPE IfcElectricDistributionBoardTypeEnum = ENUMERATION OF	(CONSUMERUNIT	,DISTRIBUTIONBOARD	,MOTORCONTROLCENTRE	,SWITCHBOARD	,USERDEFINED	,NOTDEFINED);
IfcElectricDistributionBoardTypeEnum::IfcElectricDistributionBoardTypeEnum() {}
IfcElectricDistributionBoardTypeEnum::~IfcElectricDistributionBoardTypeEnum() {}
shared_ptr<IfcPPObject> IfcElectricDistributionBoardTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcElectricDistributionBoardTypeEnum> copy_self( new IfcElectricDistributionBoardTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElectricDistributionBoardTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELECTRICDISTRIBUTIONBOARDTYPEENUM("; }
	if( m_enum == ENUM_CONSUMERUNIT )
	{
		stream << ".CONSUMERUNIT.";
	}
	else if( m_enum == ENUM_DISTRIBUTIONBOARD )
	{
		stream << ".DISTRIBUTIONBOARD.";
	}
	else if( m_enum == ENUM_MOTORCONTROLCENTRE )
	{
		stream << ".MOTORCONTROLCENTRE.";
	}
	else if( m_enum == ENUM_SWITCHBOARD )
	{
		stream << ".SWITCHBOARD.";
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
shared_ptr<IfcElectricDistributionBoardTypeEnum> IfcElectricDistributionBoardTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElectricDistributionBoardTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElectricDistributionBoardTypeEnum>(); }
	shared_ptr<IfcElectricDistributionBoardTypeEnum> type_object( new IfcElectricDistributionBoardTypeEnum() );
	if( boost::iequals( arg, L".CONSUMERUNIT." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_CONSUMERUNIT;
	}
	else if( boost::iequals( arg, L".DISTRIBUTIONBOARD." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_DISTRIBUTIONBOARD;
	}
	else if( boost::iequals( arg, L".MOTORCONTROLCENTRE." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_MOTORCONTROLCENTRE;
	}
	else if( boost::iequals( arg, L".SWITCHBOARD." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_SWITCHBOARD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElectricDistributionBoardTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
