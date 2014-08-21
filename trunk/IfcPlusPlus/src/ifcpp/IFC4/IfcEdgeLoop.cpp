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
#include "include/IfcEdgeLoop.h"
#include "include/IfcOrientedEdge.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcEdgeLoop 
IfcEdgeLoop::IfcEdgeLoop() {}
IfcEdgeLoop::IfcEdgeLoop( int id ) { m_id = id; }
IfcEdgeLoop::~IfcEdgeLoop() {}
shared_ptr<IfcPPObject> IfcEdgeLoop::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcEdgeLoop> copy_self( new IfcEdgeLoop() );
	for( size_t ii=0; ii<m_EdgeList.size(); ++ii )
	{
		auto item_ii = m_EdgeList[ii];
		if( item_ii )
		{
			copy_self->m_EdgeList.push_back( dynamic_pointer_cast<IfcOrientedEdge>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcEdgeLoop::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCEDGELOOP" << "(";
	writeEntityList( stream, m_EdgeList );
	stream << ");";
}
void IfcEdgeLoop::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcEdgeLoop::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcEdgeLoop, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_EdgeList, map );
}
void IfcEdgeLoop::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLoop::getAttributes( vec_attributes );
	if( m_EdgeList.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> EdgeList_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_EdgeList.begin(), m_EdgeList.end(), std::back_inserter( EdgeList_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "EdgeList", EdgeList_vec_object ) );
	}
}
void IfcEdgeLoop::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcLoop::getAttributesInverse( vec_attributes_inverse );
}
void IfcEdgeLoop::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLoop::setInverseCounterparts( ptr_self_entity );
}
void IfcEdgeLoop::unlinkSelf()
{
	IfcLoop::unlinkSelf();
}
