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
#include "include/IfcGeometricRepresentationItem.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcGeometricRepresentationItem 
IfcGeometricRepresentationItem::IfcGeometricRepresentationItem() { m_entity_enum = IFCGEOMETRICREPRESENTATIONITEM; }
IfcGeometricRepresentationItem::IfcGeometricRepresentationItem( int id ) { m_id = id; m_entity_enum = IFCGEOMETRICREPRESENTATIONITEM; }
IfcGeometricRepresentationItem::~IfcGeometricRepresentationItem() {}
shared_ptr<IfcPPObject> IfcGeometricRepresentationItem::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcGeometricRepresentationItem> copy_self( new IfcGeometricRepresentationItem() );
	return copy_self;
}
void IfcGeometricRepresentationItem::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCGEOMETRICREPRESENTATIONITEM" << "(";
	stream << ");";
}
void IfcGeometricRepresentationItem::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcGeometricRepresentationItem::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcGeometricRepresentationItem::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcRepresentationItem::getAttributes( vec_attributes );
}
void IfcGeometricRepresentationItem::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcRepresentationItem::getAttributesInverse( vec_attributes_inverse );
}
void IfcGeometricRepresentationItem::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcRepresentationItem::setInverseCounterparts( ptr_self_entity );
}
void IfcGeometricRepresentationItem::unlinkFromInverseCounterparts()
{
	IfcRepresentationItem::unlinkFromInverseCounterparts();
}
