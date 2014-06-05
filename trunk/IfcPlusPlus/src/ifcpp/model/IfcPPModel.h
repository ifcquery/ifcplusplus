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

#pragma once

#include <vector>
#include <map>
#include <string>
#include "ifcpp/model/shared_ptr.h"

class IfcPPEntity;
class IfcProject;
class IfcGeometricRepresentationContext;
class UnitConverter;

class IfcPPModel
{
public:
	IfcPPModel();
	~IfcPPModel();
	
	enum IfcVersion { IFC_VERSION_UNDEFINED, IFC_VERSION_UNKNOWN, IFC2X, IFC2X2, IFC2X3, IFC2X4, IFC4 };

	const std::map<int,shared_ptr<IfcPPEntity> >& getMapIfcObjects() const { return m_map_entities; }
	void insertEntity( shared_ptr<IfcPPEntity> e, bool overwrite_existing = false );
	void removeEntity( shared_ptr<IfcPPEntity> e );
	void removeEntity( int entity_id );
	int getMaxUsedEntityId();
	shared_ptr<IfcProject> getIfcProject();
	shared_ptr<IfcGeometricRepresentationContext> getIfcGeometricRepresentationContext3D();
	std::string getFileHeader() { return m_file_header; }
	std::string getFileDescription() { return m_IFC_FILE_DESCRIPTION; }
	std::string getFileName() { return m_IFC_FILE_NAME; }
	std::string getFileSchema() { return m_IFC_FILE_SCHEMA; }
	shared_ptr<UnitConverter> getUnitConverter() { return m_unit_converter; }

	void setFileHeader( std::string header );
	void setFileDescription( std::string schema );
	void setFileName( std::string schema );
	void setFileSchema( std::string schema );
	void setIfcProject( shared_ptr<IfcProject> project );
	void resolveInverseAttributes();
	void clearIfcModel();
	void initIfcModel();
	void resetIfcModel();
	void updateCache();
	void clearCache();
	void initFileHeader( std::string file_name );
	IfcVersion getIfcSchemaVersion() {	return m_ifc_schema_version; }
	void setIfcSchemaVersion( IfcVersion ver ) { m_ifc_schema_version = ver; }

	friend class IfcStepReader;

private:
	std::map<int,shared_ptr<IfcPPEntity> >			m_map_entities;
	shared_ptr<IfcProject>							m_ifc_project;
	shared_ptr<IfcGeometricRepresentationContext>	m_geom_context_3d;
	shared_ptr<UnitConverter>						m_unit_converter;
	std::string										m_file_header;
	std::string										m_IFC_FILE_DESCRIPTION;
	std::string										m_IFC_FILE_NAME;
	std::string										m_IFC_FILE_SCHEMA;
	IfcVersion										m_ifc_schema_version;
};
