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
#include "include/IfcPileConstructionEnum.h"

// TYPE IfcPileConstructionEnum = ENUMERATION OF	(CAST_IN_PLACE	,COMPOSITE	,PRECAST_CONCRETE	,PREFAB_STEEL	,USERDEFINED	,NOTDEFINED);
IfcPileConstructionEnum::IfcPileConstructionEnum() {}
IfcPileConstructionEnum::~IfcPileConstructionEnum() {}
shared_ptr<IfcPPObject> IfcPileConstructionEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPileConstructionEnum> copy_self( new IfcPileConstructionEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPileConstructionEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPILECONSTRUCTIONENUM("; }
	if( m_enum == ENUM_CAST_IN_PLACE )
	{
		stream << ".CAST_IN_PLACE.";
	}
	else if( m_enum == ENUM_COMPOSITE )
	{
		stream << ".COMPOSITE.";
	}
	else if( m_enum == ENUM_PRECAST_CONCRETE )
	{
		stream << ".PRECAST_CONCRETE.";
	}
	else if( m_enum == ENUM_PREFAB_STEEL )
	{
		stream << ".PREFAB_STEEL.";
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
shared_ptr<IfcPileConstructionEnum> IfcPileConstructionEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPileConstructionEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPileConstructionEnum>(); }
	shared_ptr<IfcPileConstructionEnum> type_object( new IfcPileConstructionEnum() );
	if( boost::iequals( arg, L".CAST_IN_PLACE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_CAST_IN_PLACE;
	}
	else if( boost::iequals( arg, L".COMPOSITE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_COMPOSITE;
	}
	else if( boost::iequals( arg, L".PRECAST_CONCRETE." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PRECAST_CONCRETE;
	}
	else if( boost::iequals( arg, L".PREFAB_STEEL." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_PREFAB_STEEL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPileConstructionEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
