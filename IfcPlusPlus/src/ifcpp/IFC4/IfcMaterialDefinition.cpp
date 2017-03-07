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
#include "include/IfcExternalReferenceRelationship.h"
#include "include/IfcMaterialDefinition.h"
#include "include/IfcMaterialProperties.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialDefinition 
IfcMaterialDefinition::IfcMaterialDefinition() { m_entity_enum = IFCMATERIALDEFINITION; }
IfcMaterialDefinition::IfcMaterialDefinition( int id ) { m_id = id; m_entity_enum = IFCMATERIALDEFINITION; }
IfcMaterialDefinition::~IfcMaterialDefinition() {}
shared_ptr<IfcPPObject> IfcMaterialDefinition::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcMaterialDefinition> copy_self( new IfcMaterialDefinition() );
	return copy_self;
}
void IfcMaterialDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCMATERIALDEFINITION" << "(";
	stream << ");";
}
void IfcMaterialDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialDefinition::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcMaterialDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcMaterialDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
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
	if( m_HasExternalReferences_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasExternalReferences_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasExternalReferences_inverse.size(); ++i )
		{
			if( !m_HasExternalReferences_inverse[i].expired() )
			{
				HasExternalReferences_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcExternalReferenceRelationship>( m_HasExternalReferences_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasExternalReferences_inverse", HasExternalReferences_inverse_vec_obj ) );
	}
	if( m_HasProperties_inverse.size() > 0 )
	{
		shared_ptr<IfcPPAttributeObjectVector> HasProperties_inverse_vec_obj( new IfcPPAttributeObjectVector() );
		for( size_t i=0; i<m_HasProperties_inverse.size(); ++i )
		{
			if( !m_HasProperties_inverse[i].expired() )
			{
				HasProperties_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcMaterialProperties>( m_HasProperties_inverse[i] ) );
			}
		}
		vec_attributes_inverse.push_back( std::make_pair( "HasProperties_inverse", HasProperties_inverse_vec_obj ) );
	}
}
void IfcMaterialDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialDefinition::unlinkFromInverseCounterparts()
{
}
