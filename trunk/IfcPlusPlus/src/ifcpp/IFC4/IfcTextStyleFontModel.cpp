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
#include "include/IfcFontStyle.h"
#include "include/IfcFontVariant.h"
#include "include/IfcFontWeight.h"
#include "include/IfcLabel.h"
#include "include/IfcSizeSelect.h"
#include "include/IfcTextFontName.h"
#include "include/IfcTextStyleFontModel.h"

// ENTITY IfcTextStyleFontModel 
IfcTextStyleFontModel::IfcTextStyleFontModel() {}
IfcTextStyleFontModel::IfcTextStyleFontModel( int id ) { m_id = id; }
IfcTextStyleFontModel::~IfcTextStyleFontModel() {}
shared_ptr<IfcPPObject> IfcTextStyleFontModel::getDeepCopy()
{
	shared_ptr<IfcTextStyleFontModel> copy_self( new IfcTextStyleFontModel() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy() ); }
	for( size_t ii=0; ii<m_FontFamily.size(); ++ii )
	{
		auto item_ii = m_FontFamily[ii];
		if( item_ii )
		{
			copy_self->m_FontFamily.push_back( dynamic_pointer_cast<IfcTextFontName>(item_ii->getDeepCopy() ) );
		}
	}
	if( m_FontStyle ) { copy_self->m_FontStyle = dynamic_pointer_cast<IfcFontStyle>( m_FontStyle->getDeepCopy() ); }
	if( m_FontVariant ) { copy_self->m_FontVariant = dynamic_pointer_cast<IfcFontVariant>( m_FontVariant->getDeepCopy() ); }
	if( m_FontWeight ) { copy_self->m_FontWeight = dynamic_pointer_cast<IfcFontWeight>( m_FontWeight->getDeepCopy() ); }
	if( m_FontSize ) { copy_self->m_FontSize = dynamic_pointer_cast<IfcSizeSelect>( m_FontSize->getDeepCopy() ); }
	return copy_self;
}
void IfcTextStyleFontModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTSTYLEFONTMODEL" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeTypeList( stream, m_FontFamily );
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
void IfcTextStyleFontModel::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<6 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextStyleFontModel, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>6 ){ std::cout << "Wrong parameter count for entity IfcTextStyleFontModel, expecting 6, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	readTypeList( args[1], m_FontFamily );
	m_FontStyle = IfcFontStyle::createObjectFromStepData( args[2] );
	m_FontVariant = IfcFontVariant::createObjectFromStepData( args[3] );
	m_FontWeight = IfcFontWeight::createObjectFromStepData( args[4] );
	m_FontSize = IfcSizeSelect::createObjectFromStepData( args[5], map );
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
void IfcTextStyleFontModel::unlinkSelf()
{
	IfcPreDefinedTextFont::unlinkSelf();
}
