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
#include "include/IfcPipeSegmentTypeEnum.h"

// TYPE IfcPipeSegmentTypeEnum = ENUMERATION OF	(CULVERT	,FLEXIBLESEGMENT	,RIGIDSEGMENT	,GUTTER	,SPOOL	,USERDEFINED	,NOTDEFINED);
IfcPipeSegmentTypeEnum::IfcPipeSegmentTypeEnum() {}
IfcPipeSegmentTypeEnum::~IfcPipeSegmentTypeEnum() {}
shared_ptr<IfcPPObject> IfcPipeSegmentTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPipeSegmentTypeEnum> copy_self( new IfcPipeSegmentTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcPipeSegmentTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPIPESEGMENTTYPEENUM("; }
	if( m_enum == ENUM_CULVERT )
	{
		stream << ".CULVERT.";
	}
	else if( m_enum == ENUM_FLEXIBLESEGMENT )
	{
		stream << ".FLEXIBLESEGMENT.";
	}
	else if( m_enum == ENUM_RIGIDSEGMENT )
	{
		stream << ".RIGIDSEGMENT.";
	}
	else if( m_enum == ENUM_GUTTER )
	{
		stream << ".GUTTER.";
	}
	else if( m_enum == ENUM_SPOOL )
	{
		stream << ".SPOOL.";
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
shared_ptr<IfcPipeSegmentTypeEnum> IfcPipeSegmentTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcPipeSegmentTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcPipeSegmentTypeEnum>(); }
	shared_ptr<IfcPipeSegmentTypeEnum> type_object( new IfcPipeSegmentTypeEnum() );
	if( boost::iequals( arg, L".CULVERT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_CULVERT;
	}
	else if( boost::iequals( arg, L".FLEXIBLESEGMENT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_FLEXIBLESEGMENT;
	}
	else if( boost::iequals( arg, L".RIGIDSEGMENT." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_RIGIDSEGMENT;
	}
	else if( boost::iequals( arg, L".GUTTER." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_GUTTER;
	}
	else if( boost::iequals( arg, L".SPOOL." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_SPOOL;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcPipeSegmentTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
