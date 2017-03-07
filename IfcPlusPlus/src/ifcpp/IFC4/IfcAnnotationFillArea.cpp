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
#include "include/IfcAnnotationFillArea.h"
#include "include/IfcCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcAnnotationFillArea 
IfcAnnotationFillArea::IfcAnnotationFillArea() { m_entity_enum = IFCANNOTATIONFILLAREA; }
IfcAnnotationFillArea::IfcAnnotationFillArea( int id ) { m_id = id; m_entity_enum = IFCANNOTATIONFILLAREA; }
IfcAnnotationFillArea::~IfcAnnotationFillArea() {}
shared_ptr<IfcPPObject> IfcAnnotationFillArea::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAnnotationFillArea> copy_self( new IfcAnnotationFillArea() );
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
void IfcAnnotationFillArea::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCANNOTATIONFILLAREA" << "(";
	if( m_OuterBoundary ) { stream << "#" << m_OuterBoundary->m_id; } else { stream << "$"; }
	stream << ",";
	writeEntityList( stream, m_InnerBoundaries );
	stream << ");";
}
void IfcAnnotationFillArea::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAnnotationFillArea::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAnnotationFillArea, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_OuterBoundary, map );
	readEntityReferenceList( args[1], m_InnerBoundaries, map );
}
void IfcAnnotationFillArea::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "OuterBoundary", m_OuterBoundary ) );
	if( m_InnerBoundaries.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> InnerBoundaries_vec_object( new  IfcPPAttributeObjectVector() );
		std::copy( m_InnerBoundaries.begin(), m_InnerBoundaries.end(), std::back_inserter( InnerBoundaries_vec_object->m_vec ) );
		vec_attributes.push_back( std::make_pair( "InnerBoundaries", InnerBoundaries_vec_object ) );
	}
}
void IfcAnnotationFillArea::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcAnnotationFillArea::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcAnnotationFillArea::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
