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
#include "include/IfcPoint.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertexPoint.h"

// ENTITY IfcVertexPoint 
IfcVertexPoint::IfcVertexPoint() { m_entity_enum = IFCVERTEXPOINT; }
IfcVertexPoint::IfcVertexPoint( int id ) { m_id = id; m_entity_enum = IFCVERTEXPOINT; }
IfcVertexPoint::~IfcVertexPoint() {}
shared_ptr<IfcPPObject> IfcVertexPoint::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVertexPoint> copy_self( new IfcVertexPoint() );
	if( m_VertexGeometry ) { copy_self->m_VertexGeometry = dynamic_pointer_cast<IfcPoint>( m_VertexGeometry->getDeepCopy(options) ); }
	return copy_self;
}
void IfcVertexPoint::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCVERTEXPOINT" << "(";
	if( m_VertexGeometry ) { stream << "#" << m_VertexGeometry->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcVertexPoint::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVertexPoint::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVertexPoint, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_VertexGeometry, map );
}
void IfcVertexPoint::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcVertex::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "VertexGeometry", m_VertexGeometry ) );
}
void IfcVertexPoint::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcVertex::getAttributesInverse( vec_attributes_inverse );
}
void IfcVertexPoint::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcVertex::setInverseCounterparts( ptr_self_entity );
}
void IfcVertexPoint::unlinkFromInverseCounterparts()
{
	IfcVertex::unlinkFromInverseCounterparts();
}
