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
#include "include/IfcStructuralLoadOrResult.h"

// ENTITY IfcStructuralLoadOrResult 
IfcStructuralLoadOrResult::IfcStructuralLoadOrResult() { m_entity_enum = IFCSTRUCTURALLOADORRESULT; }
IfcStructuralLoadOrResult::IfcStructuralLoadOrResult( int id ) { m_id = id; m_entity_enum = IFCSTRUCTURALLOADORRESULT; }
IfcStructuralLoadOrResult::~IfcStructuralLoadOrResult() {}
shared_ptr<IfcPPObject> IfcStructuralLoadOrResult::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcStructuralLoadOrResult> copy_self( new IfcStructuralLoadOrResult() );
	if( m_Name ) { copy_self->m_Name = dynamic_pointer_cast<IfcLabel>( m_Name->getDeepCopy(options) ); }
	return copy_self;
}
void IfcStructuralLoadOrResult::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCSTRUCTURALLOADORRESULT" << "(";
	if( m_Name ) { m_Name->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcStructuralLoadOrResult::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcStructuralLoadOrResult::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const int num_args = (int)args.size();
	if( num_args != 1 ){ std::stringstream err; err << "Wrong parameter count for entity IfcStructuralLoadOrResult, expecting 1, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	m_Name = IfcLabel::createObjectFromSTEP( args[0] );
}
void IfcStructuralLoadOrResult::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcStructuralLoad::getAttributes( vec_attributes );
}
void IfcStructuralLoadOrResult::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcStructuralLoad::getAttributesInverse( vec_attributes_inverse );
}
void IfcStructuralLoadOrResult::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcStructuralLoad::setInverseCounterparts( ptr_self_entity );
}
void IfcStructuralLoadOrResult::unlinkFromInverseCounterparts()
{
	IfcStructuralLoad::unlinkFromInverseCounterparts();
}
