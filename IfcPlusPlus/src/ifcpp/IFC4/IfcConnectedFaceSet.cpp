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
#include "include/IfcConnectedFaceSet.h"
#include "include/IfcFace.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcConnectedFaceSet 
IfcConnectedFaceSet::IfcConnectedFaceSet() {}
IfcConnectedFaceSet::IfcConnectedFaceSet( int id ) { m_id = id; }
IfcConnectedFaceSet::~IfcConnectedFaceSet() {}
shared_ptr<IfcPPObject> IfcConnectedFaceSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcConnectedFaceSet> copy_self( new IfcConnectedFaceSet() );
	for( size_t ii=0; ii<m_CfsFaces.size(); ++ii )
	{
		auto item_ii = m_CfsFaces[ii];
		if( item_ii )
		{
			copy_self->m_CfsFaces.push_back( dynamic_pointer_cast<IfcFace>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcConnectedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCONNECTEDFACESET" << "(";
	writeEntityList( stream, m_CfsFaces );
	stream << ");";
}
void IfcConnectedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcConnectedFaceSet::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcConnectedFaceSet, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_CfsFaces, map );
}
void IfcConnectedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
	if( m_CfsFaces.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> CfsFaces_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_CfsFaces.begin(), m_CfsFaces.end(), std::back_inserter( CfsFaces_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "CfsFaces", CfsFaces_vec_object ) );
	}
}
void IfcConnectedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTopologicalRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcConnectedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcConnectedFaceSet::unlinkSelf()
{
	IfcTopologicalRepresentationItem::unlinkSelf();
}
