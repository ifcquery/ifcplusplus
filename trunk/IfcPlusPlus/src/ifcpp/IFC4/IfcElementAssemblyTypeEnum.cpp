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
#include "include/IfcElementAssemblyTypeEnum.h"

// TYPE IfcElementAssemblyTypeEnum = ENUMERATION OF	(ACCESSORY_ASSEMBLY	,ARCH	,BEAM_GRID	,BRACED_FRAME	,GIRDER	,REINFORCEMENT_UNIT	,RIGID_FRAME	,SLAB_FIELD	,TRUSS	,USERDEFINED	,NOTDEFINED);
IfcElementAssemblyTypeEnum::IfcElementAssemblyTypeEnum() {}
IfcElementAssemblyTypeEnum::~IfcElementAssemblyTypeEnum() {}
shared_ptr<IfcPPObject> IfcElementAssemblyTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcElementAssemblyTypeEnum> copy_self( new IfcElementAssemblyTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcElementAssemblyTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCELEMENTASSEMBLYTYPEENUM("; }
	if( m_enum == ENUM_ACCESSORY_ASSEMBLY )
	{
		stream << ".ACCESSORY_ASSEMBLY.";
	}
	else if( m_enum == ENUM_ARCH )
	{
		stream << ".ARCH.";
	}
	else if( m_enum == ENUM_BEAM_GRID )
	{
		stream << ".BEAM_GRID.";
	}
	else if( m_enum == ENUM_BRACED_FRAME )
	{
		stream << ".BRACED_FRAME.";
	}
	else if( m_enum == ENUM_GIRDER )
	{
		stream << ".GIRDER.";
	}
	else if( m_enum == ENUM_REINFORCEMENT_UNIT )
	{
		stream << ".REINFORCEMENT_UNIT.";
	}
	else if( m_enum == ENUM_RIGID_FRAME )
	{
		stream << ".RIGID_FRAME.";
	}
	else if( m_enum == ENUM_SLAB_FIELD )
	{
		stream << ".SLAB_FIELD.";
	}
	else if( m_enum == ENUM_TRUSS )
	{
		stream << ".TRUSS.";
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
shared_ptr<IfcElementAssemblyTypeEnum> IfcElementAssemblyTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcElementAssemblyTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcElementAssemblyTypeEnum>(); }
	shared_ptr<IfcElementAssemblyTypeEnum> type_object( new IfcElementAssemblyTypeEnum() );
	if( boost::iequals( arg, L".ACCESSORY_ASSEMBLY." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ACCESSORY_ASSEMBLY;
	}
	else if( boost::iequals( arg, L".ARCH." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_ARCH;
	}
	else if( boost::iequals( arg, L".BEAM_GRID." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_BEAM_GRID;
	}
	else if( boost::iequals( arg, L".BRACED_FRAME." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_BRACED_FRAME;
	}
	else if( boost::iequals( arg, L".GIRDER." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_GIRDER;
	}
	else if( boost::iequals( arg, L".REINFORCEMENT_UNIT." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_REINFORCEMENT_UNIT;
	}
	else if( boost::iequals( arg, L".RIGID_FRAME." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_RIGID_FRAME;
	}
	else if( boost::iequals( arg, L".SLAB_FIELD." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_SLAB_FIELD;
	}
	else if( boost::iequals( arg, L".TRUSS." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_TRUSS;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcElementAssemblyTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
