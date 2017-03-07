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
#include "include/IfcRoofTypeEnum.h"

// TYPE IfcRoofTypeEnum = ENUMERATION OF	(FLAT_ROOF	,SHED_ROOF	,GABLE_ROOF	,HIP_ROOF	,HIPPED_GABLE_ROOF	,GAMBREL_ROOF	,MANSARD_ROOF	,BARREL_ROOF	,RAINBOW_ROOF	,BUTTERFLY_ROOF	,PAVILION_ROOF	,DOME_ROOF	,FREEFORM	,USERDEFINED	,NOTDEFINED);
IfcRoofTypeEnum::IfcRoofTypeEnum() {}
IfcRoofTypeEnum::~IfcRoofTypeEnum() {}
shared_ptr<IfcPPObject> IfcRoofTypeEnum::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcRoofTypeEnum> copy_self( new IfcRoofTypeEnum() );
	copy_self->m_enum = m_enum;
	return copy_self;
}
void IfcRoofTypeEnum::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCROOFTYPEENUM("; }
	if( m_enum == ENUM_FLAT_ROOF )
	{
		stream << ".FLAT_ROOF.";
	}
	else if( m_enum == ENUM_SHED_ROOF )
	{
		stream << ".SHED_ROOF.";
	}
	else if( m_enum == ENUM_GABLE_ROOF )
	{
		stream << ".GABLE_ROOF.";
	}
	else if( m_enum == ENUM_HIP_ROOF )
	{
		stream << ".HIP_ROOF.";
	}
	else if( m_enum == ENUM_HIPPED_GABLE_ROOF )
	{
		stream << ".HIPPED_GABLE_ROOF.";
	}
	else if( m_enum == ENUM_GAMBREL_ROOF )
	{
		stream << ".GAMBREL_ROOF.";
	}
	else if( m_enum == ENUM_MANSARD_ROOF )
	{
		stream << ".MANSARD_ROOF.";
	}
	else if( m_enum == ENUM_BARREL_ROOF )
	{
		stream << ".BARREL_ROOF.";
	}
	else if( m_enum == ENUM_RAINBOW_ROOF )
	{
		stream << ".RAINBOW_ROOF.";
	}
	else if( m_enum == ENUM_BUTTERFLY_ROOF )
	{
		stream << ".BUTTERFLY_ROOF.";
	}
	else if( m_enum == ENUM_PAVILION_ROOF )
	{
		stream << ".PAVILION_ROOF.";
	}
	else if( m_enum == ENUM_DOME_ROOF )
	{
		stream << ".DOME_ROOF.";
	}
	else if( m_enum == ENUM_FREEFORM )
	{
		stream << ".FREEFORM.";
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
shared_ptr<IfcRoofTypeEnum> IfcRoofTypeEnum::createObjectFromSTEP( const std::wstring& arg )
{
	// read TYPE
	if( arg.compare( L"$" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	else if( arg.compare( L"*" ) == 0 ) { return shared_ptr<IfcRoofTypeEnum>(); }
	shared_ptr<IfcRoofTypeEnum> type_object( new IfcRoofTypeEnum() );
	if( boost::iequals( arg, L".FLAT_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FLAT_ROOF;
	}
	else if( boost::iequals( arg, L".SHED_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_SHED_ROOF;
	}
	else if( boost::iequals( arg, L".GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".HIP_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIP_ROOF;
	}
	else if( boost::iequals( arg, L".HIPPED_GABLE_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_HIPPED_GABLE_ROOF;
	}
	else if( boost::iequals( arg, L".GAMBREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_GAMBREL_ROOF;
	}
	else if( boost::iequals( arg, L".MANSARD_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_MANSARD_ROOF;
	}
	else if( boost::iequals( arg, L".BARREL_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BARREL_ROOF;
	}
	else if( boost::iequals( arg, L".RAINBOW_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_RAINBOW_ROOF;
	}
	else if( boost::iequals( arg, L".BUTTERFLY_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_BUTTERFLY_ROOF;
	}
	else if( boost::iequals( arg, L".PAVILION_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_PAVILION_ROOF;
	}
	else if( boost::iequals( arg, L".DOME_ROOF." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_DOME_ROOF;
	}
	else if( boost::iequals( arg, L".FREEFORM." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_FREEFORM;
	}
	else if( boost::iequals( arg, L".USERDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_USERDEFINED;
	}
	else if( boost::iequals( arg, L".NOTDEFINED." ) )
	{
		type_object->m_enum = IfcRoofTypeEnum::ENUM_NOTDEFINED;
	}
	return type_object;
}
