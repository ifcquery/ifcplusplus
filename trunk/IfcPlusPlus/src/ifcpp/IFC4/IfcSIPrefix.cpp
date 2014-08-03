/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/

#include <sstream>
#include <limits>
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcSIPrefix.h"

// TYPE IfcSIPrefix 
IfcSIPrefix::IfcSIPrefix() {}
IfcSIPrefix::~IfcSIPrefix() {}
shared_ptr<IfcPPObject> IfcSIPrefix::getDeepCopy()
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
shared_ptr<IfcSIPrefix> IfcSIPrefix::createObjectFromStepData( const std::wstring& arg )
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
