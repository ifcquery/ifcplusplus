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
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcLabel.h"
#include "include/IfcSurfaceSide.h"
#include "include/IfcSurfaceStyle.h"
#include "include/IfcSurfaceStyleElementSelect.h"

// ENTITY IfcSurfaceStyle 
IfcSurfaceStyle::IfcSurfaceStyle() { m_entity_enum = IFCSURFACESTYLE; }
IfcSurfaceStyle::IfcSurfaceStyle( int id ) { m_id = id; m_entity_enum = IFCSURFACESTYLE; }
IfcSurfaceStyle::~IfcSurfaceStyle() {}
shared_ptr<IfcPPObject> IfcSurfaceStyle::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyle> copy_self( new IfcSurfaceStyle() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	if( m_Side ) { copy_self->m_Side = dynamic_pointer_cast<IfcSurfaceSide>( m_Side->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Styles.size(); ++ii )
	{
		auto item_ii = m_Styles[ii];
		if( item_ii )
		{
			copy_self->m_Styles.push_back( dynamic_pointer_cast<IfcSurfaceStyleElementSelect>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcSurfaceStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACESTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Side ) { m_Side->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_Styles.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcSurfaceStyleElementSelect>& type_object = m_Styles[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, true );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ");";
}
void IfcSurfaceStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyle::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyle, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
	m_Side = IfcSurfaceSide::createObjectFromSTEP( args[1] );
	readSelectList( args[2], m_Styles, map );
}
void IfcSurfaceStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationStyle::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Side", m_Side ) );
	if( m_Styles.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Styles_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Styles.begin(), m_Styles.end(), std::back_inserter( Styles_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Styles", Styles_vec_object ) );
	}
}
void IfcSurfaceStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationStyle::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyle::unlinkFromInverseCounterparts()
{
	IfcPresentationStyle::unlinkFromInverseCounterparts();
}
