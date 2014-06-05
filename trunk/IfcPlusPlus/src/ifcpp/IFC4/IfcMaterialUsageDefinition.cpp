/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com - Copyright (C) 2011 Fabian Gerold
*
* This library is open source and may be redistributed and/or modified under  
* the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
* (at your option) any later version.  The full license is in LICENSE file
* included with this distribution, and on the openscenegraph.org website.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* OpenSceneGraph Public License for more details.
*/
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IfcPPEntityEnums.h"
#include "include/IfcMaterialUsageDefinition.h"
#include "include/IfcRelAssociatesMaterial.h"

// ENTITY IfcMaterialUsageDefinition 
IfcMaterialUsageDefinition::IfcMaterialUsageDefinition() {}
IfcMaterialUsageDefinition::IfcMaterialUsageDefinition( int id ) { m_id = id; }
IfcMaterialUsageDefinition::~IfcMaterialUsageDefinition() {}

// method setEntity takes over all attributes from another instance of the class
void IfcMaterialUsageDefinition::setEntity( shared_ptr<IfcPPEntity> other_entity )
{
	shared_ptr<IfcMaterialUsageDefinition> other = dynamic_pointer_cast<IfcMaterialUsageDefinition>(other_entity);
	if( !other) { return; }
}
void IfcMaterialUsageDefinition::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "=IFCMATERIALUSAGEDEFINITION" << "(";
	stream << ");";
}
void IfcMaterialUsageDefinition::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcMaterialUsageDefinition::readStepArguments( const std::vector<std::string>& args, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
}
void IfcMaterialUsageDefinition::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
}
void IfcMaterialUsageDefinition::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	shared_ptr<IfcPPAttributeObjectVector> AssociatedTo_inverse_vec_obj( new IfcPPAttributeObjectVector() );
	for( int i=0; i<m_AssociatedTo_inverse.size(); ++i ) { AssociatedTo_inverse_vec_obj->m_vec.push_back( shared_ptr<IfcRelAssociatesMaterial>( m_AssociatedTo_inverse[i] ) ); }
	vec_attributes_inverse.push_back( std::make_pair( "AssociatedTo_inverse", AssociatedTo_inverse_vec_obj ) );
}
void IfcMaterialUsageDefinition::setInverseCounterparts( shared_ptr<IfcPPEntity> )
{
}
void IfcMaterialUsageDefinition::unlinkSelf()
{
}
