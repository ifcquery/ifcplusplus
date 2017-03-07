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
#include "include/IfcFaceBound.h"
#include "include/IfcLoop.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcFaceBound 
IfcFaceBound::IfcFaceBound() { m_entity_enum = IFCFACEBOUND; }
IfcFaceBound::IfcFaceBound( int id ) { m_id = id; m_entity_enum = IFCFACEBOUND; }
IfcFaceBound::~IfcFaceBound() {}
shared_ptr<IfcPPObject> IfcFaceBound::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFaceBound> copy_self( new IfcFaceBound() );
	if( m_Bound ) { copy_self->m_Bound = dynamic_pointer_cast<IfcLoop>( m_Bound->getDeepCopy(options) ); }
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcBoolean>( m_Orientation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFaceBound::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFACEBOUND" << "(";
	if( m_Bound ) { stream << "#" << m_Bound->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_Orientation ) { m_Orientation->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcFaceBound::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFaceBound::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFaceBound, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Bound, map );
	m_Orientation = IfcBoolean::createObjectFromSTEP( args[1] );
}
void IfcFaceBound::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcTopologicalRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Bound", m_Bound ) );
	vec_attributes.push_back( std::make_pair( "Orientation", m_Orientation ) );
}
void IfcFaceBound::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcTopologicalRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcFaceBound::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcTopologicalRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcFaceBound::unlinkFromInverseCounterparts()
{
	IfcTopologicalRepresentationItem::unlinkFromInverseCounterparts();
}
