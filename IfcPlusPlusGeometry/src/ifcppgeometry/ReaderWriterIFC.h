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

#include <sstream>
#include <osgDB/ReaderWriter>
#include <osg/Switch>
#include <osg/CullFace>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/StatusObservable.h>
#include <ifcpp/reader/IfcPPReader.h>
#include <ifcpp/writer/IfcPPWriterSTEP.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcProduct.h>

#include "RepresentationConverter.h"
#include "GeometryInputData.h"
#include "GeometrySettings.h"

// osg::Registry uses osg::ref_ptr, so always use it for ReaderWriterIFC too
class ReaderWriterIFC : public osgDB::ReaderWriter, public StatusObservable
{
public:
	ReaderWriterIFC();
	~ReaderWriterIFC();	
	
	virtual const char* className() { return "ReaderWriterIFC"; }
	virtual osgDB::ReaderWriter::ReadResult readNode(const std::string& filename, const osgDB::ReaderWriter::Options*);

	void createGeometry();
	void convertIfcProduct(	const shared_ptr<IfcProduct>& product, shared_ptr<ShapeInputData>& product_shape );
	void resolveProjectStructure( const shared_ptr<IfcObjectDefinition>& parent_obj_def, osg::Group* parent_group );

	// getters and setters
	void setModel( shared_ptr<IfcPPModel> model );
	shared_ptr<IfcPPModel>&						getIfcPPModel()					{ return m_ifc_model; }
	shared_ptr<IfcPPReader>&					getIfcPPReader()				{ return m_step_reader; }
	shared_ptr<IfcPPWriterSTEP>&				getIfcPPWriter()				{ return m_step_writer; }
	shared_ptr<RepresentationConverter>&		getRepresentationConverter()	{ return m_representation_converter; }
	std::map<int,shared_ptr<ShapeInputData> >&	getShapeInputData()				{ return m_shape_input_data; }
	std::map<int,shared_ptr<IfcPPObject> >&		getObjectsOutsideSpatialStructure()	{ return m_map_outside_spatial_structure; }
	shared_ptr<UnitConverter>&					getUnitConverter()				{ return m_unit_converter; }
	shared_ptr<GeometrySettings>&				getGeomSettings()				{ return m_geom_settings; }

	std::stringstream& getErrors() { return m_err; }
	std::stringstream& getMessages() { return m_messages; }
	void resetNumVerticesPerCircle();
	void clearInputCache();
	void resetModel();

	static void slotProgressValueWrapper( void* obj_ptr, double value, const std::string& progress_type );
	static void slotProgressTextWrapper( void* obj_ptr, const std::wstring& str );
	static void slotMessageWrapper( void* obj_ptr, const std::wstring& str );
	static void slotErrorWrapper( void* obj_ptr, const std::wstring& str );

protected:
	shared_ptr<IfcPPModel>				m_ifc_model;
	shared_ptr<IfcPPReader>				m_step_reader;
	shared_ptr<IfcPPWriterSTEP>			m_step_writer;
	std::stringstream					m_err;
	std::stringstream					m_messages;
	shared_ptr<GeometrySettings>		m_geom_settings;

	shared_ptr<UnitConverter>			m_unit_converter;
	shared_ptr<RepresentationConverter> m_representation_converter;
	osg::ref_ptr<osg::StateSet>			m_glass_stateset;
	osg::ref_ptr<osg::CullFace>			m_cull_back_off;

	std::map<int, shared_ptr<ShapeInputData> > m_shape_input_data;
	std::map<int, shared_ptr<IfcPPObject> > m_map_visited;
	std::map<int, shared_ptr<IfcPPObject> > m_map_outside_spatial_structure;
	osg::ref_ptr<osg::Group> m_group_result;
	double m_recent_progress;
};
