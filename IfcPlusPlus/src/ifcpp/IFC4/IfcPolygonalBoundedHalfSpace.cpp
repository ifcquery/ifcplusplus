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
#include "include/IfcAxis2Placement3D.h"
#include "include/IfcBoolean.h"
#include "include/IfcBoundedCurve.h"
#include "include/IfcPolygonalBoundedHalfSpace.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcPolygonalBoundedHalfSpace 
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace() { m_entity_enum = IFCPOLYGONALBOUNDEDHALFSPACE; }
IfcPolygonalBoundedHalfSpace::IfcPolygonalBoundedHalfSpace( int id ) { m_id = id; m_entity_enum = IFCPOLYGONALBOUNDEDHALFSPACE; }
IfcPolygonalBoundedHalfSpace::~IfcPolygonalBoundedHalfSpace() {}
shared_ptr<IfcPPObject> IfcPolygonalBoundedHalfSpace::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPolygonalBoundedHalfSpace> copy_self( new IfcPolygonalBoundedHalfSpace() );
	if( m_BaseSurface ) { copy_self->m_BaseSurface = dynamic_pointer_cast<IfcSurface>( m_BaseSurface->getDeepCopy(options) ); }
	if( m_AgreementFlag ) { copy_self->m_AgreementFlag = dynamic_pointer_cast<IfcBoolean>( m_AgreementFlag->getDeepCopy(options) ); }
	if( m_Position ) { copy_self->m_Position = dynamic_pointer_cast<IfcAxis2Placement3D>( m_Position->getDeepCopy(options) ); }
	if( m_PolygonalBoundary ) { copy_self->m_PolygonalBoundary = dynamic_pointer_cast<IfcBoundedCurve>( m_PolygonalBoundary->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPolygonalBoundedHalfSpace::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPOLYGONALBOUNDEDHALFSPACE" << "(";
	if( m_BaseSurface ) { stream << "#" << m_BaseSurface->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_AgreementFlag ) { m_AgreementFlag->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_Position ) { stream << "#" << m_Position->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_PolygonalBoundary ) { stream << "#" << m_PolygonalBoundary->m_id; } else { stream << "$"; }
	stream << ");";
}
void IfcPolygonalBoundedHalfSpace::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPolygonalBoundedHalfSpace::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPolygonalBoundedHalfSpace, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BaseSurface, map );
	m_AgreementFlag = IfcBoolean::createObjectFromSTEP( args[1] );
	readEntityReference( args[2], m_Position, map );
	readEntityReference( args[3], m_PolygonalBoundary, map );
}
void IfcPolygonalBoundedHalfSpace::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcHalfSpaceSolid::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "Position", m_Position ) );
	vec_attributes.push_back( std::make_pair( "PolygonalBoundary", m_PolygonalBoundary ) );
}
void IfcPolygonalBoundedHalfSpace::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcHalfSpaceSolid::getAttributesInverse( vec_attributes_inverse );
}
void IfcPolygonalBoundedHalfSpace::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcHalfSpaceSolid::setInverseCounterparts( ptr_self_entity );
}
void IfcPolygonalBoundedHalfSpace::unlinkFromInverseCounterparts()
{
	IfcHalfSpaceSolid::unlinkFromInverseCounterparts();
}
