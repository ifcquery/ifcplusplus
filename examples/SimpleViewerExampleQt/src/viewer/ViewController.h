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

#include <QObject>
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/Material>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/geometry/GeomUtils.h>

class Orbit3DManipulator;
class QtOSGWidget;

class ViewController
{
public:
	enum ViewerMode
	{
		VIEWER_MODE_SHADED,
		VIEWER_MODE_WIREFRAME,
		VIEWER_MODE_HIDDEN_LINE
	};
	enum ViewerProjection
	{
		PROJECTION_PARALLEL,
		PROJECTION_PERSPECTIVE
	};

	ViewController( Orbit3DManipulator* camera_manip );
	~ViewController();

	osg::Group*		getRootNode()				{ return m_rootnode.get(); }
	osg::Group*		getModelNode()				{ return m_model_node.get(); }
	osg::Group*		getTempNode()				{ return m_temp_node.get(); }
	osg::Group*		getSnapNode()				{ return m_snap_node.get(); }
	osg::Switch*	getCoordinateAxesNode()		{ return m_coordinate_axes_switch.get(); }
	osg::MatrixTransform* getLightTransform()	{ return m_transform_light.get(); }
	osg::StateSet*	getStateSetDefault()		{ return m_stateset_default.get(); }
	osg::Material*	getMaterialSelected()		{ return m_material_selected.get(); }
	osg::Material*	getDefaultMaterial()		{ return m_material_default; }
	osg::StateSet*	getStateSetTransparent()	{ return m_stateset_transparent; }
	osg::Light*		getSunLight()				{ return m_sun_light; }
	osg::Camera*	getHUDCamera()				{ return m_hud_camera; }
	ViewerMode		getViewerMode() { return m_viewer_mode; }
	osgViewer::View* getMainView() { return m_main_view.get(); }
	Orbit3DManipulator* getOrbitManipulator3D() { return m_camera_manip; }
	double getShininess() { return m_shinyness; }
	void toggleSunLight();
	void setSunLightOn( bool on );
	bool isSunLightOn() { return m_light_on; }
	void toggleModelTransparency();
	void setModelTransparent( bool transparent );
	void setViewerMode( ViewerMode mode );
	void setProjection( ViewerProjection p, int width, int height );

	void setGLWidget(QtOSGWidget* glw);
	QtOSGWidget* getGLWidget();

	void setCursor( const QCursor& cursor );
	void restoreCursor();	
	osg::Matrix& getModelScreen() { return m_model_screen; }
	osg::Matrix& getScreenModel() { return m_screen_model; }
	void setModelScreen( osg::Matrix& m ) { m_model_screen.set(m); }
	void setScreenModel( osg::Matrix& m ) { m_screen_model.set(m); }
	void switchCurveRepresentation( osg::Group* grp, bool on );

	void setPointerRay( glm::dvec3& origin, glm::dvec3& direction );
	GeomUtils::Ray					m_current_pointer_ray;
	bool							m_pointer_push_drag = false;
	bool							m_show_curve_representation = true;

private:
	osg::ref_ptr<osg::Group>			m_rootnode;
	osg::ref_ptr<osg::Group>			m_temp_node;
	osg::ref_ptr<osg::Group>			m_snap_node;
	osg::ref_ptr<osg::Switch>			m_coordinate_axes_switch;
	osg::ref_ptr<osg::Group>			m_model_node;
	osg::ref_ptr<osg::MatrixTransform>	m_transform_light;
	osg::ref_ptr<osgViewer::View>		m_main_view;
	QtOSGWidget*						m_gl_widget = nullptr;
	osg::ref_ptr<Orbit3DManipulator>	m_camera_manip;
	osg::ref_ptr<osg::Camera>			m_hud_camera;
	osg::ref_ptr<osg::Material>			m_material_default;
	osg::ref_ptr<osg::Material>			m_material_selected;
	osg::ref_ptr<osg::StateSet>			m_stateset_default;
	osg::ref_ptr<osg::StateSet>			m_stateset_transparent;
	osg::ref_ptr<osg::Light>			m_sun_light;
	osg::Matrix							m_model_screen;
	osg::Matrix							m_screen_model;
	ViewerProjection					m_projection = PROJECTION_PERSPECTIVE;
	ViewerMode							m_viewer_mode = VIEWER_MODE_SHADED;
	bool								m_light_on = false;
	bool								m_transparent_model = false;
	double								m_shinyness = 35.0;
	double								m_factor_graphics_scale = 1.0;
	double								m_near_plane = 0.1;
	double								m_far_plane = 10000.0;
};
