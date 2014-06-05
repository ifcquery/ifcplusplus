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
#include "include/IfcColourRgbList.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcSurfaceStyleShading.h"
#include "include/IfcTessellatedFaceSet.h"

// ENTITY IfcIndexedColourMap 
IfcIndexedColourMap::IfcIndexedColourMap() {}
IfcIndexedColourMap::IfcIndexedColourMap( int id ) { m_id = id; }
IfcIndexedColourMap::~IfcIndexedColourMap() {}

// method setEntity takes over all attributes from another instance of the class
void IfcIndexedColourMap::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcIndexedColourMap> other = dynamic_pointer_cast<IfcIndexedColourMap>(other_entity);
	if( !other) { return; }
	m_MappedTo = other->m_MappedTo;
	m_Overrides = other->m_Overrides;
	m_Colours = other->m_Colours;
	m_ColourIndex = other->m_ColourIndex;
}
void IfcIndexedColourMap::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCINDEXEDCOLOURMAP" << "(";
	if( m_MappedTo ) { stream << "#" << m_MappedTo->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Overrides ) { stream << "#" << m_Overrides->getId(); } else { stream << "$"; }
	stream << ",";
	if( m_Colours ) { stream << "#" << m_Colours->getId(); } else { stream << "$"; }
	stream << ",";
	writeIntList( stream, m_ColourIndex );
	stream << ");";
}
void IfcIndexedColourMap::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedColourMap::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<4 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcIndexedColourMap, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>4 ){ std::cout << "Wrong parameter count for entity IfcIndexedColourMap, expecting 4, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReference( args[0], m_MappedTo, map );
	readEntityReference( args[1], m_Overrides, map );
	readEntityReference( args[2], m_Colours, map );
	readIntList(  args[3], m_ColourIndex );
}
void IfcIndexedColourMap::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "MappedTo", m_MappedTo ) );
	vec_attributes.push_back( std::make_pair( "Overrides", m_Overrides ) );
	vec_attributes.push_back( std::make_pair( "Colours", m_Colours ) );
}
void IfcIndexedColourMap::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcIndexedColourMap::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcIndexedColourMap> ptr_self = dynamic_pointer_cast<IfcIndexedColourMap>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcIndexedColourMap::setInverseCounterparts: type mismatch" ); }
	if( m_MappedTo )
	{
		m_MappedTo->m_HasColours_inverse.push_back( ptr_self );
	}
}
void IfcIndexedColourMap::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
	if( m_MappedTo )
	{
		std::vector<weak_ptr<IfcIndexedColourMap> >& HasColours_inverse = m_MappedTo->m_HasColours_inverse;
		std::vector<weak_ptr<IfcIndexedColourMap> >::iterator it_HasColours_inverse;
		for( it_HasColours_inverse = HasColours_inverse.begin(); it_HasColours_inverse != HasColours_inverse.end(); ++it_HasColours_inverse)
		{
			shared_ptr<IfcIndexedColourMap> self_candidate( *it_HasColours_inverse );
			if( self_candidate->getId() == this->getId() )
			{
				HasColours_inverse.erase( it_HasColours_inverse );
				break;
			}
		}
	}
}
