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
#include "include/IfcGridPlacement.h"
#include "include/IfcGridPlacementDirectionSelect.h"
#include "include/IfcLocalPlacement.h"
#include "include/IfcProduct.h"
#include "include/IfcVirtualGridIntersection.h"

// ENTITY IfcGridPlacement 
IfcGridPlacement::IfcGridPlacement() { m_entity_enum = IFCGRIDPLACEMENT; }
IfcGridPlacement::IfcGridPlacement( int id ) { m_id = id; m_entity_enum = IFCGRIDPLACEMENT; }
IfcGridPlacement::~IfcGridPlacement() {}
shared_ptr<IfcPPObject> IfcGridPlacement::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGridPlacement> copy_self( new IfcGridPlacement() );
	if( m_PlacementLocation ) { copy_self->m_PlacementLocation = dynamic_pointer_cast<IfcVirtualGridIntersection>( m_PlacementLocation->getDeepCopy(options) ); }
	if( m_PlacementRefDirection ) { copy_self->m_PlacementRefDirection = dynamic_pointer_cast<IfcGridPlacementDirectionSelect>( m_PlacementRefDirection->getDeepCopy(options) ); }
	return copy_self;
}
void IfcGridPlacement::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCGRIDPLACEMENT" << "(";
	if( m_PlacementLocation ) { stream << "#" << m_PlacementLocation->m_id; } else { stream << "$"; }
	stream << ",";
	if( m_PlacementRefDirection ) { m_PlacementRefDirection->getStepParameter( stream, true ); } else { stream << "$" ; }
	stream << ");";
}
void IfcGridPlacement::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGridPlacement::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcGridPlacement, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_PlacementLocation, map );
	m_PlacementRefDirection = IfcGridPlacementDirectionSelect::createObjectFromSTEP( args[1], map );
}
void IfcGridPlacement::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcObjectPlacement::getAttributes( vec_attributes );
	vec_attributes.push_back( std::make_pair( "PlacementLocation", m_PlacementLocation ) );
	vec_attributes.push_back( std::make_pair( "PlacementRefDirection", m_PlacementRefDirection ) );
}
void IfcGridPlacement::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcObjectPlacement::getAttributesInverse( vec_attributes_inverse );
}
void IfcGridPlacement::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcObjectPlacement::setInverseCounterparts( ptr_self_entity );
}
void IfcGridPlacement::unlinkFromInverseCounterparts()
{
	IfcObjectPlacement::unlinkFromInverseCounterparts();
}
