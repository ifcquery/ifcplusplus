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
#include "include/IfcParameterValue.h"
#include "include/IfcTextureVertex.h"

// ENTITY IfcTextureVertex 
IfcTextureVertex::IfcTextureVertex() {}
IfcTextureVertex::IfcTextureVertex( int id ) { m_id = id; }
IfcTextureVertex::~IfcTextureVertex() {}
shared_ptr<IfcPPObject> IfcTextureVertex::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTextureVertex> copy_self( new IfcTextureVertex() );
	for( size_t ii=0; ii<m_Coordinates.size(); ++ii )
	{
		auto item_ii = m_Coordinates[ii];
		if( item_ii )
		{
			copy_self->m_Coordinates.push_back( dynamic_pointer_cast<IfcParameterValue>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTextureVertex::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTEXTUREVERTEX" << "(";
	writeTypeOfRealList( stream, m_Coordinates );
	stream << ");";
}
void IfcTextureVertex::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTextureVertex::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTextureVertex, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readTypeOfRealList( args[0], m_Coordinates );
}
void IfcTextureVertex::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	if( m_Coordinates.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Coordinates_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Coordinates.begin(), m_Coordinates.end(), std::back_inserter( Coordinates_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Coordinates", Coordinates_vec_object ) );
	}
}
void IfcTextureVertex::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcTextureVertex::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcTextureVertex::unlinkSelf()
{
	IfcPresentationItem::unlinkSelf();
}
