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
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcVertex.h"
#include "include/IfcVertexLoop.h"

// ENTITY IfcVertexLoop 
IfcVertexLoop::IfcVertexLoop() { m_entity_enum = IFCVERTEXLOOP; }
IfcVertexLoop::IfcVertexLoop( int id ) { m_id = id; m_entity_enum = IFCVERTEXLOOP; }
IfcVertexLoop::~IfcVertexLoop() {}
shared_ptr<IfcPPObject> IfcVertexLoop::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcVertexLoop> copy_self( new IfcVertexLoop() );
	if( m_LoopVertex ) { copy_self->m_LoopVertex = dynamic_pointer_cast<IfcVertex>( m_LoopVertex->getDeepCopy(options) ); }
	return copy_self;
}
void IfcVertexLoop::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCVERTEXLOOP" << "(";
	if( m_LoopVertex ) { stream << "#" << m_LoopVertex->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcVertexLoop::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcVertexLoop::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcVertexLoop, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_LoopVertex, map );
}
void IfcVertexLoop::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcLoop::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "LoopVertex", m_LoopVertex ) );
}
void IfcVertexLoop::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcLoop::getAttributesInverse( vec_attributes_inverse );
}
void IfcVertexLoop::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcLoop::setInverseCounterparts( ptr_self_entity );
}
void IfcVertexLoop::unlinkFromInverseCounterparts()
{
	IfcLoop::unlinkFromInverseCounterparts();
}
