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
#include "include/IfcDoorTypeOperationEnum.h"

// TYPE IfcDoorTypeOperationEnum = ENUMERATION OF	(SINGLE_SWING_LEFT	,SINGLE_SWING_RIGHT	,DOUBLE_DOOR_SINGLE_SWING	,DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT	,DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT	,DOUBLE_SWING_LEFT	,DOUBLE_SWING_RIGHT	,DOUBLE_DOOR_DOUBLE_SWING	,SLIDING_TO_LEFT	,SLIDING_TO_RIGHT	,DOUBLE_DOOR_SLIDING	,FOLDING_TO_LEFT	,FOLDING_TO_RIGHT	,DOUBLE_DOOR_FOLDING	,REVOLVING	,ROLLINGUP	,SWING_FIXED_LEFT	,SWING_FIXED_RIGHT	,USERDEFINED	,NOTDEFINED);
IfcDoorTypeOperationEnum::IfcDoorTypeOperationEnum() {}
IfcDoorTypeOperationEnum::~IfcDoorTypeOperationEnum() {}
shared_ptr<IfcPPObject> IfcDoorTypeOperationEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDoorTypeOperationEnum> copy_self( new IfcDoorTypeOperationEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDoorTypeOperationEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDOORTYPEOPERATIONENUM("; }
	if( m_enum == ENUM_SINGLE_SWING_LEFT )
	{
		stream << ".SINGLE_SWING_LEFT.";
	}
	else if( m_enum == ENUM_SINGLE_SWING_RIGHT )
	{
		stream << ".SINGLE_SWING_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT )
	{
		stream << ".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_SWING_LEFT )
	{
		stream << ".DOUBLE_SWING_LEFT.";
	}
	else if( m_enum == ENUM_DOUBLE_SWING_RIGHT )
	{
		stream << ".DOUBLE_SWING_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_DOUBLE_SWING )
	{
		stream << ".DOUBLE_DOOR_DOUBLE_SWING.";
	}
	else if( m_enum == ENUM_SLIDING_TO_LEFT )
	{
		stream << ".SLIDING_TO_LEFT.";
	}
	else if( m_enum == ENUM_SLIDING_TO_RIGHT )
	{
		stream << ".SLIDING_TO_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_SLIDING )
	{
		stream << ".DOUBLE_DOOR_SLIDING.";
	}
	else if( m_enum == ENUM_FOLDING_TO_LEFT )
	{
		stream << ".FOLDING_TO_LEFT.";
	}
	else if( m_enum == ENUM_FOLDING_TO_RIGHT )
	{
		stream << ".FOLDING_TO_RIGHT.";
	}
	else if( m_enum == ENUM_DOUBLE_DOOR_FOLDING )
	{
		stream << ".DOUBLE_DOOR_FOLDING.";
	}
	else if( m_enum == ENUM_REVOLVING )
	{
		stream << ".REVOLVING.";
	}
	else if( m_enum == ENUM_ROLLINGUP )
	{
		stream << ".ROLLINGUP.";
	}
	else if( m_enum == ENUM_SWING_FIXED_LEFT )
	{
		stream << ".SWING_FIXED_LEFT.";
	}
	else if( m_enum == ENUM_SWING_FIXED_RIGHT )
	{
		stream << ".SWING_FIXED_RIGHT.";
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
shared_ptr<IfcDoorTypeOperationEnum> IfcDoorTypeOperationEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDoorTypeOperationEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDoorTypeOperationEnum>(); }
	shared_ptr<IfcDoorTypeOperationEnum> type_object( new IfcDoorTypeOperationEnum() );
	if( boost::iequals( arg, L".SINGLE_SWING_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SINGLE_SWING_LEFT;
	}
	else if( boost::iequals( arg, L".SINGLE_SWING_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SINGLE_SWING_RIGHT;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_SINGLE_SWING." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_LEFT;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_SINGLE_SWING_OPPOSITE_RIGHT;
	}
	else if( boost::iequals( arg, L".DOUBLE_SWING_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_SWING_LEFT;
	}
	else if( boost::iequals( arg, L".DOUBLE_SWING_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_SWING_RIGHT;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_DOUBLE_SWING." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_DOUBLE_SWING;
	}
	else if( boost::iequals( arg, L".SLIDING_TO_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SLIDING_TO_LEFT;
	}
	else if( boost::iequals( arg, L".SLIDING_TO_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SLIDING_TO_RIGHT;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_SLIDING." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_SLIDING;
	}
	else if( boost::iequals( arg, L".FOLDING_TO_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_FOLDING_TO_LEFT;
	}
	else if( boost::iequals( arg, L".FOLDING_TO_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_FOLDING_TO_RIGHT;
	}
	else if( boost::iequals( arg, L".DOUBLE_DOOR_FOLDING." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_DOUBLE_DOOR_FOLDING;
	}
	else if( boost::iequals( arg, L".REVOLVING." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_REVOLVING;
	}
	else if( boost::iequals( arg, L".ROLLINGUP." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_ROLLINGUP;
	}
	else if( boost::iequals( arg, L".SWING_FIXED_LEFT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SWING_FIXED_LEFT;
	}
	else if( boost::iequals( arg, L".SWING_FIXED_RIGHT." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_SWING_FIXED_RIGHT;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDoorTypeOperationEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
