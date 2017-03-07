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
#include "include/IfcSizeSelect.h"
#include "include/IfcTextAlignment.h"
#include "include/IfcTextDecoration.h"
#include "include/IfcTextStyleTextModel.h"
#include "include/IfcTextTransformation.h"

// ENTITY IfcTextStyleTextModel 
IfcTextStyleTextModel::IfcTextStyleTextModel() { m_entity_enum = IFCTEXTSTYLETEXTMODEL; }
IfcTextStyleTextModel::IfcTextStyleTextModel( int id ) { m_id = id; m_entity_enum = IFCTEXTSTYLETEXTMODEL; }
IfcTextStyleTextModel::~IfcTextStyleTextModel() {}
shared_ptr<IfcPPObject> IfcTextStyleTextModel::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextStyleTextModel> copy_self( new IfcTextStyleTextModel() );
	if( m_TextIndent ) { copy_self->m_TextIndent = dynamic_pointer_cast<IfcSizeSelect>( m_TextIndent->getDeepCopy(options) ); }
	if( m_TextAlign ) { copy_self->m_TextAlign = dynamic_pointer_cast<IfcTextAlignment>( m_TextAlign->getDeepCopy(options) ); }
	if( m_TextDecoration ) { copy_self->m_TextDecoration = dynamic_pointer_cast<IfcTextDecoration>( m_TextDecoration->getDeepCopy(options) ); }
	if( m_LetterSpacing ) { copy_self->m_LetterSpacing = dynamic_pointer_cast<IfcSizeSelect>( m_LetterSpacing->getDeepCopy(options) ); }
	if( m_WordSpacing ) { copy_self->m_WordSpacing = dynamic_pointer_cast<IfcSizeSelect>( m_WordSpacing->getDeepCopy(options) ); }
	if( m_TextTransform ) { copy_self->m_TextTransform = dynamic_pointer_cast<IfcTextTransformation>( m_TextTransform->getDeepCopy(options) ); }
	if( m_LineHeight ) { copy_self->m_LineHeight = dynamic_pointer_cast<IfcSizeSelect>( m_LineHeight->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTextStyleTextModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTSTYLETEXTMODEL" << "(";
	if( m_TextIndent ) { m_TextIndent->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TextAlign ) { m_TextAlign->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_TextDecoration ) { m_TextDecoration->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LetterSpacing ) { m_LetterSpacing->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_WordSpacing ) { m_WordSpacing->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ",";
	if( m_TextTransform ) { m_TextTransform->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_LineHeight ) { m_LineHeight->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcTextStyleTextModel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyleTextModel::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 7 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextStyleTextModel, expecting 7, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_TextIndent = IfcSizeSelect::createObjectFromSTEP( args[0], map );
	m_TextAlign = IfcTextAlignment::createObjectFromSTEP( args[1] );
	m_TextDecoration = IfcTextDecoration::createObjectFromSTEP( args[2] );
	m_LetterSpacing = IfcSizeSelect::createObjectFromSTEP( args[3], map );
	m_WordSpacing = IfcSizeSelect::createObjectFromSTEP( args[4], map );
	m_TextTransform = IfcTextTransformation::createObjectFromSTEP( args[5] );
	m_LineHeight = IfcSizeSelect::createObjectFromSTEP( args[6], map );
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
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextStyleTextModel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyleTextModel::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
