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
#include "include/IfcBoundaryCurve.h"
#include "include/IfcCurveBoundedSurface.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"
#include "include/IfcSurface.h"

// ENTITY IfcCurveBoundedSurface 
IfcCurveBoundedSurface::IfcCurveBoundedSurface() { m_entity_enum = IFCCURVEBOUNDEDSURFACE; }
IfcCurveBoundedSurface::IfcCurveBoundedSurface( int id ) { m_id = id; m_entity_enum = IFCCURVEBOUNDEDSURFACE; }
IfcCurveBoundedSurface::~IfcCurveBoundedSurface() {}
shared_ptr<IfcPPObject> IfcCurveBoundedSurface::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcCurveBoundedSurface> copy_self( new IfcCurveBoundedSurface() );
	if( m_BasisSurface ) { copy_self->m_BasisSurface = dynamic_pointer_cast<IfcSurface>( m_BasisSurface->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Boundaries.size(); ++ii )
	{
		auto item_ii = m_Boundaries[ii];
		if( item_ii )
		{
			copy_self->m_Boundaries.push_back( dynamic_pointer_cast<IfcBoundaryCurve>(item_ii->getDeepCopy(options) ) );
		}
	}
	if( m_ImplicitOuter ) { copy_self->m_ImplicitOuter = dynamic_pointer_cast<IfcBoolean>( m_ImplicitOuter->getDeepCopy(options) ); }
	return copy_self;
}
void IfcCurveBoundedSurface::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCCURVEBOUNDEDSURFACE" << "(";
	if( m_BasisSurface ) { stream << "#" << m_BasisSurface->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_Boundaries );
	stream << ",";
	if( m_ImplicitOuter ) { m_ImplicitOuter->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcCurveBoundedSurface::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcCurveBoundedSurface::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 3 ){ std::stringstream err; err << "Wrong parameter count for entity IfcCurveBoundedSurface, expecting 3, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_BasisSurface, map );
	readEntityReferenceList( args[1], m_Boundaries, map );
	m_ImplicitOuter = IfcBoolean::createObjectFromSTEP( args[2] );
}
void IfcCurveBoundedSurface::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcBoundedSurface::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "BasisSurface", m_BasisSurface ) );
	if( m_Boundaries.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> Boundaries_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_Boundaries.begin(), m_Boundaries.end(), std::back_inserter( Boundaries_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "Boundaries", Boundaries_vec_object ) );
	}
	vec_attributes.push_back( std::make_pair( "ImplicitOuter", m_ImplicitOuter ) );
}
void IfcCurveBoundedSurface::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcBoundedSurface::getAttributesInverse( vec_attributes_inverse );
}
void IfcCurveBoundedSurface::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcBoundedSurface::setInverseCounterparts( ptr_self_entity );
}
void IfcCurveBoundedSurface::unlinkFromInverseCounterparts()
{
	IfcBoundedSurface::unlinkFromInverseCounterparts();
}
