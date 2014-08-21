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
#include "include/IfcRoleEnum.h"

// TYPE IfcRoleEnum = ENUMERATION OF	(SUPPLIER	,MANUFACTURER	,CONTRACTOR	,SUBCONTRACTOR	,ARCHITECT	,STRUCTURALENGINEER	,COSTENGINEER	,CLIENT	,BUILDINGOWNER	,BUILDINGOPERATOR	,MECHANICALENGINEER	,ELECTRICALENGINEER	,PROJECTMANAGER	,FACILITIESMANAGER	,CIVILENGINEER	,COMMISSIONINGENGINEER	,ENGINEER	,OWNER	,CONSULTANT	,CONSTRUCTIONMANAGER	,FIELDCONSTRUCTIONMANAGER	,RESELLER	,USERDEFINED);
IfcRoleEnum::IfcRoleEnum() {}
IfcRoleEnum::~IfcRoleEnum() {}
shared_ptr<IfcPPObject> IfcRoleEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRoleEnum> copy_self( new IfcRoleEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRoleEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCROLEENUM("; }
	if( m_enum == ENUM_SUPPLIER )
	{
		stream << ".SUPPLIER.";
	}
	else if( m_enum == ENUM_MANUFACTURER )
	{
		stream << ".MANUFACTURER.";
	}
	else if( m_enum == ENUM_CONTRACTOR )
	{
		stream << ".CONTRACTOR.";
	}
	else if( m_enum == ENUM_SUBCONTRACTOR )
	{
		stream << ".SUBCONTRACTOR.";
	}
	else if( m_enum == ENUM_ARCHITECT )
	{
		stream << ".ARCHITECT.";
	}
	else if( m_enum == ENUM_STRUCTURALENGINEER )
	{
		stream << ".STRUCTURALENGINEER.";
	}
	else if( m_enum == ENUM_COSTENGINEER )
	{
		stream << ".COSTENGINEER.";
	}
	else if( m_enum == ENUM_CLIENT )
	{
		stream << ".CLIENT.";
	}
	else if( m_enum == ENUM_BUILDINGOWNER )
	{
		stream << ".BUILDINGOWNER.";
	}
	else if( m_enum == ENUM_BUILDINGOPERATOR )
	{
		stream << ".BUILDINGOPERATOR.";
	}
	else if( m_enum == ENUM_MECHANICALENGINEER )
	{
		stream << ".MECHANICALENGINEER.";
	}
	else if( m_enum == ENUM_ELECTRICALENGINEER )
	{
		stream << ".ELECTRICALENGINEER.";
	}
	else if( m_enum == ENUM_PROJECTMANAGER )
	{
		stream << ".PROJECTMANAGER.";
	}
	else if( m_enum == ENUM_FACILITIESMANAGER )
	{
		stream << ".FACILITIESMANAGER.";
	}
	else if( m_enum == ENUM_CIVILENGINEER )
	{
		stream << ".CIVILENGINEER.";
	}
	else if( m_enum == ENUM_COMMISSIONINGENGINEER )
	{
		stream << ".COMMISSIONINGENGINEER.";
	}
	else if( m_enum == ENUM_ENGINEER )
	{
		stream << ".ENGINEER.";
	}
	else if( m_enum == ENUM_OWNER )
	{
		stream << ".OWNER.";
	}
	else if( m_enum == ENUM_CONSULTANT )
	{
		stream << ".CONSULTANT.";
	}
	else if( m_enum == ENUM_CONSTRUCTIONMANAGER )
	{
		stream << ".CONSTRUCTIONMANAGER.";
	}
	else if( m_enum == ENUM_FIELDCONSTRUCTIONMANAGER )
	{
		stream << ".FIELDCONSTRUCTIONMANAGER.";
	}
	else if( m_enum == ENUM_RESELLER )
	{
		stream << ".RESELLER.";
	}
	else if( m_enum == ENUM_USERDEFINED )
	{
		stream << ".USERDEFINED.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcRoleEnum> IfcRoleEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRoleEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRoleEnum>(); }
	shared_ptr<IfcRoleEnum> type_object( new IfcRoleEnum() );
	if( boost::iequals( arg, L".SUPPLIER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_SUPPLIER;
	}
	else if( boost::iequals( arg, L".MANUFACTURER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_MANUFACTURER;
	}
	else if( boost::iequals( arg, L".CONTRACTOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONTRACTOR;
	}
	else if( boost::iequals( arg, L".SUBCONTRACTOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_SUBCONTRACTOR;
	}
	else if( boost::iequals( arg, L".ARCHITECT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ARCHITECT;
	}
	else if( boost::iequals( arg, L".STRUCTURALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_STRUCTURALENGINEER;
	}
	else if( boost::iequals( arg, L".COSTENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_COSTENGINEER;
	}
	else if( boost::iequals( arg, L".CLIENT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CLIENT;
	}
	else if( boost::iequals( arg, L".BUILDINGOWNER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_BUILDINGOWNER;
	}
	else if( boost::iequals( arg, L".BUILDINGOPERATOR." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_BUILDINGOPERATOR;
	}
	else if( boost::iequals( arg, L".MECHANICALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_MECHANICALENGINEER;
	}
	else if( boost::iequals( arg, L".ELECTRICALENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ELECTRICALENGINEER;
	}
	else if( boost::iequals( arg, L".PROJECTMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_PROJECTMANAGER;
	}
	else if( boost::iequals( arg, L".FACILITIESMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_FACILITIESMANAGER;
	}
	else if( boost::iequals( arg, L".CIVILENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CIVILENGINEER;
	}
	else if( boost::iequals( arg, L".COMMISSIONINGENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_COMMISSIONINGENGINEER;
	}
	else if( boost::iequals( arg, L".ENGINEER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_ENGINEER;
	}
	else if( boost::iequals( arg, L".OWNER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_OWNER;
	}
	else if( boost::iequals( arg, L".CONSULTANT." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONSULTANT;
	}
	else if( boost::iequals( arg, L".CONSTRUCTIONMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_CONSTRUCTIONMANAGER;
	}
	else if( boost::iequals( arg, L".FIELDCONSTRUCTIONMANAGER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_FIELDCONSTRUCTIONMANAGER;
	}
	else if( boost::iequals( arg, L".RESELLER." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_RESELLER;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRoleEnum::ENUM_USERDEFINED;
	}
	return type_object;
}
