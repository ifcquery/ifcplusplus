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
#include "include/IfcFontStyle.h"
#include "include/IfcFontVariant.h"
#include "include/IfcFontWeight.h"
#include "include/IfcLabel.h"
#include "include/IfcSizeSelect.h"
#include "include/IfcTextFontName.h"
#include "include/IfcTextStyleFontModel.h"

// ENTITY IfcTextStyleFontModel 
IfcTextStyleFontModel::IfcTextStyleFontModel() { m_entity_enum = IFCTEXTSTYLEFONTMODEL; }
IfcTextStyleFontModel::IfcTextStyleFontModel( int id ) { m_id = id; m_entity_enum = IFCTEXTSTYLEFONTMODEL; }
IfcTextStyleFontModel::~IfcTextStyleFontModel() {}
shared_ptr<IfcPPObject> IfcTextStyleFontModel::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextStyleFontModel> copy_self( new IfcTextStyleFontModel() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_FontFamily.size(); ++ii )
	{
		auto item_ii = m_FontFamily[ii];
		if( item_ii )
		{
			copy_self->m_FontFamily.push_back( dynamic_pointer_cast<IfcTextFontName>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_FontStyle ) { copy_self->m_FontStyle = dynamic_pointer_cast<IfcFontStyle>( m_FontStyle->getDeepCopy(options) ); }
	if( m_FontVariant ) { copy_self->m_FontVariant = dynamic_pointer_cast<IfcFontVariant>( m_FontVariant->getDeepCopy(options) ); }
	if( m_FontWeight ) { copy_self->m_FontWeight = dynamic_pointer_cast<IfcFontWeight>( m_FontWeight->getDeepCopy(options) ); }
	if( m_FontSize ) { copy_self->m_FontSize = dynamic_pointer_cast<IfcSizeSelect>( m_FontSize->getDeepCopy(options) ); }
	return copy_self;
}
void IfcTextStyleFontModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTSTYLEFONTMODEL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_FontFamily.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcTextFontName>& type_object = m_FontFamily[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	if( m_FontStyle ) { m_FontStyle->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FontVariant ) { m_FontVariant->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FontWeight ) { m_FontWeight->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_FontSize ) { m_FontSize->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcTextStyleFontModel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextStyleFontModel::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 6 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTextStyleFontModel, expecting 6, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	readSelectList( args[1], m_FontFamily, map );
	m_FontStyle = IfcFontStyle::createObjectFromSTEP( args[2] );
	m_FontVariant = IfcFontVariant::createObjectFromSTEP( args[3] );
	m_FontWeight = IfcFontWeight::createObjectFromSTEP( args[4] );
	m_FontSize = IfcSizeSelect::createObjectFromSTEP( args[5], map );
}
void IfcTextStyleFontModel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPreDefinedTextFont::getAttributes( vec_attributes );
	if( m_FontFamily.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> FontFamily_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_FontFamily.begin(), m_FontFamily.end(), std::back_inserter( FontFamily_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "FontFamily", FontFamily_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "FontStyle", m_FontStyle ) );
	vec_attributes.push_back( std::make_pair( "FontVariant", m_FontVariant ) );
	vec_attributes.push_back( std::make_pair( "FontWeight", m_FontWeight ) );
	vec_attributes.push_back( std::make_pair( "FontSize", m_FontSize ) );
}
void IfcTextStyleFontModel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPreDefinedTextFont::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextStyleFontModel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPreDefinedTextFont::setInverseCounterparts( ptr_self_entity );
}
void IfcTextStyleFontModel::unlinkFromInverseCounterparts()
{
	IfcPreDefinedTextFont::unlinkFromInverseCounterparts();
}
