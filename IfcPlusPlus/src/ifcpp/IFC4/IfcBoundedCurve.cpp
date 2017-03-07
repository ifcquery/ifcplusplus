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
#include "include/IfcBoundedCurve.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcBoundedCurve 
IfcBoundedCurve::IfcBoundedCurve() { m_entity_enum = IFCBOUNDEDCURVE; }
IfcBoundedCurve::IfcBoundedCurve( int id ) { m_id = id; m_entity_enum = IFCBOUNDEDCURVE; }
IfcBoundedCurve::~IfcBoundedCurve() {}
shared_ptr<IfcPPObject> IfcBoundedCurve::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcBoundedCurve> copy_self( new IfcBoundedCurve() );
	return copy_self;
}
void IfcBoundedCurve::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCBOUNDEDCURVE" << "(";
	stream << ");";
}
void IfcBoundedCurve::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcBoundedCurve::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcBoundedCurve::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcCurve::getAttributes( vec_attributes );
}
void IfcBoundedCurve::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcCurve::getAttributesInverse( vec_attributes_inverse );
}
void IfcBoundedCurve::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcCurve::setInverseCounterparts( ptr_self_entity );
}
void IfcBoundedCurve::unlinkFromInverseCounterparts()
{
	IfcCurve::unlinkFromInverseCounterparts();
}
