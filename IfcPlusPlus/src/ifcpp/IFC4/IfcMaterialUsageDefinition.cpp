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
#include "include/IfcMaterialUsageDefinition.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialUsageDefinition 
IfcMaterialUsageDefinition::IfcMaterialUsageDefinition() { m_entity_enum = IFCMATERIALUSAGEDEFINITION; }
IfcMaterialUsageDefinition::IfcMaterialUsageDefinition( int id ) { m_id = id; m_entity_enum = IFCMATERIALUSAGEDEFINITION; }
IfcMaterialUsageDefinition::~IfcMaterialUsageDefinition() {}
shared_ptr<IfcPPObject> IfcMaterialUsageDefinition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialUsageDefinition> copy_self( new IfcMaterialUsageDefinition() );
	return copy_self;
}
void IfcMaterialUsageDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALUSAGEDEFINITION" << "(";
	stream << ");";
}
void IfcMaterialUsageDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialUsageDefinition::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcMaterialUsageDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcMaterialUsageDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	if( m_AssociatedTo_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> AssociatedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_AssociatedTo_inverse.size(); ++i )
		{
			if( !m_AssociatedTo_inverse[i].expired() )
			{
				AssociatedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesMaterial>( m_AssociatedTo_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "AssociatedTo_inverse", AssociatedTo_inverse_vec_obj ) );
	}
}
void IfcMaterialUsageDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialUsageDefinition::unlinkFromInverseCounterparts()
{
}
