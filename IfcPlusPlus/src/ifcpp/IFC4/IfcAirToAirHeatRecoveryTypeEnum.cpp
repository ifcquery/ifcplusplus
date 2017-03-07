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
#include "include/IfcAirToAirHeatRecoveryTypeEnum.h"

// TYPE IfcAirToAirHeatRecoveryTypeEnum = ENUMERATION OF	(FIXEDPLATECOUNTERFLOWEXCHANGER	,FIXEDPLATECROSSFLOWEXCHANGER	,FIXEDPLATEPARALLELFLOWEXCHANGER	,ROTARYWHEEL	,RUNAROUNDCOILLOOP	,HEATPIPE	,TWINTOWERENTHALPYRECOVERYLOOPS	,THERMOSIPHONSEALEDTUBEHEATEXCHANGERS	,THERMOSIPHONCOILTYPEHEATEXCHANGERS	,USERDEFINED	,NOTDEFINED);
IfcAirToAirHeatRecoveryTypeEnum::IfcAirToAirHeatRecoveryTypeEnum() {}
IfcAirToAirHeatRecoveryTypeEnum::~IfcAirToAirHeatRecoveryTypeEnum() {}
shared_ptr<IfcPPObject> IfcAirToAirHeatRecoveryTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAirToAirHeatRecoveryTypeEnum> copy_self( new IfcAirToAirHeatRecoveryTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcAirToAirHeatRecoveryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCAIRTOAIRHEATRECOVERYTYPEENUM("; }
	if( m_enum == ENUM_FIXEDPLATECOUNTERFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATECOUNTERFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_FIXEDPLATECROSSFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATECROSSFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_FIXEDPLATEPARALLELFLOWEXCHANGER )
	{
		stream << ".FIXEDPLATEPARALLELFLOWEXCHANGER.";
	}
	else if( m_enum == ENUM_ROTARYWHEEL )
	{
		stream << ".ROTARYWHEEL.";
	}
	else if( m_enum == ENUM_RUNAROUNDCOILLOOP )
	{
		stream << ".RUNAROUNDCOILLOOP.";
	}
	else if( m_enum == ENUM_HEATPIPE )
	{
		stream << ".HEATPIPE.";
	}
	else if( m_enum == ENUM_TWINTOWERENTHALPYRECOVERYLOOPS )
	{
		stream << ".TWINTOWERENTHALPYRECOVERYLOOPS.";
	}
	else if( m_enum == ENUM_THERMOSIPHONSEALEDTUBEHEATEXCHANGERS )
	{
		stream << ".THERMOSIPHONSEALEDTUBEHEATEXCHANGERS.";
	}
	else if( m_enum == ENUM_THERMOSIPHONCOILTYPEHEATEXCHANGERS )
	{
		stream << ".THERMOSIPHONCOILTYPEHEATEXCHANGERS.";
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
shared_ptr<IfcAirToAirHeatRecoveryTypeEnum> IfcAirToAirHeatRecoveryTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcAirToAirHeatRecoveryTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcAirToAirHeatRecoveryTypeEnum>(); }
	shared_ptr<IfcAirToAirHeatRecoveryTypeEnum> type_object( new IfcAirToAirHeatRecoveryTypeEnum() );
	if( boost::iequals( arg, L".FIXEDPLATECOUNTERFLOWEXCHANGER." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATECOUNTERFLOWEXCHANGER;
	}
	else if( boost::iequals( arg, L".FIXEDPLATECROSSFLOWEXCHANGER." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATECROSSFLOWEXCHANGER;
	}
	else if( boost::iequals( arg, L".FIXEDPLATEPARALLELFLOWEXCHANGER." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_FIXEDPLATEPARALLELFLOWEXCHANGER;
	}
	else if( boost::iequals( arg, L".ROTARYWHEEL." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_ROTARYWHEEL;
	}
	else if( boost::iequals( arg, L".RUNAROUNDCOILLOOP." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_RUNAROUNDCOILLOOP;
	}
	else if( boost::iequals( arg, L".HEATPIPE." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_HEATPIPE;
	}
	else if( boost::iequals( arg, L".TWINTOWERENTHALPYRECOVERYLOOPS." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_TWINTOWERENTHALPYRECOVERYLOOPS;
	}
	else if( boost::iequals( arg, L".THERMOSIPHONSEALEDTUBEHEATEXCHANGERS." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_THERMOSIPHONSEALEDTUBEHEATEXCHANGERS;
	}
	else if( boost::iequals( arg, L".THERMOSIPHONCOILTYPEHEATEXCHANGERS." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_THERMOSIPHONCOILTYPEHEATEXCHANGERS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcAirToAirHeatRecoveryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
