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

// method setEntity takes over all attributes from another instance of the class
void IfcTextStyle::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextStyle> other = dynamic_pointer_cast<IfcTextStyle>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_TextCharacterAppearance = other->m_TextCharacterAppearance;
	m_TextStyle = other->m_TextStyle;
	m_TextFontStyle = other->m_TextFontStyle;
	m_ModelOrDraughting = other->m_ModelOrDraughting;
}
void IfcTextStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTSTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TextCharacterAppearance ) { stream << "#" << m_TextCharacterAppearance->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TextStyle ) { stream << "#" << m_TextStyle->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_TextFontStyle ) { m_TextFontStyle->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_ModelOrDraughting == false ) { stream << ".F."; }
	else if( m_ModelOrDraughting == true ) { stream << ".T."; }
	stream << ");";
}
void IfcTextStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyle::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
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
	if( _stricmp( args[4].c_str(), ".F." ) == 0 ) { m_ModelOrDraughting = false; }
	else if( _stricmp( args[4].c_str(), ".T." ) == 0 ) { m_ModelOrDraughting = true; }
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
}
void IfcTextStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyle::unlinkSelf()
{
	IfcPresentationStyle::unlinkSelf();
}
