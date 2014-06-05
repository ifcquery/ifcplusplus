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
#include "include/IfcSurfaceTexture.h"
#include "include/IfcTextureCoordinate.h"

// ENTITY IfcTextureCoordinate 
IfcTextureCoordinate::IfcTextureCoordinate() {}
IfcTextureCoordinate::IfcTextureCoordinate( int id ) { m_id = id; }
IfcTextureCoordinate::~IfcTextureCoordinate() {}

// method setEntity takes over all attributes from another instance of the class
void IfcTextureCoordinate::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcTextureCoordinate> other = dynamic_pointer_cast<IfcTextureCoordinate>(other_entity);
	if( !other) { return; }
	m_Maps = other->m_Maps;
}
void IfcTextureCoordinate::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCTEXTURECOORDINATE" << "(";
	writeEntityList( stream, m_Maps );
	stream << ");";
}
void IfcTextureCoordinate::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureCoordinate::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args<1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextureCoordinate, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	#ifdef _DEBUG
	if( num_args>1 ){ std::cout << "Wrong parameter count for entity IfcTextureCoordinate, expecting 1, having " << num_args << ". Object id: " << getId() << std::endl; }
	#endif
	readEntityReferenceList( args[0], m_Maps, map );
}
void IfcTextureCoordinate::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
}
void IfcTextureCoordinate::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcTextureCoordinate::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
	shared_ptr<IfcTextureCoordinate> ptr_self = dynamic_pointer_cast<IfcTextureCoordinate>( ptr_self_entity );
	if( !ptr_self ) { throw IfcPPException( "IfcTextureCoordinate::setInverseCounterparts: type mismatch" ); }
	for( int i=0; i<m_Maps.size(); ++i )
	{
		if( m_Maps[i] )
		{
			m_Maps[i]->m_IsMappedBy_inverse.push_back( ptr_self );
		}
	}
}
void IfcTextureCoordinate::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
	for( int i=0; i<m_Maps.size(); ++i )
	{
		if( m_Maps[i] )
		{
			std::vector<weak_ptr<IfcTextureCoordinate> >& IsMappedBy_inverse = m_Maps[i]->m_IsMappedBy_inverse;
			std::vector<weak_ptr<IfcTextureCoordinate> >::iterator it_IsMappedBy_inverse;
			for( it_IsMappedBy_inverse = IsMappedBy_inverse.begin(); it_IsMappedBy_inverse != IsMappedBy_inverse.end(); ++it_IsMappedBy_inverse)
			{
				shared_ptr<IfcTextureCoordinate> self_candidate( *it_IsMappedBy_inverse );
				if( self_candidate->getId() == this->getId() )
				{
					IsMappedBy_inverse.erase( it_IsMappedBy_inverse );
					break;
				}
			}
		}
	}
}
