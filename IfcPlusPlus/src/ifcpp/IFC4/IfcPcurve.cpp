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
#include "include/IfcCurve.h"
#include "include/IfcPcurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcPcurve 
IfcPcurve::IfcPcurve() { m_entity_enum = IFCPCURVE; }
IfcPcurve::IfcPcurve( int id ) { m_id = id; m_entity_enum = IFCPCURVE; }
IfcPcurve::~IfcPcurve() {}
shared_ptr<IfcPPObject> IfcPcurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPcurve> copy_self( new IfcPcurve() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcSurface>( m_BasisSurface->getDeepCopy(options) ); }
	if( m_ReferenceCurve ) { copy_self->m_ReferenceCurve = dynamic_pointer_cast<IfcCurve>( m_ReferenceCurve->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPcurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPCURVE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_ReferenceCurve ) { stream << "#" << m_ReferenceCurve->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcPcurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPcurve::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPcurve, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisSurface, map );
	readEntityReference( args[1], m_ReferenceCurve, map );
}
void IfcPcurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "ReferenceCurve", m_ReferenceCurve ) );
}
void IfcPcurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcPcurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcPcurve::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}
