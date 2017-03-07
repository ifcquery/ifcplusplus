/* -*-c++-*- IFC++ www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcIndexedPolygonalFaceWithVoids.h"
#include "include/IfcPolygonalFaceSet.h"
#include "include/IfcPositiveInteger.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcIndexedPolygonalFaceWithVoids 
IfcIndexedPolygonalFaceWithVoids::IfcIndexedPolygonalFaceWithVoids() { m_entity_enum = IFCINDEXEDPOLYGONALFACEWITHVOIDS; }
IfcIndexedPolygonalFaceWithVoids::IfcIndexedPolygonalFaceWithVoids( int id ) { m_id = id; m_entity_enum = IFCINDEXEDPOLYGONALFACEWITHVOIDS; }
IfcIndexedPolygonalFaceWithVoids::~IfcIndexedPolygonalFaceWithVoids() {}
shared_ptr<IfcPPObject> IfcIndexedPolygonalFaceWithVoids::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcIndexedPolygonalFaceWithVoids> copy_self( new IfcIndexedPolygonalFaceWithVoids() );
	for( size_t ii=0; ii<m_CoordIndex.size(); ++ii )
	{
		auto item_ii = m_CoordIndex[ii];
		if( item_ii )
		{
			copy_self->m_CoordIndex.push_back( dynamic_pointer_cast<IfcPositiveInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	copy_self->m_InnerCoordIndices.resize( m_InnerCoordIndices.size() );
	for( size_t ii=0; ii<m_InnerCoordIndices.size(); ++ii )
	{
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii = m_InnerCoordIndices[ii];
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii_target = copy_self->m_InnerCoordIndices[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcPositiveInteger>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcPositiveInteger>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	return copy_self;
}
void IfcIndexedPolygonalFaceWithVoids::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCINDEXEDPOLYGONALFACEWITHVOIDS" << "(";
	stream << "(";
	for( size_t ii = 0; ii < m_CoordIndex.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcPositiveInteger>& type_object = m_CoordIndex[ii];
		if( type_object )
		{
			type_object->getStepParameter( stream, false );
		}
		else
		{
			stream << "$";
		}
	}
	stream << ")";
	stream << ",";
	stream << "("; 
	for( size_t ii = 0; ii < m_InnerCoordIndices.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcPositiveInteger> >&inner_vec = m_InnerCoordIndices[ii];
		if( ii > 0 )
		{
			stream << "), (";
		}
		for( size_t jj = 0; jj < inner_vec.size(); ++jj )
		{
			if( jj > 0 )
			{
				stream << ", ";
			}
			const shared_ptr<IfcPositiveInteger>& type_object = inner_vec[jj];
			if( type_object )
			{
				type_object->getStepParameter( stream, false );
			}
			else
			{
				stream << "$";
			}
		}
	}
	stream << ")"; 
	stream << ");";
}
void IfcIndexedPolygonalFaceWithVoids::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcIndexedPolygonalFaceWithVoids::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcIndexedPolygonalFaceWithVoids, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readSelectList( args[0], m_CoordIndex, map );
	readEntityReferenceList2D( args[1], m_InnerCoordIndices, map );
}
void IfcIndexedPolygonalFaceWithVoids::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcIndexedPolygonalFace::getAttributes( vec_attributes );
}
void IfcIndexedPolygonalFaceWithVoids::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcIndexedPolygonalFace::getAttributesInverse( vec_attributes_inverse );
}
void IfcIndexedPolygonalFaceWithVoids::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcIndexedPolygonalFace::setInverseCounterparts( ptr_self_entity );
}
void IfcIndexedPolygonalFaceWithVoids::unlinkFromInverseCounterparts()
{
	IfcIndexedPolygonalFace::unlinkFromInverseCounterparts();
}
