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
#include "include/IfcSurfaceSide.h"
#include "include/IfcSurfaceStyle.h"
#include "include/IfcSurfaceStyleElementSelect.h"

// ENTITY IfcSurfaceStyle 
IfcSurfaceStyle::IfcSurfaceStyle() {}
IfcSurfaceStyle::IfcSurfaceStyle( int id ) { m_id = id; }
IfcSurfaceStyle::~IfcSurfaceStyle() {}

// method setEntity takes over all attributes from another instance of the class
void IfcSurfaceStyle::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcSurfaceStyle> other = dynamic_pointer_cast<IfcSurfaceStyle>(other_entity);
	if( !other) { return; }
	m_Name = other->m_Name;
	m_Side = other->m_Side;
	m_Styles = other->m_Styles;
}
void IfcSurfaceStyle::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCSURFACESTYLE" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_Side ) { m_Side->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	writeTypeList( stream, m_Styles, true );
	stream << ");";
}
void IfcSurfaceStyle::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyle::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcSurfaceStyle, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcSurfaceStyle, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	m_Name = IfcLabel::createObjectFromStepData( args[0] );
	m_Side = IfcSurfaceSide::createObjectFromStepData( args[1] );
	readSelectList( args[2], m_Styles, map );
}
void IfcSurfaceStyle::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationStyle::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Side", m_Side ) );
	shared_ptr<IfcPPAttributeObjectVector> Styles_vec_object( new  IfcPPAttributeObjectVector() );
	std::copy( m_Styles.begin(), m_Styles.end(), std::back_inserter( Styles_vec_object->m_vec ) );
	vec_attributes.push_back( std::make_pair( "Styles", Styles_vec_object ) );
}
void IfcSurfaceStyle::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcSurfaceStyle::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationStyle::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyle::unlinkSelf()
{
	IfcPresentationStyle::unlinkSelf();
}
