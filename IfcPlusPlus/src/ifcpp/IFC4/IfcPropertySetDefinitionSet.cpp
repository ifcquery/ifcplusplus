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
#include <map>
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/model/shared_ptr.h"
#include "ifcpp/model/IfcPPException.h"
#include "include/IfcPropertySetDefinitionSelect.h"
#include "include/IfcPropertySetDefinition.h"
#include "include/IfcPropertySetDefinitionSet.h"

// TYPE IfcPropertySetDefinitionSet 
IfcPropertySetDefinitionSet::IfcPropertySetDefinitionSet() {}
IfcPropertySetDefinitionSet::~IfcPropertySetDefinitionSet() {}
void IfcPropertySetDefinitionSet::getStepParameter( std::stringstream& stream, bool is_select_type ) const
{
	if( is_select_type ) { stream << "IFCPROPERTYSETDEFINITIONSET("; }
	writeEntityList( stream, m_vec );
	if( is_select_type ) { stream << ")"; }
}
shared_ptr<IfcPropertySetDefinitionSet> IfcPropertySetDefinitionSet::createObjectFromStepData( const std::string& arg, const std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// read TYPE
	if( arg.compare( "$" ) == 0 ) { return shared_ptr<IfcPropertySetDefinitionSet>(); }
	shared_ptr<IfcPropertySetDefinitionSet> type_object( new IfcPropertySetDefinitionSet() );
	readEntityReferenceList(  arg, type_object->m_vec, map );
	return type_object;
}
