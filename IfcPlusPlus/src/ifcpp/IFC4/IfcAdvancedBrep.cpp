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
#include "include/IfcAdvancedBrep.h"
#include "include/IfcClosedShell.h"
#include "include/IfcPresentationLayerAssignment.h"
#include "include/IfcStyledItem.h"

// ENTITY IfcAdvancedBrep 
IfcAdvancedBrep::IfcAdvancedBrep() { m_entity_enum = IFCADVANCEDBREP; }
IfcAdvancedBrep::IfcAdvancedBrep( int id ) { m_id = id; m_entity_enum = IFCADVANCEDBREP; }
IfcAdvancedBrep::~IfcAdvancedBrep() {}
shared_ptr<IfcPPObject> IfcAdvancedBrep::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcAdvancedBrep> copy_self( new IfcAdvancedBrep() );
	if( m_Outer ) { copy_self->m_Outer = dynamic_pointer_cast<IfcClosedShell>( m_Outer->getDeepCopy(options) ); }
	return copy_self;
}
void IfcAdvancedBrep::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCADVANCEDBREP" << "(";
	if( m_Outer ) { stream << "#" << m_Outer->m_id; } else { stream << "*"; }
	stream << ");";
}
void IfcAdvancedBrep::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcAdvancedBrep::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcAdvancedBrep, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Outer, map );
}
void IfcAdvancedBrep::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcManifoldSolidBrep::getAttributes( vec_attributes );
}
void IfcAdvancedBrep::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcManifoldSolidBrep::getAttributesInverse( vec_attributes_inverse );
}
void IfcAdvancedBrep::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcManifoldSolidBrep::setInverseCounterparts( ptr_self_entity );
}
void IfcAdvancedBrep::unlinkFromInverseCounterparts()
{
	IfcManifoldSolidBrep::unlinkFromInverseCounterparts();
}
