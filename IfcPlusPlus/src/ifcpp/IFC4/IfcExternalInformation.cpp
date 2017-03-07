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
#include "include/IfcExternalInformation.h"

// ENTITY IfcExternalInformation 
IfcExternalInformation::IfcExternalInformation() { m_entity_enum = IFCEXTERNALINFORMATION; }
IfcExternalInformation::IfcExternalInformation( int id ) { m_id = id; m_entity_enum = IFCEXTERNALINFORMATION; }
IfcExternalInformation::~IfcExternalInformation() {}
shared_ptr<IfcPPObject> IfcExternalInformation::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcExternalInformation> copy_self( new IfcExternalInformation() );
	return copy_self;
}
void IfcExternalInformation::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCEXTERNALINFORMATION" << "(";
	stream << ");";
}
void IfcExternalInformation::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcExternalInformation::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcExternalInformation::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcExternalInformation::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
}
void IfcExternalInformation::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcExternalInformation::unlinkFromInverseCounterparts()
{
}
