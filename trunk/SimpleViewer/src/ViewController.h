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

#include <QtCore/QObject>
#include <osgGA/GUIEventHandler>
#include <osg/Material>
#include <ifcpp/model/shared_ptr.h>

class ViewController
{
public:
	enum ViewerMode
	{
		VIEWER_MODE_SHADED,
		VIEWER_MODE_WIREFRAME,
		VIEWER_MODE_HIDDEN_LINE
	};

	ViewController();
	~ViewController();

	osg::Group*		getRootNode()	{ return m_rootnode.get(); }
	osg::Switch*	getModelNode()	{ return m_sw_model.get(); }
	osg::MatrixTransform* getModelTransform() { return m_transform_model.get(); }
	osg::StateSet* getStateSetDefault() { return m_stateset_default.get(); }
	osg::StateSet* getStateSetSelected() { return m_stateset_selected.get(); }
	osg::Material* getDefaultMaterial() { return m_material_default; }
	void toggleSceneLight();
	void toggleModelTransparency();
	void toggleBoundingSphere();
	void setViewerMode( ViewerMode mode );
	ViewerMode getViewerMode() { return m_viewer_mode; }

private:
	osg::ref_ptr<osg::Group>			m_rootnode;
	osg::ref_ptr<osg::Switch>			m_sw_coord_axes;
	osg::ref_ptr<osg::Switch>			m_sw_model;
	osg::ref_ptr<osg::Switch>			m_sw_bound;
	osg::ref_ptr<osg::MatrixTransform>	m_transform_model;
	osg::ref_ptr<osg::MatrixTransform>	m_transform_light;
	bool								m_light_on;
	float								m_shinyness;
	double								m_factor_graphics_scale;
	bool								m_transparent_model;
	ViewerMode							m_viewer_mode;

	osg::ref_ptr<osg::Material>			m_material_default;
	osg::ref_ptr<osg::StateSet>			m_stateset_selected;
	osg::ref_ptr<osg::StateSet>			m_stateset_default;
	osg::ref_ptr<osg::StateSet>			m_stateset_transparent;
};
