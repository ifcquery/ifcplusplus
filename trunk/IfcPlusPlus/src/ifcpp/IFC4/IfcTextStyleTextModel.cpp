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
#include "include/IfcSizeSelect.h"
#include "include/IfcTextAlignment.h"
#include "include/IfcTextDecoration.h"
#include "include/IfcTextStyleTextModel.h"
#include "include/IfcTextTransformation.h"

// ENTITY IfcTextStyleTextModel 
IfcTextStyleTextModel::IfcTextStyleTextModel() {}
IfcTextStyleTextModel::IfcTextStyleTextModel( int id ) { m_id = id; }
IfcTextStyleTextModel::~IfcTextStyleTextModel() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTextStyleTextModel::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextStyleTextModel> other = dynamic_pointer_cast<IfcTextStyleTextModel>(other_entity);
	if( !other) { return; }
	m_TextIndent = other->m_TextIndent;
	m_TextAlign = other->m_TextAlign;
	m_TextDecoration = other->m_TextDecoration;
	m_LetterSpacing = other->m_LetterSpacing;
	m_WordSpacing = other->m_WordSpacing;
	m_TextTransform = other->m_TextTransform;
	m_LineHeight = other->m_LineHeight;
}
void IfcTextStyleTextModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTSTYLETEXTMODEL" << "(";
	if( m_TextIndent ) { m_TextIndent->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TextAlign ) { m_TextAlign->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TextDecoration ) { m_TextDecoration->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LetterSpacing ) { m_LetterSpacing->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_WordSpacing ) { m_WordSpacing->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ",";
	if( m_TextTransform ) { m_TextTransform->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LineHeight ) { m_LineHeight->getStepParameter( stream, true ); } else { stream << "$"; }
	stream << ");";
}
void IfcTextStyleTextModel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyleTextModel::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<7 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextStyleTextModel, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>7 ){ std::cout << "Wrong parameter count for entity IfcTextStyleTextModel, expecting 7, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_TextIndent = IfcSizeSelect::createObjectFromStepData( args[0], map );
	m_TextAlign = IfcTextAlignment::createObjectFromStepData( args[1] );
	m_TextDecoration = IfcTextDecoration::createObjectFromStepData( args[2] );
	m_LetterSpacing = IfcSizeSelect::createObjectFromStepData( args[3], map );
	m_WordSpacing = IfcSizeSelect::createObjectFromStepData( args[4], map );
	m_TextTransform = IfcTextTransformation::createObjectFromStepData( args[5] );
	m_LineHeight = IfcSizeSelect::createObjectFromStepData( args[6], map );
}
void IfcTextStyleTextModel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TextIndent", m_TextIndent ) );
	vec_attributes.push_back( std::make_pair( "TextAlign", m_TextAlign ) );
	vec_attributes.push_back( std::make_pair( "TextDecoration", m_TextDecoration ) );
	vec_attributes.push_back( std::make_pair( "LetterSpacing", m_LetterSpacing ) );
	vec_attributes.push_back( std::make_pair( "WordSpacing", m_WordSpacing ) );
	vec_attributes.push_back( std::make_pair( "TextTransform", m_TextTransform ) );
	vec_attributes.push_back( std::make_pair( "LineHeight", m_LineHeight ) );
}
void IfcTextStyleTextModel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTextStyleTextModel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyleTextModel::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
