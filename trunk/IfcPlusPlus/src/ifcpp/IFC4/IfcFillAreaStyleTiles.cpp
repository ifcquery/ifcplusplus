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
#include "include/IfcFillAreaStyleTiles.h"
#include "include/IfcPositiveRatioMeasure.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVector.h"

// ENTITY IfcFillAreaStyleTiles 
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles() {}
IfcFillAreaStyleTiles::IfcFillAreaStyleTiles( int id ) { m_id = id; }
IfcFillAreaStyleTiles::~IfcFillAreaStyleTiles() {}

// method setEntity takes over all attributes from another instance of the class
void IfcFillAreaStyleTiles::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcFillAreaStyleTiles> other = dynamic_pointer_cast<IfcFillAreaStyleTiles>(other_entity);
	if( !other) { return; }
	m_TilingPattern = other->m_TilingPattern;
	m_Tiles = other->m_Tiles;
	m_TilingScale = other->m_TilingScale;
}
void IfcFillAreaStyleTiles::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCFILLAREASTYLETILES" << "(";
	writeEntityList( stream, m_TilingPattern );
	stream << ",";
	writeEntityList( stream, m_Tiles );
	stream << ",";
	if( m_TilingScale ) { m_TilingScale->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFillAreaStyleTiles::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFillAreaStyleTiles::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<3 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcFillAreaStyleTiles, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>3 ){ std::cout << "Wrong parameter count for entity IfcFillAreaStyleTiles, expecting 3, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_TilingPattern, map );
	readEntityReferenceList( args[1], m_Tiles, map );
	m_TilingScale = IfcPositiveRatioMeasure::createObjectFromStepData( args[2] );
}
void IfcFillAreaStyleTiles::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "TilingScale", m_TilingScale ) );
}
void IfcFillAreaStyleTiles::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcFillAreaStyleTiles::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFillAreaStyleTiles::unlinkSelf()
{
	IfcGeometricRepresentationItem::unlinkSelf();
}
