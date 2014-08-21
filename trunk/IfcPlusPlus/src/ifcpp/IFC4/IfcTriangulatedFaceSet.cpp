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
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTriangulatedFaceSet.h"

// ENTITY IfcTriangulatedFaceSet 
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet() {}
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet( int id ) { m_id = id; }
IfcTriangulatedFaceSet::~IfcTriangulatedFaceSet() {}
shared_ptr<IfcPPObject> IfcTriangulatedFaceSet::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcTriangulatedFaceSet> copy_self( new IfcTriangulatedFaceSet() );
	if( m_Coordinates ) { copy_self->m_Coordinates = dynamic_pointer_cast<IfcCartesianPointList3D>( m_Coordinates->getDeepCopy(options) ); }
	copy_self->m_Normals.resize( m_Normals.size() );
	for( size_t ii=0; ii<m_Normals.size(); ++ii )
	{
		std::vector<shared_ptr<IfcParameterValue> >& vec_ii = m_Normals[ii];
		std::vector<shared_ptr<IfcParameterValue> >& vec_ii_target = copy_self->m_Normals[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcParameterValue>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcParameterValue>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	if( m_Closed ) { copy_self->m_Closed = m_Closed; }
	if( m_CoordIndex.size() > 0 )
	{
		copy_self->m_CoordIndex.resize( m_CoordIndex.size() );
		for( size_t i = 0; i < m_CoordIndex.size(); ++i )
		{
			std::copy( m_CoordIndex[i].begin(), m_CoordIndex[i].end(), std::back_inserter( copy_self->m_CoordIndex[i] ) );
		}
	}
	if( m_NormalIndex.size() > 0 )
	{
		copy_self->m_NormalIndex.resize( m_NormalIndex.size() );
		for( size_t i = 0; i < m_NormalIndex.size(); ++i )
		{
			std::copy( m_NormalIndex[i].begin(), m_NormalIndex[i].end(), std::back_inserter( copy_self->m_NormalIndex[i] ) );
		}
	}
	return copy_self;
}
void IfcTriangulatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTRIANGULATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->m_id; } else { stream << "*"; }
	stream << ",";
	writeTypeOfRealList2D( stream, m_Normals );
	stream << ",";
	if( m_Closed == false ) { stream << ".F."; }
	else if( m_Closed == true ) { stream << ".T."; }
	stream << ",";
	writeIntList2D( stream, m_CoordIndex );
	stream << ",";
	writeIntList2D( stream, m_NormalIndex );
	stream << ");";
}
void IfcTriangulatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTriangulatedFaceSet::readStepArguments( const std::vector<std::wstring>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream strserr; strserr << "Wrong parameter count for entity IfcTriangulatedFaceSet, expecting 5, having " << num_args << ". Object id: " << m_id << std::endl; throw IfcPPException( strserr.str().c_str() ); }
	readEntityReference( args[0], m_Coordinates, map );
	readTypeOfRealList2D( args[1], m_Normals );
	if( boost::iequals( args[2], L".F." ) ) { m_Closed = false; }
	else if( boost::iequals( args[2], L".T." ) ) { m_Closed = true; }
	readIntList2D(  args[3], m_CoordIndex );
	readIntList2D(  args[4], m_NormalIndex );
}
void IfcTriangulatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedFaceSet::getAttributes( vec_attributes );
}
void IfcTriangulatedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTessellatedFaceSet::getAttributesInverse( vec_attributes_inverse );
}
void IfcTriangulatedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTessellatedFaceSet::setInverseCounterparts( ptr_self_entity );
}
void IfcTriangulatedFaceSet::unlinkSelf()
{
	IfcTessellatedFaceSet::unlinkSelf();
}
