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
#include "include/IfcLabel.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcProductRepresentation.h"
#include "include/IfcRepresentationContext.h"
#include "include/IfcRepresentationItem.h"
#include "include/IfcRepresentationMap.h"
#include "include/IfcShapeAspect.h"
#include "include/IfcShapeRepresentation.h"

// ENTITY IfcShapeRepresentation 
IfcShapeRepresentation::IfcShapeRepresentation() { m_entity_enum = IFCSHAPEREPRESENTATION; }
IfcShapeRepresentation::IfcShapeRepresentation( int id ) { m_id = id; m_entity_enum = IFCSHAPEREPRESENTATION; }
IfcShapeRepresentation::~IfcShapeRepresentation() {}
shared_ptr<IfcPPObject> IfcShapeRepresentation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcShapeRepresentation> copy_self( new IfcShapeRepresentation() );
	if( m_ContextOfItems )
	{
		if( options.shallow_copy_IfcRepresentationContext ) { copy_self->m_ContextOfItems = m_ContextOfItems; }
		else { copy_self->m_ContextOfItems = dynamic_pointer_cast<IfcRepresentationContext>( m_ContextOfItems->getDeepCopy(options) ); }
	}
	if( m_RepresentationIdentifier ) { copy_self->m_RepresentationIdentifier = dynamic_pointer_cast<IfcLabel>( m_RepresentationIdentifier->getDeepCopy(options) ); }
	if( m_RepresentationType ) { copy_self->m_RepresentationType = dynamic_pointer_cast<IfcLabel>( m_RepresentationType->getDeepCopy(options) ); }
	for( size_t ii=0; ii<m_Items.size(); ++ii )
	{
		auto item_ii = m_Items[ii];
		if( item_ii )
		{
			copy_self->m_Items.push_back( dynamic_pointer_cast<IfcRepresentationItem>(item_ii->getDeepCopy(options) ) );
		}
	}
	return copy_self;
}
void IfcShapeRepresentation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSHAPEREPRESENTATION" << "(";
	if( m_ContextOfItems ) { stream << "#" << m_ContextOfItems->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_RepresentationIdentifier ) { m_RepresentationIdentifier->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	if( m_RepresentationType ) { m_RepresentationType->getStepParameter( stream ); } else { stream << "*"; }
	stream << ",";
	writeEntityList( stream, m_Items );
	stream << ");";
}
void IfcShapeRepresentation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcShapeRepresentation::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 4 ){ std::stringstream err; err << "Wrong parameter count for entity IfcShapeRepresentation, expecting 4, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_ContextOfItems, map );
	m_RepresentationIdentifier = IfcLabel::createObjectFromSTEP( args[1] );
	m_RepresentationType = IfcLabel::createObjectFromSTEP( args[2] );
	readEntityReferenceList( args[3], m_Items, map );
}
void IfcShapeRepresentation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcShapeModel::getAttributes( vec_attributes );
}
void IfcShapeRepresentation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcShapeModel::getAttributesInverse( vec_attributes_inverse );
}
void IfcShapeRepresentation::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcShapeModel::setInverseCounterparts( ptr_self_entity );
}
void IfcShapeRepresentation::unlinkFromInverseCounterparts()
{
	IfcShapeModel::unlinkFromInverseCounterparts();
}
