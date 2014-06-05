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
#include "include/IfcColour.h"
#include "include/IfcTextStyleForDefinedFont.h"

// ENTITY IfcTextStyleForDefinedFont 
IfcTextStyleForDefinedFont::IfcTextStyleForDefinedFont() {}
IfcTextStyleForDefinedFont::IfcTextStyleForDefinedFont( int id ) { m_id = id; }
IfcTextStyleForDefinedFont::~IfcTextStyleForDefinedFont() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTextStyleForDefinedFont::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextStyleForDefinedFont> other = dynamic_pointer_cast<IfcTextStyleForDefinedFont>(other_entity);
	if( !other) { return; }
	m_Colour = other->m_Colour;
	m_BackgroundColour = other->m_BackgroundColour;
}
void IfcTextStyleForDefinedFont::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTSTYLEFORDEFINEDFONT" << "(";
	if( m_Colour ) { m_Colour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_BackgroundColour ) { m_BackgroundColour->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextStyleForDefinedFont::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyleForDefinedFont::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<2 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextStyleForDefinedFont, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>2 ){ std::cout << "Wrong parameter count for entity IfcTextStyleForDefinedFont, expecting 2, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Colour = IfcColour::createObjectFromStepData( args[0], map );
	m_BackgroundColour = IfcColour::createObjectFromStepData( args[1], map );
}
void IfcTextStyleForDefinedFont::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Colour", m_Colour ) );
	vec_attributes.push_back( std::make_pair( "BackgroundColour", m_BackgroundColour ) );
}
void IfcTextStyleForDefinedFont::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTextStyleForDefinedFont::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyleForDefinedFont::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
