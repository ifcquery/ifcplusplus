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
#include "include/IfcReal.h"
#include "include/IfcSurfaceStyleRefraction.h"

// ENTITY IfcSurfaceStyleRefraction 
IfcSurfaceStyleRefraction::IfcSurfaceStyleRefraction() { m_entity_enum = IFCSURFACESTYLEREFRACTION; }
IfcSurfaceStyleRefraction::IfcSurfaceStyleRefraction( int id ) { m_id = id; m_entity_enum = IFCSURFACESTYLEREFRACTION; }
IfcSurfaceStyleRefraction::~IfcSurfaceStyleRefraction() {}
shared_ptr<IfcPPObject> IfcSurfaceStyleRefraction::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcSurfaceStyleRefraction> copy_self( new IfcSurfaceStyleRefraction() );
	if( m_RefractionIndex ) { copy_self->m_RefractionIndex = dynamic_pointer_cast<IfcReal>( m_RefractionIndex->getDeepCopy(options) ); }
	if( m_DispersionFactor ) { copy_self->m_DispersionFactor = dynamic_pointer_cast<IfcReal>( m_DispersionFactor->getDeepCopy(options) ); }
	return copy_self;
}
void IfcSurfaceStyleRefraction::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSURFACESTYLEREFRACTION" << "(";
	if( m_RefractionIndex ) { m_RefractionIndex->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_DispersionFactor ) { m_DispersionFactor->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcSurfaceStyleRefraction::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcSurfaceStyleRefraction::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcSurfaceStyleRefraction, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_RefractionIndex = IfcReal::createObjectFromSTEP( args[0] );
	m_DispersionFactor = IfcReal::createObjectFromSTEP( args[1] );
}
void IfcSurfaceStyleRefraction::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcPresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "RefractionIndex", m_RefractionIndex ) );
	vec_attributes.push_back( std::make_pair( "DispersionFactor", m_DispersionFactor ) );
}
void IfcSurfaceStyleRefraction::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcPresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcSurfaceStyleRefraction::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcPresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcSurfaceStyleRefraction::unlinkFromInverseCounterparts()
{
	IfcPresentationItem::unlinkFromInverseCounterparts();
}
