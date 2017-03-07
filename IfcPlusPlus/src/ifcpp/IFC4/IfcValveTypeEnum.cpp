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
#include "include/IfcValveTypeEnum.h"

// TYPE IfcValveTypeEnum = ENUMERATION OF	(AIRRELEASE	,ANTIVACUUM	,CHANGEOVER	,CHECK	,COMMISSIONING	,DIVERTING	,DRAWOFFCOCK	,DOUBLECHECK	,DOUBLEREGULATING	,FAUCET	,FLUSHING	,GASCOCK	,GASTAP	,ISOLATING	,MIXING	,PRESSUREREDUCING	,PRESSURERELIEF	,REGULATING	,SAFETYCUTOFF	,STEAMTRAP	,STOPCOCK	,USERDEFINED	,NOTDEFINED);
IfcValveTypeEnum::IfcValveTypeEnum() {}
IfcValveTypeEnum::~IfcValveTypeEnum() {}
shared_ptr<IfcPPObject> IfcValveTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcValveTypeEnum> copy_self( new IfcValveTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcValveTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCVALVETYPEENUM("; }
	if( m_enum == ENUM_AIRRELEASE )
	{
		stream << ".AIRRELEASE.";
	}
	else if( m_enum == ENUM_ANTIVACUUM )
	{
		stream << ".ANTIVACUUM.";
	}
	else if( m_enum == ENUM_CHANGEOVER )
	{
		stream << ".CHANGEOVER.";
	}
	else if( m_enum == ENUM_CHECK )
	{
		stream << ".CHECK.";
	}
	else if( m_enum == ENUM_COMMISSIONING )
	{
		stream << ".COMMISSIONING.";
	}
	else if( m_enum == ENUM_DIVERTING )
	{
		stream << ".DIVERTING.";
	}
	else if( m_enum == ENUM_DRAWOFFCOCK )
	{
		stream << ".DRAWOFFCOCK.";
	}
	else if( m_enum == ENUM_DOUBLECHECK )
	{
		stream << ".DOUBLECHECK.";
	}
	else if( m_enum == ENUM_DOUBLEREGULATING )
	{
		stream << ".DOUBLEREGULATING.";
	}
	else if( m_enum == ENUM_FAUCET )
	{
		stream << ".FAUCET.";
	}
	else if( m_enum == ENUM_FLUSHING )
	{
		stream << ".FLUSHING.";
	}
	else if( m_enum == ENUM_GASCOCK )
	{
		stream << ".GASCOCK.";
	}
	else if( m_enum == ENUM_GASTAP )
	{
		stream << ".GASTAP.";
	}
	else if( m_enum == ENUM_ISOLATING )
	{
		stream << ".ISOLATING.";
	}
	else if( m_enum == ENUM_MIXING )
	{
		stream << ".MIXING.";
	}
	else if( m_enum == ENUM_PRESSUREREDUCING )
	{
		stream << ".PRESSUREREDUCING.";
	}
	else if( m_enum == ENUM_PRESSURERELIEF )
	{
		stream << ".PRESSURERELIEF.";
	}
	else if( m_enum == ENUM_REGULATING )
	{
		stream << ".REGULATING.";
	}
	else if( m_enum == ENUM_SAFETYCUTOFF )
	{
		stream << ".SAFETYCUTOFF.";
	}
	else if( m_enum == ENUM_STEAMTRAP )
	{
		stream << ".STEAMTRAP.";
	}
	else if( m_enum == ENUM_STOPCOCK )
	{
		stream << ".STOPCOCK.";
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
shared_ptr<IfcValveTypeEnum> IfcValveTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcValveTypeEnum>(); }
	shared_ptr<IfcValveTypeEnum> type_object( new IfcValveTypeEnum() );
	if( boost::iequals( arg, L".AIRRELEASE." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_AIRRELEASE;
	}
	else if( boost::iequals( arg, L".ANTIVACUUM." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ANTIVACUUM;
	}
	else if( boost::iequals( arg, L".CHANGEOVER." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHANGEOVER;
	}
	else if( boost::iequals( arg, L".CHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_CHECK;
	}
	else if( boost::iequals( arg, L".COMMISSIONING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_COMMISSIONING;
	}
	else if( boost::iequals( arg, L".DIVERTING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DIVERTING;
	}
	else if( boost::iequals( arg, L".DRAWOFFCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DRAWOFFCOCK;
	}
	else if( boost::iequals( arg, L".DOUBLECHECK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLECHECK;
	}
	else if( boost::iequals( arg, L".DOUBLEREGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_DOUBLEREGULATING;
	}
	else if( boost::iequals( arg, L".FAUCET." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FAUCET;
	}
	else if( boost::iequals( arg, L".FLUSHING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_FLUSHING;
	}
	else if( boost::iequals( arg, L".GASCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASCOCK;
	}
	else if( boost::iequals( arg, L".GASTAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_GASTAP;
	}
	else if( boost::iequals( arg, L".ISOLATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_ISOLATING;
	}
	else if( boost::iequals( arg, L".MIXING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_MIXING;
	}
	else if( boost::iequals( arg, L".PRESSUREREDUCING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSUREREDUCING;
	}
	else if( boost::iequals( arg, L".PRESSURERELIEF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_PRESSURERELIEF;
	}
	else if( boost::iequals( arg, L".REGULATING." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_REGULATING;
	}
	else if( boost::iequals( arg, L".SAFETYCUTOFF." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_SAFETYCUTOFF;
	}
	else if( boost::iequals( arg, L".STEAMTRAP." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STEAMTRAP;
	}
	else if( boost::iequals( arg, L".STOPCOCK." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_STOPCOCK;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcValveTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
