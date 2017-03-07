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
#include "include/IfcOrientedEdge.h"
#include "include/IfcPath.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPath 
IfcPath::IfcPath() { m_entity_enum = IFCPATH; }
IfcPath::IfcPath( int id ) { m_id = id; m_entity_enum = IFCPATH; }
IfcPath::~IfcPath() {}
shared_ptr<IfcPPObject> IfcPath::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPath> copy_self( new IfcPath() );
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
void IfcPath::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPATH" << "(";
	writeEntityList( stream, m_EdgeList );
	stream << ");";
}
void IfcPath::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPath::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPath, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_EdgeList, map );
}
void IfcPath::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
	if( m_EdgeList.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> EdgeList_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_EdgeList.begin(), m_EdgeList.end(), std::back_inserter( EdgeList_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "EdgeList", EdgeList_vec_object ) );
	}
}
void IfcPath::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTopologicalRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcPath::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcPath::unlinkFromInverseCounterparts()
{
	IfcTopologicalRepresentationItem::unlinkFromInverseCounterparts();
}
