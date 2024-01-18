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

#include <iostream>
#include <iomanip>
#include <QApplication>
#include <osg/BoundsChecking>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/PolygonStipple>
#include <osg/LineStipple>
#include <osg/MatrixTransform>
#include <osg/Hint>
#include <osgViewer/View>
#include <osgFx/Scribe>
#include <osgText/Text>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <IfcRelationship.h>
#include <IfcProduct.h>
#include <ifcpp/geometry/GeometryConverter.h>
#include <ifcpp/geometry/GeomUtils.h>
#include <ifcpp/geometry/ConverterOSG.h>

#include "IfcPlusPlusSystem.h"
#include "ViewerUtil.h"
#include "ViewController.h"
#include "viewer/IntersectionHandler.h"
#include "Orbit3DManipulator.h"

int Orbit3DManipulator::m_minimum_distance_flag_index = allocateRelativeFlag();

Orbit3DManipulator::Orbit3DManipulator( IfcPlusPlusSystem* s, IntersectionHandler* ih, int flags )
	: StandardManipulator(flags), m_system(s), m_intersection_handler( ih )
{
	m_eye.set( 10, 10, 10 );
	m_up.set( 0, 0, 1 );
	m_rotate_center.set( 0, 0, 0 );
	setMinimumDistance( 0.05, true );
	m_wheel_zoom_factor = 0.05;

	initManipulator();
	
	connect( m_system, &IfcPlusPlusSystem::signalModelLoadingDone,	this, &Orbit3DManipulator::slotModelLoadingDone );
}

Orbit3DManipulator::Orbit3DManipulator( const Orbit3DManipulator& om, const osg::CopyOp& copyOp ) : osg::Object(om, copyOp), StandardManipulator( om, copyOp ),
	m_system( om.m_system), m_eye( om.m_eye ), m_lookat( om.m_lookat ), m_rotate_center( om.m_rotate_center ),
	m_wheel_zoom_factor( om.m_wheel_zoom_factor ), m_minimum_distance( om.m_minimum_distance )
{
	initManipulator();
}

Orbit3DManipulator::~Orbit3DManipulator(){}

void Orbit3DManipulator::initManipulator()
{
	setAnimationTime( 0.2 );

	_homeEye.set( m_eye );
	_homeCenter.set( m_lookat );
	_homeUp.set( m_up );
	setAutoComputeHomePosition( false );
	_allowThrow = false;

	m_animation_data = new OrbitAnimationData();
	m_selection_disabled = false;
	m_fovy = 30;

	osg::Vec4f color( 249.f/255, 129.f/255, 12.f/255, 1.f );
	osg::Geode* intersect_geode = new osg::Geode();

	osg::Vec3Array* vertices = new osg::Vec3Array();
	vertices->push_back( osg::Vec3d( 0, 0, -1 ) );
	vertices->push_back( osg::Vec3d( 0, 0, 1 ) );
	
	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( vertices );
	geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size()) );
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->resize( vertices->size(), color );
	colors->setBinding( osg::Array::BIND_PER_VERTEX );
	geometry->setColorArray( colors );
	osg::StateSet* stateset_geom = geometry->getOrCreateStateSet();
	stateset_geom->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	int lineStipplePattern = 0XAAAA;
	osg::ref_ptr<osg::LineStipple> stipple= new osg::LineStipple();
	stipple->setPattern( (short)lineStipplePattern );
	stipple->setFactor( 4 );
	stateset_geom->setAttributeAndModes( stipple, osg::StateAttribute::ON );	
	intersect_geode->addDrawable(geometry);

	// circle
	osg::Vec3Array* vertices_circle = new osg::Vec3Array();
	for( int i=0; i<41; ++i )
	{
		vertices_circle->push_back( osg::Vec3d( sin(M_PI*double(i)*0.05)*0.2, cos(M_PI*double(i)*0.05)*0.2, 0 ) );
	}
	
	osg::Geometry* geometry_circle = new osg::Geometry();
	geometry_circle->setVertexArray( vertices_circle );
	geometry_circle->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices_circle->size()) );
	osg::ref_ptr<osg::Vec4Array> colors_circle = new osg::Vec4Array();
	colors_circle->resize( vertices_circle->size(), color );
	colors_circle->setBinding( osg::Array::BIND_PER_VERTEX );
	geometry_circle->setColorArray( colors_circle );
	osg::StateSet* stateset_circle = geometry_circle->getOrCreateStateSet();
	stateset_circle->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::ref_ptr<osg::Hint> antialias_hint = new osg::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	stateset_circle->setAttributeAndModes(antialias_hint.get(), osg::StateAttribute::ON);
	intersect_geode->addDrawable(geometry_circle);

	osg::Geometry* geometry_thick_line = new osg::Geometry();
	geometry_thick_line->setVertexArray( vertices_circle );
	geometry_thick_line->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices_circle->size()) );
	osg::ref_ptr<osg::Vec4Array> colors_thick_line = new osg::Vec4Array();
	colors_thick_line->setBinding( osg::Array::BIND_PER_VERTEX );
	colors_thick_line->resize( vertices_circle->size(), color );
	geometry_thick_line->setColorArray( colors_thick_line );
	osg::StateSet* stateset_geom_thick_line = geometry_thick_line->getOrCreateStateSet();
	osg::LineWidth* linewidth = new osg::LineWidth();
	linewidth->setWidth(4.0f);
	stateset_geom_thick_line->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
	stateset_geom_thick_line->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	intersect_geode->addDrawable(geometry_thick_line);

	osg::MatrixTransform* cone_transform_self = new osg::MatrixTransform();
	m_intersect_geom_transform = new osg::MatrixTransform();	
	
	cone_transform_self->addChild(intersect_geode);
	m_intersect_geom_transform->addChild(cone_transform_self);
	m_intersect_geom_switch = new osg::Switch();
	m_intersect_geom_switch->setAllChildrenOff();
	m_intersect_geom_switch->addChild( m_intersect_geom_transform );


	m_snap_radius = 0.025;

	{
		osg::Vec3Array* vertices = new osg::Vec3Array();
		for( size_t ii = 0; ii < 21; ++ii )
		{
			float sin_alpha = m_snap_radius*0.5*sin( float( ii ) / 20 * 2 * M_PI );
			float cos_alpha = m_snap_radius*0.5*cos( float( ii ) / 20 * 2 * M_PI );
			vertices->push_back( osg::Vec3( m_snapped_point.x() + sin_alpha, m_snapped_point.y() + cos_alpha, m_snapped_point.z() ) );
		}
		for( size_t ii = 0; ii < 21; ++ii )
		{
			float sin_alpha = m_snap_radius*0.5*sin( float( ii ) / 20 * 2 * M_PI );
			float cos_alpha = m_snap_radius*0.5*cos( float( ii ) / 20 * 2 * M_PI );
			vertices->push_back( osg::Vec3( m_snapped_point.x() + sin_alpha, m_snapped_point.y(), m_snapped_point.z() + cos_alpha ) );
		}
		for( size_t ii = 0; ii < 21; ++ii )
		{
			float sin_alpha = m_snap_radius*0.5*sin( float( ii ) / 20 * 2 * M_PI );
			float cos_alpha = m_snap_radius*0.5*cos( float( ii ) / 20 * 2 * M_PI );
			vertices->push_back( osg::Vec3( m_snapped_point.x(), m_snapped_point.y() + sin_alpha, m_snapped_point.z() + cos_alpha ) );
		}

		vertices->push_back( osg::Vec3( m_snapped_point.x() - m_snap_radius*0.5, m_snapped_point.y(), m_snapped_point.z() ) );
		vertices->push_back( osg::Vec3( m_snapped_point.x() + m_snap_radius*0.5, m_snapped_point.y(), m_snapped_point.z() ) );

		vertices->push_back( osg::Vec3( m_snapped_point.x(), m_snapped_point.y() - m_snap_radius*0.5, m_snapped_point.z() ) );
		vertices->push_back( osg::Vec3( m_snapped_point.x(), m_snapped_point.y() + m_snap_radius*0.5, m_snapped_point.z() ) );

		vertices->push_back( osg::Vec3( m_snapped_point.x(), m_snapped_point.y(), m_snapped_point.z() - m_snap_radius*0.5 ) );
		vertices->push_back( osg::Vec3( m_snapped_point.x(), m_snapped_point.y(), m_snapped_point.z() + m_snap_radius*0.5 ) );

		osg::Geometry* geom = new osg::Geometry();
		geom->setVertexArray( vertices );
		geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, 21 ) );  // 0, ..., 20
		geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 21, 21 ) ); // 21, ..., 41
		geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 42, 21 ) ); // 42, ..., 62
		geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 63, 6 ) );
		geom->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

		osg::Vec4Array* colors = new osg::Vec4Array();
		colors->push_back( osg::Vec4( 0.99, 0.99, 0.1, 0.1 ) );
		colors->setBinding( osg::Array::BIND_OVERALL );
		geom->setColorArray( colors );

		osg::Geode* geode = new osg::Geode();
		geode->addDrawable( geom );

		m_snap_wire_cone = new osg::AutoTransform();
		m_snap_wire_cone->addChild( geode );
	}
}

void Orbit3DManipulator::slotModelLoadingDone()
{

}

void Orbit3DManipulator::setSelectionDisabled( bool disabled )
{
	m_selection_disabled = disabled;
}

void Orbit3DManipulator::setByMatrix( const osg::Matrixd& matrix )
{
	matrix.getLookAt( m_eye, m_lookat, m_up );
}

void Orbit3DManipulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
	setByMatrix( osg::Matrixd::inverse( matrix ) );
}

osg::Matrixd Orbit3DManipulator::getMatrix() const
{
	osg::Matrix m;
	m.makeLookAt( m_eye, m_lookat, m_up );
	return m;
}

osg::Matrixd Orbit3DManipulator::getInverseMatrix() const
{
	osg::Matrix m;
	m.makeLookAt( m_eye, m_lookat, m_up );
	return m;
}

// doc in parent
void Orbit3DManipulator::setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation )
{
	m_eye.set( eye );
	// TODO: implement
	//m_lookat = eye + rotation * osg::Vec3d( 0., 0., -m_distance );
	//m_rotation = rotation;

	// fix current rotation
	if( getVerticalAxisFixed() )
	{
		//fixVerticalAxis( m_lookat, m_rotation, true );
	}
}

void Orbit3DManipulator::getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const
{
	eye.set( m_eye );
	// TODO: implement

}

void Orbit3DManipulator::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
{
	m_eye.set( eye );
	m_lookat.set( center );
	m_up.set( up );
	emit( signalEyeLookatChanged() );
}

void Orbit3DManipulator::getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const
{
	center.set( m_lookat );
	eye.set( m_eye );
	up.set( m_up );
}

void Orbit3DManipulator::setEyeLookat( const osg::Vec3d& eye, const osg::Vec3d& lookat )
{
	m_eye.set( eye );
	m_lookat.set( lookat );
	emit( signalEyeLookatChanged() );
}

void Orbit3DManipulator::setRotateCenter( const osg::Vec3d& center )
{
	m_rotate_center.set( center );
}

void Orbit3DManipulator::computeRayPointer( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return;

	osg::Matrix model_screen( osg::Matrix::identity() );
	// do not apply viewport here ( m_cam->getViewport()->computeWindowMatrix() ). Use normalized screen coords instead (ea.getXnormalized())
	osg::Matrixd pm( view->getCamera()->getProjectionMatrix() );
	osg::Matrixd vm( getInverseMatrix() );

	if( pm.valid() ) model_screen.preMult( pm );
	if( vm.valid() ) model_screen.preMult( vm );

	osg::Matrix screen_model;
	screen_model.invert( model_screen );

	m_system->getViewController()->setModelScreen( model_screen );
	m_system->getViewController()->setScreenModel( screen_model );

	osg::Vec3d ray_pointer_start( screen_model.preMult( osg::Vec3d( ea.getXnormalized(), ea.getYnormalized(), 0.0) ) );
	osg::Vec3d pointer_end = screen_model.preMult( osg::Vec3d( ea.getXnormalized(), ea.getYnormalized(), 0.1) );

	osg::Vec3d ray_direction = pointer_end - ray_pointer_start;
	ray_direction.normalize();

	glm::dvec3 ray_pointer_start_glm(ray_pointer_start.x(), ray_pointer_start.y(), ray_pointer_start.z());
	glm::dvec3 ray_direction_glm(ray_direction.x(), ray_direction.y(), ray_direction.z());
	m_system->getViewController()->setPointerRay( ray_pointer_start_glm, ray_direction_glm );

	double fovy, apect_ratio, zNear, zFar;
	bool perspective = pm.getPerspective( fovy, apect_ratio, zNear, zFar );
	if( perspective )
	{
		m_fovy = fovy;
	}
}

bool Orbit3DManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	bool handled_by_kernel = m_system->handle( ea, aa );

	switch( ea.getEventType() )
	{

	case osgGA::GUIEventAdapter::FRAME:
		return handleFrame( ea, aa );

	case osgGA::GUIEventAdapter::RESIZE:
		return handleResize( ea, aa );

	default:
		break;
	}

	if( ea.getHandled() )
	{
		return false;
	}

	computeRayPointer( ea, aa );

	bool handled = false;
	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		handled = handleMouseMove( ea, aa );
		break;

	case osgGA::GUIEventAdapter::DRAG:
		handled = handleMouseDrag( ea, aa );
		break;

	case osgGA::GUIEventAdapter::PUSH:
		handled = handleMousePush( ea, aa );
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		handled = handleMouseRelease( ea, aa );

		if( m_has_snapped_point )
		{
			if( !m_system->getViewController()->m_pointer_push_drag )
			{
				m_system->notifyCursorCoordinates( m_snapped_point.x(), m_snapped_point.y(), m_snapped_point.z() );
			}
		}

		break;

	case osgGA::GUIEventAdapter::KEYDOWN:
		handled = handleKeyDown( ea, aa );
		break;

	case osgGA::GUIEventAdapter::KEYUP:
		handled = handleKeyUp( ea, aa );
		break;

	case osgGA::GUIEventAdapter::SCROLL:
		if( _flags & PROCESS_MOUSE_WHEEL )
			handled = handleMouseWheel( ea, aa );
		else
			handled = false;
		break;

	default:
		handled = false;
	}

	return handled;
}

bool Orbit3DManipulator::handleFrame( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	double current_frame_time = ea.getTime();

	_delta_frame_time = current_frame_time - _last_frame_time;
	_last_frame_time = current_frame_time;

	unsigned int button_mask = ea.getButtonMask();
	int button = ea.getButton();
	if( button_mask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
		|| button_mask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON
		|| button_mask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
	{
		// some button is currently pressed
	}
	else
	{
		m_intersect_geom_switch->setAllChildrenOff();
	}

	if( _thrown && performMovement( ea, aa ) )
	{
		aa.requestRedraw();
	}

	if( m_animation_data && m_animation_data->_isAnimating )
	{
		if( m_animation_data->_startTime < 0 )
		{
			// animation has been started from some location without acces to a GUIEventAdapter object, so no start time could be set. Do it now.
			m_animation_data->start( current_frame_time );
		}
		performAnimationMovement( ea, aa );
	}

	return false;
}

bool Orbit3DManipulator::handleMouseMove( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	SceneGraphUtils::removeChildren( m_system->getViewController()->getSnapNode() );

	const GeomUtils::Ray& pointer_ray = m_system->getViewController()->m_current_pointer_ray;
	carve::geom::vector<3> ray_point( carve::geom::VECTOR( pointer_ray.origin.x, pointer_ray.origin.y, pointer_ray.origin.z ) );
	carve::geom::vector<3> ray_direction( carve::geom::VECTOR( pointer_ray.direction.x, pointer_ray.direction.y, pointer_ray.direction.z ) );

	osg::Group* model_group = m_system->getViewController()->getModelNode();
	double smallest_distance = DBL_MAX;
	m_snapped_point.set( 0, 0, 0 );
	m_has_snapped_point = false;
	
	m_intersection_handler->intersectGroup( model_group, ray_point, ray_direction, smallest_distance, m_snapped_point );

	
	if( smallest_distance < m_snap_radius )
	{
		m_has_snapped_point = true;

		osg::MatrixTransform* mt = new osg::MatrixTransform();
		mt->addChild( m_snap_wire_cone );
		mt->setMatrix( osg::Matrix::translate( m_snapped_point ) );
		m_system->getViewController()->getSnapNode()->addChild( mt );
		m_system->notifyCursorCoordinates( m_snapped_point.x(), m_snapped_point.y(), m_snapped_point.z() );
	}
	else
	{
		m_system->notifyCursorCoordinates( 0, 0, 0 );
	}

    return false;
}

bool Orbit3DManipulator::handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if( !view )
	{
		return false;
	}

	unsigned int button_mask = ea.getButtonMask();

	m_system->getViewController()->m_pointer_push_drag = false;
	flushMouseEventStack();
	addMouseEvent( ea );


	// TODO: intersect pan plane instead
	osg::Vec3d pointer_intersection;
	
	if( m_intersection_handler->intersectSceneSimple( ea, view, pointer_intersection ) )
	{
		m_rotate_center.set( pointer_intersection );
	}
		
	if( button_mask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON || button_mask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
	{
		m_pan_start.set( ea.getXnormalized(), ea.getYnormalized() );
		m_pan_end.set( ea.getXnormalized(), ea.getYnormalized() );
		m_pan_ray.setRay( &(m_system->getViewController()->m_current_pointer_ray) );

		glm::dvec3 panPlaneNormal(m_rotate_center.x() - m_eye.x(), m_rotate_center.y() - m_eye.y(), m_rotate_center.z() - -m_eye.z());
		panPlaneNormal = glm::normalize( panPlaneNormal );
		m_pan_plane.setPlane( glm::dvec3( m_rotate_center.x(), m_rotate_center.y(), m_rotate_center.z() ), panPlaneNormal);

		m_system->getViewController()->setCursor( Qt::OpenHandCursor );
	}
	
	if( button_mask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
	{
		osg::Vec3d distance_intersect_eye( m_rotate_center - m_eye );
		double distance_eye_intersection = distance_intersect_eye.length();

		if( !m_selection_disabled )
		{
			osg::Matrix mat_scale( osg::Matrix::scale( distance_eye_intersection*0.1, distance_eye_intersection*0.1, distance_eye_intersection*0.1 ) );
			distance_intersect_eye.normalize();
			osg::Matrix mat_rotate( osg::Matrix::rotate( osg::Vec3d( 0, 0, 1 ), distance_intersect_eye ) );
			osg::Matrix mat_translate( osg::Matrix::translate(m_rotate_center) );
			m_intersect_geom_transform->setMatrix(mat_scale*mat_translate );
			m_intersect_geom_switch->setAllChildrenOn();
			m_system->getViewController()->getSnapNode()->addChild( m_intersect_geom_switch );
		}
	}
	
	aa.requestRedraw();
	aa.requestContinuousUpdate( false );
	_thrown = false;

	return false;
}

bool Orbit3DManipulator::handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if( !view )	return false;

	osg::Group* root_node = dynamic_cast<osg::Group*>(view->getScene()->getSceneData());
	
	m_intersect_geom_switch->setAllChildrenOff();
	m_system->getViewController()->restoreCursor();

	if( ea.getButtonMask() == 0 )
	{
		double timeSinceLastRecordEvent = _ga_t0.valid() ? (ea.getTime() - _ga_t0->getTime()) : DBL_MAX;
		if( timeSinceLastRecordEvent > 0.02 )
		{
			flushMouseEventStack();
		}

		if( isMouseMoving() )
		{
			if( performMovement( ea, aa ) && _allowThrow )
			{
				aa.requestRedraw();
				aa.requestContinuousUpdate( true );
				_thrown = true;
				// TODO: fade out throw animation
			}

			return true;
		}
	}
	
	if( !m_system->getViewController()->m_pointer_push_drag && !m_selection_disabled )
	{
		// select object
		bool intersection_geometry_found = m_intersection_handler->intersectSceneSelect( ea, view );

		if( !intersection_geometry_found )
		{
			// click to background -> unselect all
			m_system->clearSelection();
			
		}
	}

	m_system->getViewController()->m_pointer_push_drag = false;

	flushMouseEventStack();
	addMouseEvent( ea );
	if( performMovement( ea, aa ) )
	{
		aa.requestRedraw();
	}
	aa.requestContinuousUpdate( false );
	_thrown = false;

	return true;
}


bool Orbit3DManipulator::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	// TODO: increase scroll factor when eye is far away from model bounding sphere
	double scroll_distance_factor = ( m_eye - m_rotate_center ).length()*0.1;

	switch( sm )
	{
		// mouse scroll up event
	case osgGA::GUIEventAdapter::SCROLL_UP:
		{
			// perform zoom
			zoomCamera( m_wheel_zoom_factor*scroll_distance_factor );
			aa.requestRedraw();
			aa.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// mouse scroll down event
	case osgGA::GUIEventAdapter::SCROLL_DOWN:
		{
			// perform zoom
			zoomCamera( -m_wheel_zoom_factor*scroll_distance_factor );
			aa.requestRedraw();
			aa.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// unhandled mouse scrolling motion
	default:
		return false;
	}
}

bool Orbit3DManipulator::handleMouseDrag( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	addMouseEvent( ea );

	m_system->getViewController()->m_pointer_push_drag = true;
	if( performMovement( ea, aa ) )
	{
		aa.requestRedraw();
	}

	aa.requestContinuousUpdate( false );
	_thrown = false;

	return true;
}

bool Orbit3DManipulator::performMovement( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if( _ga_t0.get() == nullptr || _ga_t1.get() == nullptr )
	{
		// not yet enough pointer events to determine a delta
		return false;
	}

	// get delta time
	double eventTimeDelta = _ga_t0->getTime() - _ga_t1->getTime();
	if( eventTimeDelta < 0. )
	{
		OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
		eventTimeDelta = 0.;
	}

	// get deltaX and deltaY
	float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
	float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();
	float scale = getThrowScale( eventTimeDelta );
	dx = dx*scale;
	dy = dy*scale;

	// return if there is no movement.
	if( dx == 0. && dy == 0. )
	{
		return false;
	}

	// call appropriate methods
	unsigned int button_mask = _ga_t1->getButtonMask();
	if( button_mask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
	{
		rotateCamera( dx, dy );
		return true;
	}
	else if( button_mask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON || button_mask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
	{
		// pan model
		m_system->getViewController()->setCursor( Qt::ClosedHandCursor );
		m_pan_end.set( ea.getXnormalized(), ea.getYnormalized() );
		panCamera( dx, dy, ea, aa );
		
		return true;
	}

	return false;
}

bool Orbit3DManipulator::performMouseDeltaMovement( const float dx, const float dy )
{
	rotateCamera( dx, dy );
	return true;
}

void Orbit3DManipulator::rotateCamera( float dx, float dy )
{
	if( dx != 0.0 || dy != 0.0 )
	{
		// yaw
		osg::CoordinateFrame coordinate_frame = getCoordinateFrame( m_lookat );
		osg::Vec3d yaw_axis = getUpVector( coordinate_frame );

		yaw_axis.normalize();

		// lookat is the center of the screen. rotation center can be different (intersection point)
		osg::Vec3d center_eye = m_eye - m_rotate_center;
		osg::Vec3d center_lookat = m_lookat - m_rotate_center;
		osg::Vec3d eye_lookat = m_lookat - m_eye;

		// pitch
		osg::Vec3d pitch_axis(0,0,1);
		pitch_axis = pitch_axis ^ center_eye;
		pitch_axis.normalize();

		// apply yaw and pitch around the rotation center
		osg::Quat quatpitch( dy*2.2, pitch_axis );
		osg::Vec3d center_eye_pitch = quatpitch * center_eye;
		osg::Vec3d center_lookat_pitch = quatpitch * center_lookat;

		osg::Quat quatyaw( -dx*5.2, yaw_axis );
		osg::Vec3d center_eye_new = quatyaw*center_eye_pitch;
		osg::Vec3d center_lookat_new = quatyaw*center_lookat_pitch;

		m_eye.set( m_rotate_center + center_eye_new );
		m_lookat.set( m_rotate_center + center_lookat_new );

		emit( signalEyeLookatChanged() );
	}
};

void Orbit3DManipulator::panCamera( const float dx, const float dy, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	const GeomUtils::Ray& current_pan_ray = m_system->getViewController()->m_current_pointer_ray;
	osg::Vec2d mouse_change = m_pan_end - m_pan_start;
	double length_mouse_move = mouse_change.length2();
	if( length_mouse_move > 0.00001 )
	{
		double intersectionDistance = 0;
		bool planeIntersects = GeomUtils::intersectRayPlane(m_pan_ray.origin, m_pan_ray.direction, m_pan_plane.getPosition(), m_pan_plane.getNormal(), intersectionDistance);

		if( planeIntersects )
		{
			glm::dvec3 intersect_point = m_pan_ray.origin + intersectionDistance*m_pan_ray.direction;
			
			double intersectionDistance = 0;
			bool planeIntersects2 = GeomUtils::intersectRayPlane(current_pan_ray.origin, current_pan_ray.direction, m_pan_plane.getPosition(), m_pan_plane.getNormal(), intersectionDistance);

			if( planeIntersects2 )
			{
				glm::dvec3 intersect_point2 = current_pan_ray.origin + intersectionDistance*current_pan_ray.direction;
		       	glm::dvec3 diff = intersect_point - intersect_point2;
		       	m_eye += osg::Vec3d( diff.x, diff.y, diff.z );
				m_lookat += osg::Vec3d( diff.x, diff.y, diff.z );
		    }
	
			m_pan_start = m_pan_end;
		}
		emit( signalEyeLookatChanged() );
	}
}

void Orbit3DManipulator::zoomCamera( const float dy )
{
	// push camera forwared along mouse ray
	glm::dvec3 zoom_direction = m_system->getViewController()->m_current_pointer_ray.direction;
	zoom_direction = glm::normalize(zoom_direction);
	zoom_direction *= dy*100;
	m_eye += osg::Vec3d( zoom_direction.x, zoom_direction.y, zoom_direction.z );
	m_lookat += osg::Vec3d( zoom_direction.x, zoom_direction.y, zoom_direction.z );

	emit( signalEyeLookatChanged() );
}

////////////////////////////////////////////////////// animation //////////////////////////////////////////////////////////////////
void Orbit3DManipulator::setAnimationData( OrbitAnimationData* anim_data )
{
	m_animation_data = anim_data;
	//m_animation_data->m_start_eye = anim_data->m_start_eye;
	//m_animation_data->m_start_lookat = anim_data->m_start_lookat;
	//m_animation_data->m_start_up = anim_data->m_start_up;
	//m_animation_data->m_target_eye = anim_data->m_target_eye;
	//m_animation_data->m_target_lookat = anim_data->m_target_lookat;
	//m_animation_data->m_target_up = anim_data->m_target_up;
}

void Orbit3DManipulator::applyAnimationStep( const double currentProgress, const double prevProgress )
{
	osg::Vec3d new_eye( m_animation_data->m_start_eye + (m_animation_data->m_target_eye - m_animation_data->m_start_eye) * (currentProgress) );
	osg::Vec3d new_lookat( m_animation_data->m_start_lookat + (m_animation_data->m_target_lookat - m_animation_data->m_start_lookat) * (currentProgress) );
	osg::Vec3d new_up( m_animation_data->m_start_up + (m_animation_data->m_target_up - m_animation_data->m_start_up) * (currentProgress) );

	m_eye.set( new_eye );
	m_lookat.set( new_lookat );
	m_up.set( new_up );
	emit( signalEyeLookatChanged() );
}


Orbit3DManipulator::OrbitAnimationData::OrbitAnimationData() : AnimationData()
{
}

void Orbit3DManipulator::OrbitAnimationData::start( const double startTime )
{
	AnimationData::start( startTime );
	_isAnimating = true;
}

void Orbit3DManipulator::setAnimationTime( const double t )
{
    if( t <= 0. )
    {
        finishAnimation();
		m_animation_data = nullptr;
        return;
    }

    if( !m_animation_data )
    {
		allocAnimationData();
	}

    m_animation_data->_animationTime = t;
}

bool Orbit3DManipulator::performAnimationMovement( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	double f = (ea.getTime() - m_animation_data->_startTime) / m_animation_data->_animationTime;
    if( f >= 1. )
    {
        f = 1.;
        m_animation_data->_isAnimating = false;
        if( !_thrown )
        {
			aa.requestContinuousUpdate( false );
		}
    }

    applyAnimationStep( f, m_animation_data->_phase );

    m_animation_data->_phase = f;
    aa.requestRedraw();

    return m_animation_data->_isAnimating;
}

bool Orbit3DManipulator::isAnimating() const
{
	if( m_animation_data )
	{
		 return m_animation_data->_isAnimating;
	}
    else
    {
		return false;
	}
}

osgUtil::SceneView::FusionDistanceMode Orbit3DManipulator::getFusionDistanceMode() const
{
	return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
}

float Orbit3DManipulator::getFusionDistanceValue() const
{
	return 0;//m_distance;
}

void Orbit3DManipulator::setWheelZoomFactor( double wheelZoomFactor )
{
	m_wheel_zoom_factor = wheelZoomFactor;
}

void Orbit3DManipulator::setMinimumDistance( const double& minimumDistance, bool relativeToModelSize )
{
	m_minimum_distance = minimumDistance;
	setRelativeFlag( m_minimum_distance_flag_index, relativeToModelSize );
}

double Orbit3DManipulator::getMinimumDistance( bool *relativeToModelSize ) const
{
	if( relativeToModelSize )
	{
		*relativeToModelSize = getRelativeFlag( m_minimum_distance_flag_index );
	}

	return m_minimum_distance;
}

void Orbit3DManipulator::zoomToHome( double animation_time )
{
	zoomToBoundingSphere( osg::BoundingSphere( osg::Vec3f( 0, 0, 0 ), 2.f ), animation_time );
}

void Orbit3DManipulator::zoomToBoundingSphere( const osg::BoundingSphere& bs, double _animation_time )
{
	double animation_time = 0.4;
	if( _animation_time > 0 )
	{
		animation_time = _animation_time;
	}

	osg::Vec3f bs_center( bs.center() );
	double bs_radius = bs.radius();
	if( bs_radius <= 0.5 )
	{
		bs_radius = 0.5;
	}

	m_rotate_center.set( bs.center() );

	osg::Vec3d lookat_eye( m_eye - m_lookat );
	lookat_eye.normalize();

	// define animation path
	if( m_animation_data )
	{
		osg::Vec3d target_lookat( bs.center() );
		double d_eye = bs_radius/sin( osg::DegreesToRadians(m_fovy*0.5) );
		osg::Vec3d target_eye( target_lookat + lookat_eye*d_eye );

		m_animation_data->m_start_eye.set( m_eye );
		m_animation_data->m_start_lookat.set( m_lookat );
		m_animation_data->m_start_up.set( m_up );

		m_animation_data->m_target_eye.set( target_eye );
		m_animation_data->m_target_lookat.set( target_lookat );
		m_animation_data->m_target_up.set( m_up );

		m_animation_data->_animationTime = animation_time;
		m_animation_data->start( -1 );
	}
}
