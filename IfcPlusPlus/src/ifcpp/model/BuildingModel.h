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
#include <unordered_map>
#include <string>
#include "BasicTypes.h"
#include "StatusCallback.h"

class BuildingObject;
class BuildingEntity;
class UnitConverter;
namespace IFC4X3
{
	class IfcProject;
	class IfcGeometricRepresentationContext;
}

class IFCQUERY_EXPORT BuildingModel : public StatusCallback
{
public:
	BuildingModel();
	~BuildingModel() override;

	enum SchemaVersionEnum { IFC_VERSION_UNDEFINED, IFC_VERSION_UNKNOWN, IFC2X, IFC2X2, IFC2X3, IFC2X4, IFC4, IFC4X1, IFC4X3 };
	BuildingModelMapType<int, shared_ptr<BuildingEntity> >& getMapIfcEntities() { return m_map_entities; }
	void setMapIfcEntities(const std::unordered_map<int, shared_ptr<BuildingEntity> >& map);
	void insertEntity(shared_ptr<BuildingEntity> e, bool overwrite_existing = false, bool warn_on_existing_entities = true);
	void removeEntity(shared_ptr<BuildingEntity> e);
	void removeEntity(int tag);
	void removeUnreferencedEntities();

	/*! \brief Method getNextUnusedEntityTag. Return the next tag that is not in the model. Look for gaps in existing map */
	int getLowestUnusedEntityTagSlow();

	/*! \brief Method getNextUnusedEntityTagFast. Return a tag that is not in the model. Do not look for gaps in existing map. */
	int getNextUnusedEntityTagFast();
	shared_ptr<IFC4X3::IfcProject> getIfcProject();
	shared_ptr<IFC4X3::IfcGeometricRepresentationContext> getIfcGeometricRepresentationContext3D();
	shared_ptr<UnitConverter>& getUnitConverter() { return m_unit_converter; }

	/*! \brief Method getIfcSchemaVersion. Returns the IFC version of the loaded file */
	SchemaVersionEnum& getIfcSchemaVersionEnumOfLoadedFile() { return m_ifc_schema_version_loaded_file; }
	std::string getIfcSchemaVersionOfLoadedFile();

	/*! \brief Method getIfcSchemaVersionCurrent. Returns the IFC version after loading. It is the newest implemented IFC version. IFC version of the loaded file may be older */
	SchemaVersionEnum& getIfcSchemaVersionEnumCurrent() { return m_ifc_schema_version_current; }
	void setIfcSchemaVersionEnumCurrent(SchemaVersionEnum schema) { m_ifc_schema_version_current = schema; }
	std::string getIfcSchemaVersionCurrent();

	const std::string& getFileHeader() { return m_file_header; }
	const std::string& getFileDescription() { return m_IFC_FILE_DESCRIPTION; }
	const std::string& getFileName() { return m_IFC_FILE_NAME; }

	void setFileHeader(const std::string& header);
	void setFileDescription(const std::string& description);
	void setFileName(const std::string& name);
	void setIfcProject(shared_ptr<IFC4X3::IfcProject> project);
	void setUnitConverter(shared_ptr<UnitConverter>& uc);
	void setIfcGeometricRepresentationContext3D(shared_ptr<IFC4X3::IfcGeometricRepresentationContext>& context);
	void resolveInverseAttributes();
	void unsetInverseAttributes();
	void clearIfcModel();
	void initIfcModel();
	void initCopyIfcModel(const shared_ptr<BuildingModel>& other);
	void resetIfcModel();
	void updateCache();
	void clearCache();
	void initFileHeader(const std::string& fileName, const std::string& generatingApplication);
	static void collectDependentEntities(shared_ptr<BuildingObject> entity, std::unordered_map<BuildingObject*, shared_ptr<BuildingObject> >& target_map, bool resolveInverseAttributes);

	void cancelLoading()
	{
		m_cancelLoading = true;
	}
	bool isLoadingCancelled()
	{
		return m_cancelLoading;
	}

private:
	BuildingModelMapType<int, shared_ptr<BuildingEntity> >	m_map_entities;
	int														m_max_entity_id = -1;
	shared_ptr<IFC4X3::IfcProject>							m_ifc_project;
	shared_ptr<IFC4X3::IfcGeometricRepresentationContext>	m_geom_context_3d;
	shared_ptr<UnitConverter>							m_unit_converter;
	std::string											m_file_name;
	std::string											m_file_header;
	std::string											m_IFC_FILE_DESCRIPTION;
	std::string											m_IFC_FILE_NAME;
	SchemaVersionEnum									m_ifc_schema_version_loaded_file = IFC4X3;
	SchemaVersionEnum									m_ifc_schema_version_current = IFC4X3;
	bool m_cancelLoading = false;
};
