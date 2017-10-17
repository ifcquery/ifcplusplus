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

#pragma once

#include <vector>
#include <map>
#include <string>
#include <boost/unordered_map.hpp>
#include "IfcPPBasicTypes.h"
#include "StatusCallback.h"

class IfcPPEntity;
class IfcProject;
class IfcGeometricRepresentationContext;
class UnitConverter;

class IFCPP_EXPORT IfcPPModel : public StatusCallback
{
public:
	IfcPPModel();
	virtual ~IfcPPModel();
	
	enum IfcPPVersionEnum { IFC_VERSION_UNDEFINED, IFC_VERSION_UNKNOWN, IFC2X, IFC2X2, IFC2X3, IFC2X4, IFC4 };
	class IfcPPSchemaVersion
	{
		public:
			IfcPPSchemaVersion()
			{
				m_IFC_FILE_SCHEMA = L"IFC4";
				m_ifc_file_schema_enum = IfcPPModel::IFC4;
			}
			IfcPPSchemaVersion(std::wstring schema_str, IfcPPVersionEnum schema_enum)
				: m_IFC_FILE_SCHEMA(schema_str.c_str()), m_ifc_file_schema_enum(schema_enum)
			{
			}
			~IfcPPSchemaVersion()
			{
			}
			std::wstring		m_IFC_FILE_SCHEMA;
			IfcPPVersionEnum	m_ifc_file_schema_enum;
	};
	
	const std::map<int, shared_ptr<IfcPPEntity> >& getMapIfcEntities() const { return m_map_entities; }
	void setMapIfcEntities( const std::map<int, shared_ptr<IfcPPEntity> >& map );
	void insertEntity( shared_ptr<IfcPPEntity> e, bool overwrite_existing = false, bool warn_on_existing_objects = true );
	void removeEntity( shared_ptr<IfcPPEntity> e );
	void removeEntity( int entity_id );
	void removeUnreferencedEntities();
	int getMaxUsedEntityId();
	shared_ptr<IfcProject> getIfcProject();
	shared_ptr<IfcGeometricRepresentationContext> getIfcGeometricRepresentationContext3D();
	shared_ptr<UnitConverter>& getUnitConverter() { return m_unit_converter; }
	IfcPPSchemaVersion& getIfcSchemaVersion() {	return m_ifc_schema_version; }
	const std::wstring& getFileHeader() { return m_file_header; }
	const std::wstring& getFileDescription() { return m_IFC_FILE_DESCRIPTION; }
	const std::wstring& getFileName() { return m_IFC_FILE_NAME; }

	void setFileHeader( std::wstring header );
	void setFileDescription( std::wstring schema );
	void setFileName( std::wstring schema );
	void setIfcProject( shared_ptr<IfcProject> project );
	void setUnitConverter( shared_ptr<UnitConverter>& uc );
	void setIfcGeometricRepresentationContext3D( shared_ptr<IfcGeometricRepresentationContext>& context );
	void resolveInverseAttributes();
	void clearIfcModel();
	void initIfcModel();
	void initCopyIfcModel( const shared_ptr<IfcPPModel>& other );
	void resetIfcModel();
	void updateCache();
	void clearCache();
	void initFileHeader( std::string file_name );
	static void collectDependentEntities( shared_ptr<IfcPPEntity>& entity, std::map<IfcPPEntity*, shared_ptr<IfcPPEntity> >& target_map );

	friend class IfcPPReaderSTEP;
	friend class IfcPPReaderXML;

private:
	std::map<int, shared_ptr<IfcPPEntity> >			m_map_entities;
	shared_ptr<IfcProject>							m_ifc_project;
	shared_ptr<IfcGeometricRepresentationContext>	m_geom_context_3d;
	shared_ptr<UnitConverter>						m_unit_converter;
	std::string										m_file_name;
	std::wstring									m_file_header;
	std::wstring									m_IFC_FILE_DESCRIPTION;
	std::wstring									m_IFC_FILE_NAME;
	IfcPPSchemaVersion								m_ifc_schema_version;
};
