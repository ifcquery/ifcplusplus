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
#include "include/IfcInventoryTypeEnum.h"

// TYPE IfcInventoryTypeEnum = ENUMERATION OF	(ASSETINVENTORY	,SPACEINVENTORY	,FURNITUREINVENTORY	,USERDEFINED	,NOTDEFINED);
IfcInventoryTypeEnum::IfcInventoryTypeEnum() {}
IfcInventoryTypeEnum::~IfcInventoryTypeEnum() {}
shared_ptr<IfcPPObject> IfcInventoryTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcInventoryTypeEnum> copy_self( new IfcInventoryTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcInventoryTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCINVENTORYTYPEENUM("; }
	if( m_enum == ENUM_ASSETINVENTORY )
	{
		stream << ".ASSETINVENTORY.";
	}
	else if( m_enum == ENUM_SPACEINVENTORY )
	{
		stream << ".SPACEINVENTORY.";
	}
	else if( m_enum == ENUM_FURNITUREINVENTORY )
	{
		stream << ".FURNITUREINVENTORY.";
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
shared_ptr<IfcInventoryTypeEnum> IfcInventoryTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcInventoryTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcInventoryTypeEnum>(); }
	shared_ptr<IfcInventoryTypeEnum> type_object( new IfcInventoryTypeEnum() );
	if( boost::iequals( arg, L".ASSETINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_ASSETINVENTORY;
	}
	else if( boost::iequals( arg, L".SPACEINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_SPACEINVENTORY;
	}
	else if( boost::iequals( arg, L".FURNITUREINVENTORY." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_FURNITUREINVENTORY;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcInventoryTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
