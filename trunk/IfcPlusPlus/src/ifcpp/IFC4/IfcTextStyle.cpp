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

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcLabel.h"
#include "include/IfcTextFontSelect.h"
#include "include/IfcTextStyle.h"
#include "include/IfcTextStyleForDefinedFont.h"
#include "include/IfcTextStyleTextModel.h"

// ENTITY IfcTextStyle 
IfcTextStyle::IfcTextStyle() {}
IfcTextStyle::IfcTextStyle( int id ) { m_id = id; }
IfcTextStyle::~IfcTextStyle() {}
shared_ptr<IfcPPObject> IfcTextStyle::getDeepCopy()
{
	shared_ptr<IfcTextStyle> copy_self( new IfcTextStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	if( m_TextCharacterAppearance ) { copy_self->m_TextCharacterAppearance = dynamic_pointer_cast<IfcTextStyleForDefinedFont>( m_TextCharacterAppearance->getDeepCopy() ); }
	if( m_TextStyle ) { copy_self->m_TextStyle = dynamic_pointer_cast<IfcTextStyleTextModel>( m_TextStyle->getDeepCopy() ); }
	if( m_TextFontStyle ) { copy_self->m_TextFontStyle = dynamic_pointer_cast<IfcTextFontSelect>( m_TextFontStyle->getDeepCopy() ); }
	if( m_ModelOrDraughting ) { copy_self->m_ModelOrDraughting = m_ModelOrDraughting; }
	return copy_self;
}
void IfcTextStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTSTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_TextCharacterAppearance ) { stream << "#" << m_TextCharacterAppearance->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TextStyle ) { stream << "#" << m_TextStyle->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TextFontStyle ) { m_TextFontStyle->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_ModelOrDraughting == false ) { stream << ".F."; }
	else if( m_ModelOrDraughting == true ) { stream << ".T."; }
	stream << ");";
}
void IfcTextStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyle::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextStyle, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>5 ){ std::cout << "Wrong parameter count for entity IfcTextStyle, expecting 5, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readEntityReference( args[1], m_TextCharacterAppearance, map );
	readEntityReference( args[2], m_TextStyle, map );
	m_TextFontStyle = IfcTextFontSelect::createObjectFromStepData( args[3], map );
	if( boost::iequals( args[4], L".F." ) ) { m_ModelOrDraughting = false; }
	else if( boost::iequals( args[4], L".T." ) ) { m_ModelOrDraughting = true; }
}
void IfcTextStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationStyle::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TextCharacterAppearance", m_TextCharacterAppearance ) );
	vec_attributes.push_back( std::make_pair( "TextStyle", m_TextStyle ) );
	vec_attributes.push_back( std::make_pair( "TextFontStyle", m_TextFontStyle ) );
	vec_attributes.push_back( std::make_pair( "ModelOrDraughting", shared_ptr<IfcPPBool>( new IfcPPBool( m_ModelOrDraughting ) ) ) );
}
void IfcTextStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationStyle::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyle::unlinkSelf()
{
	IfcPresentationStyle::unlinkSelf();
}
