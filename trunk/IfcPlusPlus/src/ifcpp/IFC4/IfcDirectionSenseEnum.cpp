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
#include "include/IfcDirectionSenseEnum.h"

// TYPE IfcDirectionSenseEnum = ENUMERATION OF	(POSITIVE	,NEGATIVE);
IfcDirectionSenseEnum::IfcDirectionSenseEnum() {}
IfcDirectionSenseEnum::~IfcDirectionSenseEnum() {}
shared_ptr<IfcPPObject> IfcDirectionSenseEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDirectionSenseEnum> copy_self( new IfcDirectionSenseEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDirectionSenseEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDIRECTIONSENSEENUM("; }
	if( m_enum == ENUM_POSITIVE )
	{
		stream << ".POSITIVE.";
	}
	else if( m_enum == ENUM_NEGATIVE )
	{
		stream << ".NEGATIVE.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcDirectionSenseEnum> IfcDirectionSenseEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDirectionSenseEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDirectionSenseEnum>(); }
	shared_ptr<IfcDirectionSenseEnum> type_object( new IfcDirectionSenseEnum() );
	if( boost::iequals( arg, L".POSITIVE." ) )
	{
		type_object->m_enum = IfcDirectionSenseEnum::ENUM_POSITIVE;
	}
	else if( boost::iequals( arg, L".NEGATIVE." ) )
	{
		type_object->m_enum = IfcDirectionSenseEnum::ENUM_NEGATIVE;
	}
	return type_object;
}
