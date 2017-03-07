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
#include "include/IfcBoolean.h"
#include "include/IfcCartesianPointList3D.h"
#include "include/IfcIndexedColourMap.h"
#include "include/IfcIndexedTextureMap.h"
#include "include/IfcParameterValue.h"
#include "include/IfcPositiveInteger.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcTriangulatedFaceSet.h"

// ENTITY IfcTriangulatedFaceSet 
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet() { m_entity_enum = IFCTRIANGULATEDFACESET; }
IfcTriangulatedFaceSet::IfcTriangulatedFaceSet( int id ) { m_id = id; m_entity_enum = IFCTRIANGULATEDFACESET; }
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
	if( m_Closed ) { copy_self->m_Closed = dynamic_pointer_cast<IfcBoolean>( m_Closed->getDeepCopy(options) ); }
	copy_self->m_CoordIndex.resize( m_CoordIndex.size() );
	for( size_t ii=0; ii<m_CoordIndex.size(); ++ii )
	{
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii = m_CoordIndex[ii];
		std::vector<shared_ptr<IfcPositiveInteger> >& vec_ii_target = copy_self->m_CoordIndex[ii];
		for( size_t jj=0; jj<vec_ii.size(); ++jj )
		{
			shared_ptr<IfcPositiveInteger>& item_jj = vec_ii[jj];
			if( item_jj )
			{
				vec_ii_target.push_back( dynamic_pointer_cast<IfcPositiveInteger>( item_jj->getDeepCopy(options) ) );
			}
		}
	}
	for( size_t ii=0; ii<m_PnIndex.size(); ++ii )
	{
		auto item_ii = m_PnIndex[ii];
		if( item_ii )
		{
			copy_self->m_PnIndex.push_back( dynamic_pointer_cast<IfcPositiveInteger>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcTriangulatedFaceSet::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCTRIANGULATEDFACESET" << "(";
	if( m_Coordinates ) { stream << "#" << m_Coordinates->m_id; } else { stream << "*"; }
	stream << ",";
	writeNumericTypeList2D( stream, m_Normals );
	stream << ",";
	if( m_Closed ) { m_Closed->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	stream << "("; 
	for( size_t ii = 0; ii < m_CoordIndex.size(); ++ii )
	{
		const std::vector<shared_ptr<IfcPositiveInteger> >&inner_vec = m_CoordIndex[ii];
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
	stream << ",";
	stream << "(";
	for( size_t ii = 0; ii < m_PnIndex.size(); ++ii )
	{
		if( ii > 0 )
		{
			stream << ",";
		}
		const shared_ptr<IfcPositiveInteger>& type_object = m_PnIndex[ii];
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
	stream << ");";
}
void IfcTriangulatedFaceSet::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcTriangulatedFaceSet::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 5 ){ std::stringstream err; err << "Wrong parameter count for entity IfcTriangulatedFaceSet, expecting 5, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Coordinates, map );
	readTypeOfRealList2D( args[1], m_Normals );
	m_Closed = IfcBoolean::createObjectFromSTEP( args[2] );
	readEntityReferenceList2D( args[3], m_CoordIndex, map );
	readSelectList( args[4], m_PnIndex, map );
}
void IfcTriangulatedFaceSet::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTessellatedFaceSet::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Closed", m_Closed ) );
	if( m_PnIndex.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> PnIndex_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_PnIndex.begin(), m_PnIndex.end(), std::back_inserter( PnIndex_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "PnIndex", PnIndex_vec_object ) );
	}
}
void IfcTriangulatedFaceSet::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTessellatedFaceSet::getAttributesInverse( vec_attributes_inverse );
}
void IfcTriangulatedFaceSet::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTessellatedFaceSet::setInverseCounterparts( ptr_self_entity );
}
void IfcTriangulatedFaceSet::unlinkFromInverseCounterparts()
{
	IfcTessellatedFaceSet::unlinkFromInverseCounterparts();
}
