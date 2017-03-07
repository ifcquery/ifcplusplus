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
#include "include/IfcLengthMeasure.h"
#include "include/IfcPlanarExtent.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcPlanarExtent 
IfcPlanarExtent::IfcPlanarExtent() { m_entity_enum = IFCPLANAREXTENT; }
IfcPlanarExtent::IfcPlanarExtent( int id ) { m_id = id; m_entity_enum = IFCPLANAREXTENT; }
IfcPlanarExtent::~IfcPlanarExtent() {}
shared_ptr<IfcPPObject> IfcPlanarExtent::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcPlanarExtent> copy_self( new IfcPlanarExtent() );
	if( m_SizeInX ) { copy_self->m_SizeInX = dynamic_pointer_cast<IfcLengthMeasure>( m_SizeInX->getDeepCopy(options) ); }
	if( m_SizeInY ) { copy_self->m_SizeInY = dynamic_pointer_cast<IfcLengthMeasure>( m_SizeInY->getDeepCopy(options) ); }
	return copy_self;
}
void IfcPlanarExtent::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCPLANAREXTENT" << "(";
	if( m_SizeInX ) { m_SizeInX->getStepParameter( stream ); } else { stream << "$"; }
	stream << ",";
	if( m_SizeInY ) { m_SizeInY->getStepParameter( stream ); } else { stream << "$"; }
	stream << ");";
}
void IfcPlanarExtent::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcPlanarExtent::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcPlanarExtent, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_SizeInX = IfcLengthMeasure::createObjectFromSTEP( args[0] );
	m_SizeInY = IfcLengthMeasure::createObjectFromSTEP( args[1] );
}
void IfcPlanarExtent::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcGeometricRepresentationItem::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "SizeInX", m_SizeInX ) );
	vec_attributes.push_back( std::make_pair( "SizeInY", m_SizeInY ) );
}
void IfcPlanarExtent::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcGeometricRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcPlanarExtent::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcGeometricRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcPlanarExtent::unlinkFromInverseCounterparts()
{
	IfcGeometricRepresentationItem::unlinkFromInverseCounterparts();
}
