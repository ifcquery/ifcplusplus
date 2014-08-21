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
#include "include/IfcDamperTypeEnum.h"

// TYPE IfcDamperTypeEnum = ENUMERATION OF	(BACKDRAFTDAMPER	,BALANCINGDAMPER	,BLASTDAMPER	,CONTROLDAMPER	,FIREDAMPER	,FIRESMOKEDAMPER	,FUMEHOODEXHAUST	,GRAVITYDAMPER	,GRAVITYRELIEFDAMPER	,RELIEFDAMPER	,SMOKEDAMPER	,USERDEFINED	,NOTDEFINED);
IfcDamperTypeEnum::IfcDamperTypeEnum() {}
IfcDamperTypeEnum::~IfcDamperTypeEnum() {}
shared_ptr<IfcPPObject> IfcDamperTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcDamperTypeEnum> copy_self( new IfcDamperTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcDamperTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCDAMPERTYPEENUM("; }
	if( m_enum == ENUM_BACKDRAFTDAMPER )
	{
		stream << ".BACKDRAFTDAMPER.";
	}
	else if( m_enum == ENUM_BALANCINGDAMPER )
	{
		stream << ".BALANCINGDAMPER.";
	}
	else if( m_enum == ENUM_BLASTDAMPER )
	{
		stream << ".BLASTDAMPER.";
	}
	else if( m_enum == ENUM_CONTROLDAMPER )
	{
		stream << ".CONTROLDAMPER.";
	}
	else if( m_enum == ENUM_FIREDAMPER )
	{
		stream << ".FIREDAMPER.";
	}
	else if( m_enum == ENUM_FIRESMOKEDAMPER )
	{
		stream << ".FIRESMOKEDAMPER.";
	}
	else if( m_enum == ENUM_FUMEHOODEXHAUST )
	{
		stream << ".FUMEHOODEXHAUST.";
	}
	else if( m_enum == ENUM_GRAVITYDAMPER )
	{
		stream << ".GRAVITYDAMPER.";
	}
	else if( m_enum == ENUM_GRAVITYRELIEFDAMPER )
	{
		stream << ".GRAVITYRELIEFDAMPER.";
	}
	else if( m_enum == ENUM_RELIEFDAMPER )
	{
		stream << ".RELIEFDAMPER.";
	}
	else if( m_enum == ENUM_SMOKEDAMPER )
	{
		stream << ".SMOKEDAMPER.";
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
shared_ptr<IfcDamperTypeEnum> IfcDamperTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcDamperTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcDamperTypeEnum>(); }
	shared_ptr<IfcDamperTypeEnum> type_object( new IfcDamperTypeEnum() );
	if( boost::iequals( arg, L".BACKDRAFTDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_BACKDRAFTDAMPER;
	}
	else if( boost::iequals( arg, L".BALANCINGDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_BALANCINGDAMPER;
	}
	else if( boost::iequals( arg, L".BLASTDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_BLASTDAMPER;
	}
	else if( boost::iequals( arg, L".CONTROLDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_CONTROLDAMPER;
	}
	else if( boost::iequals( arg, L".FIREDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_FIREDAMPER;
	}
	else if( boost::iequals( arg, L".FIRESMOKEDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_FIRESMOKEDAMPER;
	}
	else if( boost::iequals( arg, L".FUMEHOODEXHAUST." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_FUMEHOODEXHAUST;
	}
	else if( boost::iequals( arg, L".GRAVITYDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_GRAVITYDAMPER;
	}
	else if( boost::iequals( arg, L".GRAVITYRELIEFDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_GRAVITYRELIEFDAMPER;
	}
	else if( boost::iequals( arg, L".RELIEFDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_RELIEFDAMPER;
	}
	else if( boost::iequals( arg, L".SMOKEDAMPER." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_SMOKEDAMPER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcDamperTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
