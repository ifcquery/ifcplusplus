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
#include "include/IfcSIPrefix.h"

// TYPE IfcSIPrefix = ENUMERATION OF	(EXA	,PETA	,TERA	,GIGA	,MEGA	,KILO	,HECTO	,DECA	,DECI	,CENTI	,MILLI	,MICRO	,NANO	,PICO	,FEMTO	,ATTO);
IfcSIPrefix::IfcSIPrefix() {}
IfcSIPrefix::~IfcSIPrefix() {}
shared_ptr<IfcPPObject> IfcSIPrefix::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSIPrefix> copy_self( new IfcSIPrefix() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcSIPrefix::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCSIPREFIX("; }
	if( m_enum == ENUM_EXA )
	{
		stream << ".EXA.";
	}
	else if( m_enum == ENUM_PETA )
	{
		stream << ".PETA.";
	}
	else if( m_enum == ENUM_TERA )
	{
		stream << ".TERA.";
	}
	else if( m_enum == ENUM_GIGA )
	{
		stream << ".GIGA.";
	}
	else if( m_enum == ENUM_MEGA )
	{
		stream << ".MEGA.";
	}
	else if( m_enum == ENUM_KILO )
	{
		stream << ".KILO.";
	}
	else if( m_enum == ENUM_HECTO )
	{
		stream << ".HECTO.";
	}
	else if( m_enum == ENUM_DECA )
	{
		stream << ".DECA.";
	}
	else if( m_enum == ENUM_DECI )
	{
		stream << ".DECI.";
	}
	else if( m_enum == ENUM_CENTI )
	{
		stream << ".CENTI.";
	}
	else if( m_enum == ENUM_MILLI )
	{
		stream << ".MILLI.";
	}
	else if( m_enum == ENUM_MICRO )
	{
		stream << ".MICRO.";
	}
	else if( m_enum == ENUM_NANO )
	{
		stream << ".NANO.";
	}
	else if( m_enum == ENUM_PICO )
	{
		stream << ".PICO.";
	}
	else if( m_enum == ENUM_FEMTO )
	{
		stream << ".FEMTO.";
	}
	else if( m_enum == ENUM_ATTO )
	{
		stream << ".ATTO.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcSIPrefix> IfcSIPrefix::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcSIPrefix>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcSIPrefix>(); }
	shared_ptr<IfcSIPrefix> type_object( new IfcSIPrefix() );
	if( boost::iequals( arg, L".EXA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_EXA;
	}
	else if( boost::iequals( arg, L".PETA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_PETA;
	}
	else if( boost::iequals( arg, L".TERA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_TERA;
	}
	else if( boost::iequals( arg, L".GIGA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_GIGA;
	}
	else if( boost::iequals( arg, L".MEGA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_MEGA;
	}
	else if( boost::iequals( arg, L".KILO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_KILO;
	}
	else if( boost::iequals( arg, L".HECTO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_HECTO;
	}
	else if( boost::iequals( arg, L".DECA." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_DECA;
	}
	else if( boost::iequals( arg, L".DECI." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_DECI;
	}
	else if( boost::iequals( arg, L".CENTI." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_CENTI;
	}
	else if( boost::iequals( arg, L".MILLI." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_MILLI;
	}
	else if( boost::iequals( arg, L".MICRO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_MICRO;
	}
	else if( boost::iequals( arg, L".NANO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_NANO;
	}
	else if( boost::iequals( arg, L".PICO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_PICO;
	}
	else if( boost::iequals( arg, L".FEMTO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_FEMTO;
	}
	else if( boost::iequals( arg, L".ATTO." ) )
	{
		type_object->m_enum = IfcSIPrefix::ENUM_ATTO;
	}
	return type_object;
}
