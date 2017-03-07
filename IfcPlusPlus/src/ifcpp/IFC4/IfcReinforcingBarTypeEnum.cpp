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
#include "include/IfcReinforcingBarTypeEnum.h"

// TYPE IfcReinforcingBarTypeEnum = ENUMERATION OF	(ANCHORING	,EDGE	,LIGATURE	,MAIN	,PUNCHING	,RING	,SHEAR	,STUD	,USERDEFINED	,NOTDEFINED);
IfcReinforcingBarTypeEnum::IfcReinforcingBarTypeEnum() {}
IfcReinforcingBarTypeEnum::~IfcReinforcingBarTypeEnum() {}
shared_ptr<IfcPPObject> IfcReinforcingBarTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcReinforcingBarTypeEnum> copy_self( new IfcReinforcingBarTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcReinforcingBarTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCREINFORCINGBARTYPEENUM("; }
	if( m_enum == ENUM_ANCHORING )
	{
		stream << ".ANCHORING.";
	}
	else if( m_enum == ENUM_EDGE )
	{
		stream << ".EDGE.";
	}
	else if( m_enum == ENUM_LIGATURE )
	{
		stream << ".LIGATURE.";
	}
	else if( m_enum == ENUM_MAIN )
	{
		stream << ".MAIN.";
	}
	else if( m_enum == ENUM_PUNCHING )
	{
		stream << ".PUNCHING.";
	}
	else if( m_enum == ENUM_RING )
	{
		stream << ".RING.";
	}
	else if( m_enum == ENUM_SHEAR )
	{
		stream << ".SHEAR.";
	}
	else if( m_enum == ENUM_STUD )
	{
		stream << ".STUD.";
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
shared_ptr<IfcReinforcingBarTypeEnum> IfcReinforcingBarTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcReinforcingBarTypeEnum>(); }
	shared_ptr<IfcReinforcingBarTypeEnum> type_object( new IfcReinforcingBarTypeEnum() );
	if( boost::iequals( arg, L".ANCHORING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_ANCHORING;
	}
	else if( boost::iequals( arg, L".EDGE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_EDGE;
	}
	else if( boost::iequals( arg, L".LIGATURE." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_LIGATURE;
	}
	else if( boost::iequals( arg, L".MAIN." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_MAIN;
	}
	else if( boost::iequals( arg, L".PUNCHING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_PUNCHING;
	}
	else if( boost::iequals( arg, L".RING." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_RING;
	}
	else if( boost::iequals( arg, L".SHEAR." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_SHEAR;
	}
	else if( boost::iequals( arg, L".STUD." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_STUD;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcReinforcingBarTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
