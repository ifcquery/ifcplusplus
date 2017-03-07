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
#include "include/IfcCompositeCurveOnSurface.h"
#include "include/IfcCompositeCurveSegment.h"
#include "include/IfcLogical.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCompositeCurveOnSurface 
IfcCompositeCurveOnSurface::IfcCompositeCurveOnSurface() { m_entity_enum = IFCCOMPOSITECURVEONSURFACE; }
IfcCompositeCurveOnSurface::IfcCompositeCurveOnSurface( int id ) { m_id = id; m_entity_enum = IFCCOMPOSITECURVEONSURFACE; }
IfcCompositeCurveOnSurface::~IfcCompositeCurveOnSurface() {}
shared_ptr<IfcPPObject> IfcCompositeCurveOnSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCompositeCurveOnSurface> copy_self( new IfcCompositeCurveOnSurface() );
	for( size_t ii=0; ii<m_Segments.size(); ++ii )
	{
		auto item_ii = m_Segments[ii];
		if( item_ii )
		{
			copy_self->m_Segments.push_back( dynamic_pointer_cast<IfcCompositeCurveSegment>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_SelfIntersect ) { copy_self->m_SelfIntersect = dynamic_pointer_cast<IfcLogical>( m_SelfIntersect->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCompositeCurveOnSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCOMPOSITECURVEONSURFACE" << "(";
	writeEntityList( stream, m_Segments );
	stream << ",";
	if( m_SelfIntersect ) { m_SelfIntersect->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcCompositeCurveOnSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCompositeCurveOnSurface::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCompositeCurveOnSurface, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReferenceList( args[0], m_Segments, map );
	m_SelfIntersect = IfcLogical::createObjectFromSTEP( args[1] );
}
void IfcCompositeCurveOnSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCompositeCurve::getAttributes( vec_attributes );
}
void IfcCompositeCurveOnSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCompositeCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcCompositeCurveOnSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCompositeCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcCompositeCurveOnSurface::unlinkFromInverseCounterparts()
{
	IfcCompositeCurve::unlinkFromInverseCounterparts();
}
