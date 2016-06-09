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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcShell.h"
#include "include/IfcShellBasedSurfaceModel.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcShellBasedSurfaceModel 
IfcShellBasedSurfaceModel::IfcShellBasedSurfaceModel() { m_entity_enum = IFCSHELLBASEDSURFACEMODEL; }
IfcShellBasedSurfaceModel::IfcShellBasedSurfaceModel( int id ) { m_id = id; m_entity_enum = IFCSHELLBASEDSURFACEMODEL; }
IfcShellBasedSurfaceModel::~IfcShellBasedSurfaceModel() {}
shared_ptr<IfcPPObject> IfcShellBasedSurfaceModel::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcShellBasedSurfaceModel> copy_self( new IfcShellBasedSurfaceModel() );
	for( size_t ii=0; ii<m_SbsmBoundary.size(); ++ii )
	{
		auto item_ii = m_SbsmBoundary[ii];
		if( item_ii )
		{
			copy_self->m_SbsmBoundary.push_back( dynamic_pointer_cast<IfcShell>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcShellBasedSurfaceModel::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSHELLBASEDSURFACEMODEL" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_SbsmBoundary.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcShell>& type_object = m_SbsmBoundary[ii];
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
void IfcShellBasedSurfaceModel::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcShellBasedSurfaceModel::readStepArguments( const std::vector<std::wstring>& args, const boost::unordered_map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcShellBasedSurfaceModel, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_SbsmBoundary, map );
}
void IfcShellBasedSurfaceModel::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	if( m_SbsmBoundary.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> SbsmBoundary_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_SbsmBoundary.begin(), m_SbsmBoundary.end(), std::back_inserter( SbsmBoundary_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "SbsmBoundary", SbsmBoundary_vec_object ) );
	}
}
void IfcShellBasedSurfaceModel::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcShellBasedSurfaceModel::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcShellBasedSurfaceModel::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
