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

#include <set>
#include <sstream>
#include <osg/Group>
#include <osg/CullFace>

#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/reader/IfcPPReader.h>
#include <ifcpp/writer/IfcPPWriterSTEP.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcProduct.h>

#include "RepresentationConverter.h"
#include "GeometryInputData.h"
#include "GeometrySettings.h"

class ReaderWriterIFC : public StatusCallback
{
public:
	ReaderWriterIFC();
	~ReaderWriterIFC();	
	/*\brief Opens the given file, reads the content, and puts the entities into m_ifc_model.
	  \param[in] file_path Absolute path of the file to read.
	**/
	void loadModelFromFile( const std::string& file_path );
	
	/*\brief Creates geometry for OpenSceneGraph from previously loaded model.
	  \param[out] parent_group Group to append the resulting geometry.
	**/
	void createGeometryOSG( osg::ref_ptr<osg::Switch> parent_group );
	void convertIfcProduct(	const shared_ptr<IfcProduct>& product, shared_ptr<ShapeInputData>& product_shape );
	void resolveProjectStructure( const shared_ptr<IfcObjectDefinition>& obj_def, osg::ref_ptr<osg::Switch> group );

	// getters and setters
	void setModel( shared_ptr<IfcPPModel> model );
	shared_ptr<IfcPPModel>&						getIfcPPModel()					{ return m_ifc_model; }
	shared_ptr<IfcPPReaderSTEP>&				getIfcPPReader()				{ return m_step_reader; }
	shared_ptr<IfcPPWriterSTEP>&				getIfcPPWriter()				{ return m_step_writer; }
	shared_ptr<RepresentationConverter>&		getRepresentationConverter()	{ return m_representation_converter; }
	std::map<int,shared_ptr<ShapeInputData> >&	getShapeInputData()				{ return m_shape_input_data; }
	std::map<int,shared_ptr<IfcPPObject> >&		getObjectsOutsideSpatialStructure()	{ return m_map_outside_spatial_structure; }
	shared_ptr<GeometrySettings>&				getGeomSettings()				{ return m_geom_settings; }

	void resetNumVerticesPerCircle();
	void clearInputCache();
	void resetModel();

	// message callback wrappers
	static void slotMessageWrapper( void* obj_ptr, shared_ptr<StatusCallback::Message> m );

protected:
	shared_ptr<IfcPPModel>				m_ifc_model;
	shared_ptr<IfcPPReaderSTEP>			m_step_reader;
	shared_ptr<IfcPPWriterSTEP>			m_step_writer;
	shared_ptr<GeometrySettings>		m_geom_settings;

	shared_ptr<RepresentationConverter> m_representation_converter;
	osg::ref_ptr<osg::StateSet>			m_glass_stateset;
	osg::ref_ptr<osg::CullFace>			m_cull_back_off;

	std::map<int, shared_ptr<ShapeInputData> > m_shape_input_data;
	std::map<int, shared_ptr<IfcPPObject> > m_map_outside_spatial_structure;
	double m_recent_progress;
	std::map<int, std::vector<shared_ptr<StatusCallback::Message> > > m_messages;
#ifdef IFCPP_OPENMP
	Mutex m_writelock_messages;
#endif
};
