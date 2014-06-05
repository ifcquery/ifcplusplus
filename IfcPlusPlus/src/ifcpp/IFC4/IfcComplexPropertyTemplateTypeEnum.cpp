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
#include "include/IfcComplexPropertyTemplateTypeEnum.h"

// TYPE IfcComplexPropertyTemplateTypeEnum 
IfcComplexPropertyTemplateTypeEnum::IfcComplexPropertyTemplateTypeEnum() {}
IfcComplexPropertyTemplateTypeEnum::~IfcComplexPropertyTemplateTypeEnum() {}
void IfcComplexPropertyTemplateTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCCOMPLEXPROPERTYTEMPLATETYPEENUM("; }
	if( m_enum == ENUM_P_COMPLEX )
	{
		stream << ".P_COMPLEX.";
	}
	else if( m_enum == ENUM_Q_COMPLEX )
	{
		stream << ".Q_COMPLEX.";
	}
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcComplexPropertyTemplateTypeEnum> IfcComplexPropertyTemplateTypeEnum::createObjectFromStepData( const std::string& arg )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcComplexPropertyTemplateTypeEnum>(); }
	shared_ptr<IfcComplexPropertyTemplateTypeEnum> type_object( new IfcComplexPropertyTemplateTypeEnum() );
	if( _stricmp( arg.c_str(), ".P_COMPLEX." ) == 0 )
	{
		type_object->m_enum = IfcComplexPropertyTemplateTypeEnum::ENUM_P_COMPLEX;
	}
	else if( _stricmp( arg.c_str(), ".Q_COMPLEX." ) == 0 )
	{
		type_object->m_enum = IfcComplexPropertyTemplateTypeEnum::ENUM_Q_COMPLEX;
	}
	return type_object;
}
