/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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

#include <vector>
#include <map>
#include <string>
#include "ifcpp/model/IfcPPModel.h"
#include "ifcpp/reader/IfcPPReaderXML.h"

void IfcPPReaderXML::removeComments( std::string& buffer )
{
	// not implemented
}


void IfcPPReaderXML::readStreamHeader(	const std::string& in, shared_ptr<IfcPPModel>& target_model )
{
	// not implemented
}

void IfcPPReaderXML::readStreamData( std::string& in, const IfcPPModel::IfcPPSchemaVersion& ifc_version, std::map<int,shared_ptr<IfcPPEntity> >& map )
{
	// not implemented
}

void IfcPPReaderXML::readStreamData( std::string& in, shared_ptr<IfcPPModel>& model )
{
	IfcPPModel::IfcPPSchemaVersion& file_schema_version = model->getIfcSchemaVersion();
	std::map<int,shared_ptr<IfcPPEntity> >& map_entities = model->m_map_entities;
	readStreamData( in, file_schema_version, map_entities );
}
