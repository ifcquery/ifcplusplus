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

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcBoolean.h"
#include "include/IfcLabel.h"
#include "include/IfcTextFontSelect.h"
#include "include/IfcTextStyle.h"
#include "include/IfcTextStyleForDefinedFont.h"
#include "include/IfcTextStyleTextModel.h"

// ENTITY IfcTextStyle 
IfcTextStyle::IfcTextStyle() { m_entity_enum = IFCTEXTSTYLE; }
IfcTextStyle::IfcTextStyle( int id ) { m_id = id; m_entity_enum = IFCTEXTSTYLE; }
IfcTextStyle::~IfcTextStyle() {}
shared_ptr<IfcPPObject> IfcTextStyle::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextStyle> copy_self( new IfcTextStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_TextCharacterAppearance ) { copy_self->m_TextCharacterAppearance = dynamic_pointer_cast<IfcTextStyleForDefinedFont>( m_TextCharacterAppearance->getDeepCopy(options) ); }
	if( m_TextStyle ) { copy_self->m_TextStyle = dynamic_pointer_cast<IfcTextStyleTextModel>( m_TextStyle->getDeepCopy(options) ); }
	if( m_TextFontStyle ) { copy_self->m_TextFontStyle = dynamic_pointer_cast<IfcTextFontSelect>( m_TextFontStyle->getDeepCopy(options) ); }
	if( m_ModelOrDraughting ) { copy_self->m_ModelOrDraughting = dynamic_pointer_cast<IfcBoolean>( m_ModelOrDraughting->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTextStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTSTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TextCharacterAppearance ) { stream << "#" << m_TextCharacterAppearance->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_TextStyle ) { stream << "#" << m_TextStyle->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_TextFontStyle ) { m_TextFontStyle->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ModelOrDraughting ) { m_ModelOrDraughting->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyle::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextStyle, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readEntityReference( args[1], m_TextCharacterAppearance, map );
	readEntityReference( args[2], m_TextStyle, map );
	m_TextFontStyle = IfcTextFontSelect::createObjectFromSTEP( args[3], map );
	m_ModelOrDraughting = IfcBoolean::createObjectFromSTEP( args[4] );
}
void IfcTextStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationStyle::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TextCharacterAppearance", m_TextCharacterAppearance ) );
	vec_attributes.push_back( std::make_pair( "TextStyle", m_TextStyle ) );
	vec_attributes.push_back( std::make_pair( "TextFontStyle", m_TextFontStyle ) );
	vec_attributes.push_back( std::make_pair( "ModelOrDraughting", m_ModelOrDraughting ) );
}
void IfcTextStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationStyle::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyle::unlinkFromInverseCounterparts()
{
	IfcPresentationStyle::unlinkFromInverseCounterparts();
}
