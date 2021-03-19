/* -*-c++-*- IfcQuery www.ifcquery.com
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

#pragma once

#include <vector>
#include <map>
#include <string>
#include "BasicTypes.h"
#include "StatusCallback.h"

class BuildingEntity;
class IfcProject;
class IfcGeometricRepresentationContext;
class UnitConverter;

class IFCQUERY_EXPORT BuildingModel : public StatusCallback
{
public:
	BuildingModel();
	~BuildingModel() override;
	
	enum SchemaVersionEnum { IFC_VERSION_UNDEFINED, IFC_VERSION_UNKNOWN, IFC2X, IFC2X2, IFC2X3, IFC2X4, IFC4, IFC4X1, IFC4X3RC1, IFC4X3RC2, IFC4X3RC3 };
	std::map<int, shared_ptr<BuildingEntity> >& getMapIfcEntities() { return m_map_entities; }
	void setMapIfcEntities( const std::map<int, shared_ptr<BuildingEntity> >& map );
	void insertEntity( shared_ptr<BuildingEntity> e, bool overwrite_existing = false, bool warn_on_existing_entities = true );
	void removeEntity( shared_ptr<BuildingEntity> e );
	void removeEntity( int entity_id );
	void removeUnreferencedEntities();
	
	/*! \brief Method getMaxUsedEntityId. Return the highest entity id in the model. */
	int getMaxUsedEntityId();
	shared_ptr<IfcProject> getIfcProject();
	shared_ptr<IfcGeometricRepresentationContext> getIfcGeometricRepresentationContext3D();
	shared_ptr<UnitConverter>& getUnitConverter() { return m_unit_converter; }

	/*! \brief Method getIfcSchemaVersion. Returns the IFC version of the loaded file */
	SchemaVersionEnum& getIfcSchemaVersionEnumOfLoadedFile() {	return m_ifc_schema_version_loaded_file; }
	std::wstring getIfcSchemaVersionOfLoadedFile();

	/*! \brief Method getIfcSchemaVersionCurrent. Returns the IFC version after loading. It is the newest implemented IFC version. IFC version of the loaded file may be older */
	SchemaVersionEnum& getIfcSchemaVersionEnumCurrent() { return m_ifc_schema_version_current; }
	std::wstring getIfcSchemaVersionCurrent();
	
	const std::wstring& getFileHeader() { return m_file_header; }
	const std::wstring& getFileDescription() { return m_IFC_FILE_DESCRIPTION; }
	const std::wstring& getFileName() { return m_IFC_FILE_NAME; }

	void setFileHeader( const std::wstring& header );
	void setFileDescription( const std::wstring& description );
	void setFileName( const std::wstring& name );
	void setIfcProject( shared_ptr<IfcProject> project );
	void setUnitConverter( shared_ptr<UnitConverter>& uc );
	void setIfcGeometricRepresentationContext3D( shared_ptr<IfcGeometricRepresentationContext>& context );
	void resolveInverseAttributes();
	void unsetInverseAttributes();
	void clearIfcModel();
	void initIfcModel();
	void initCopyIfcModel( const shared_ptr<BuildingModel>& other );
	void resetIfcModel();
	void updateCache();
	void clearCache();
	void initFileHeader( std::wstring file_name );
	static void collectDependentEntities( shared_ptr<BuildingEntity> entity, std::map<BuildingEntity*, shared_ptr<BuildingEntity> >& target_map, bool resolveInverseAttributes = false );

	friend class ReaderSTEP;
	friend class ReaderXML;

private:
	std::map<int, shared_ptr<BuildingEntity> >		m_map_entities;
	shared_ptr<IfcProject>							m_ifc_project;
	shared_ptr<IfcGeometricRepresentationContext>	m_geom_context_3d;
	shared_ptr<UnitConverter>						m_unit_converter;
	std::wstring									m_file_name;
	std::wstring									m_file_header;
	std::wstring									m_IFC_FILE_DESCRIPTION;
	std::wstring									m_IFC_FILE_NAME;
	SchemaVersionEnum								m_ifc_schema_version_loaded_file = IFC4X1;
	SchemaVersionEnum								m_ifc_schema_version_current = IFC4X1;
};
