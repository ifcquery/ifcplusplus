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
#include "include/IfcCartesianPoint.h"
#include "include/IfcPolyLoop.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPolyLoop 
IfcPolyLoop::IfcPolyLoop() {}
IfcPolyLoop::IfcPolyLoop( int id ) { m_id = id; }
IfcPolyLoop::~IfcPolyLoop() {}
shared_ptr<IfcPPObject> IfcPolyLoop::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPolyLoop> copy_self( new IfcPolyLoop() );
	for( size_t ii=0; ii<m_Polygon.size(); ++ii )
	{
		auto item_ii = m_Polygon[ii];
		if( item_ii )
		{
			copy_self->m_Polygon.push_back( dynamic_pointer_cast<IfcCartesianPoint>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcPolyLoop::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPOLYLOOP" << "(";
	writeEntityList( stream, m_Polygon );
	stream << ");";
}
void IfcPolyLoop::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPolyLoop::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcPolyLoop, expecting 1, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReferenceList( args[0], m_Polygon, map );
}
void IfcPolyLoop::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLoop::getAttributes( vec_attributes );
	if( m_Polygon.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Polygon_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Polygon.begin(), m_Polygon.end(), std::back_inserter( Polygon_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Polygon", Polygon_vec_object ) );
	}
}
void IfcPolyLoop::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcLoop::getAttributesInverse( vec_attributes_inverse );
}
void IfcPolyLoop::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLoop::setInverseCounterparts( ptr_self_entity );
}
void IfcPolyLoop::unlinkSelf()
{
	IfcLoop::unlinkSelf();
}
