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
#include "include/IfcCurveBoundedPlane.h"
#include "include/IfcPlane.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcCurveBoundedPlane 
IfcCurveBoundedPlane::IfcCurveBoundedPlane() { m_entity_enum = IFCCURVEBOUNDEDPLANE; }
IfcCurveBoundedPlane::IfcCurveBoundedPlane( int id ) { m_id = id; m_entity_enum = IFCCURVEBOUNDEDPLANE; }
IfcCurveBoundedPlane::~IfcCurveBoundedPlane() {}
shared_ptr<IfcPPObject> IfcCurveBoundedPlane::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCurveBoundedPlane> copy_self( new IfcCurveBoundedPlane() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcPlane>( m_BasisSurface->getDeepCopy(options) ); }
	if( m_OuterBoundary ) { copy_self->m_OuterBoundary = dynamic_pointer_cast<IfcCurve>( m_OuterBoundary->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_InnerBoundaries.size(); ++ii )
	{
		auto item_ii = m_InnerBoundaries[ii];
		if( item_ii )
		{
			copy_self->m_InnerBoundaries.push_back( dynamic_pointer_cast<IfcCurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcCurveBoundedPlane::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVEBOUNDEDPLANE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_OuterBoundary ) { stream << "#" << m_OuterBoundary->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_InnerBoundaries );
	stream << ");";
}
void IfcCurveBoundedPlane::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveBoundedPlane::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCurveBoundedPlane, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisSurface, map );
	readEntityReference( args[1], m_OuterBoundary, map );
	readEntityReferenceList( args[2], m_InnerBoundaries, map );
}
void IfcCurveBoundedPlane::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	vec_attributes.push_back( std::make_pair( "OuterBoundary", m_OuterBoundary ) );
	if( m_InnerBoundaries.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> InnerBoundaries_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_InnerBoundaries.begin(), m_InnerBoundaries.end(), std::back_inserter( InnerBoundaries_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "InnerBoundaries", InnerBoundaries_vec_object ) );
	}
}
void IfcCurveBoundedPlane::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveBoundedPlane::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveBoundedPlane::unlinkFromInverseCounterparts()
{
	IfcBoundedSurface::unlinkFromInverseCounterparts();
}
