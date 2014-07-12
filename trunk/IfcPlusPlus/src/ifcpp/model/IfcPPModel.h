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
	
	enum IfcPPVersionEnum { IFC_VERSION_UNDEFINED, IFC_VERSION_UNKNOWN, IFC2X, IFC2X2, IFC2X3, IFC2X4, IFC4 };
	class IfcPPSchemaVersion {
		public:
			IfcPPSchemaVersion();
			IfcPPSchemaVersion( std::wstring schema_str, IfcPPVersionEnum schema_enum );
			~IfcPPSchemaVersion();
			std::wstring		m_IFC_FILE_SCHEMA;
			IfcPPVersionEnum	m_ifc_file_schema_enum;
	};
	
	const std::map<int,shared_ptr<IfcPPEntity> >& getMapIfcObjects() const { return m_map_entities; }
	void insertEntity( shared_ptr<IfcPPEntity> e, bool overwrite_existing = false );
	void removeEntity( shared_ptr<IfcPPEntity> e );
	void removeEntity( int entity_id );
	int getMaxUsedEntityId();
	shared_ptr<IfcProject> getIfcProject();
	shared_ptr<IfcGeometricRepresentationContext> getIfcGeometricRepresentationContext3D();
	const std::wstring& getFileHeader() { return m_file_header; }
	const std::wstring& getFileDescription() { return m_IFC_FILE_DESCRIPTION; }
	const std::wstring& getFileName() { return m_IFC_FILE_NAME; }
	
	shared_ptr<UnitConverter> getUnitConverter() { return m_unit_converter; }

	void setFileHeader( std::wstring header );
	void setFileDescription( std::wstring schema );
	void setFileName( std::wstring schema );
	void setFileSchema( std::wstring schema );
	void setIfcProject( shared_ptr<IfcProject> project );
	void resolveInverseAttributes();
	void clearIfcModel();
	void initIfcModel();
	void resetIfcModel();
	void updateCache();
	void clearCache();
	void initFileHeader( std::string file_name );

	IfcPPSchemaVersion& getIfcSchemaVersion() {	return m_ifc_schema_version; }
	void setIfcSchemaVersion( IfcPPSchemaVersion& ver );

	friend class IfcStepReader;

private:
	std::map<int,shared_ptr<IfcPPEntity> >			m_map_entities;
	shared_ptr<IfcProject>							m_ifc_project;
	shared_ptr<IfcGeometricRepresentationContext>	m_geom_context_3d;
	shared_ptr<UnitConverter>						m_unit_converter;
	std::wstring									m_file_header;
	std::wstring									m_IFC_FILE_DESCRIPTION;
	std::wstring									m_IFC_FILE_NAME;
	IfcPPSchemaVersion								m_ifc_schema_version;
};
