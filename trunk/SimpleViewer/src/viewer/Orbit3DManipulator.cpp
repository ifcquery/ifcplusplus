
#include <iostream>
#include <iomanip>
#include <QApplication>
#include <osg/ShapeDrawable>
#include <osgUtil/IntersectionVisitor>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osgViewer/View>
#include <osgFX/Scribe>
#include <osgText/Text>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/model/IfcPPException.h>
#include <ifcpp/reader/IfcStepReader.h>
#include <ifcpp/writer/IfcStepWriter.h>
#include <ifcpp/IFC4/include/IfcProduct.h>
#include <ifcpp/IFC4/include/IfcRelationship.h>
#include <ifcppgeometry/ReaderWriterIFC.h>

#include "IfcPlusPlusSystem.h"
#include "ViewController.h"
#include "ViewerUtil.h"
#include "Orbit3DManipulator.h"


int Orbit3DManipulator::m_minimum_distance_flag_index = allocateRelativeFlag();

Orbit3DManipulator::Orbit3DManipulator( IfcPlusPlusSystem* sys, int flags ) : StandardManipulator( flags ), m_system(sys)
{
	m_eye.set( 10, 10, 10 );
	m_up.set( 0, 0, 1 );
	m_rotate_center.set( 0, 0, 0 );
	setMinimumDistance( 0.05, true );
	m_wheel_zoom_factor = 0.05;

	initManipulator();
}


/// Constructor.
Orbit3DManipulator::Orbit3DManipulator( const Orbit3DManipulator& om, const osg::CopyOp& copyOp ) : osg::Object(om, copyOp), StandardManipulator( om, copyOp ),
	m_system( om.m_system), m_eye( om.m_eye ), m_lookat( om.m_lookat ), m_rotate_center( om.m_rotate_center ), 
	m_wheel_zoom_factor( om.m_wheel_zoom_factor ), m_minimum_distance( om.m_minimum_distance )
{
	initManipulator();
}

void Orbit3DManipulator::initManipulator()
{
	setAnimationTime( 0.2 );

	_homeEye.set( m_eye );
	_homeCenter.set( m_lookat );
	_homeUp.set( m_up );
	setAutoComputeHomePosition( false );
	_allowThrow = false;

	m_animation_data = new OrbitAnimationData();
	m_intersect_hit_geometry = false;
	m_control_key_down = false;
	m_fovy = 30;

}

/** Set the position of the manipulator using a 4x4 matrix.*/
void Orbit3DManipulator::setByMatrix( const osg::Matrixd& matrix )
{
	matrix.getLookAt( m_eye, m_lookat, m_up );
}


/** Set the position of the manipulator using a 4x4 matrix.*/
void Orbit3DManipulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
	setByMatrix( osg::Matrixd::inverse( matrix ) );
}


/** Get the position of the manipulator as 4x4 matrix.*/
osg::Matrixd Orbit3DManipulator::getMatrix() const
{
	osg::Matrix m;
	m.makeLookAt( m_eye, m_lookat, m_up );
	return m;
}


/** Get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
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


// doc in parent
void Orbit3DManipulator::getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const
{
	eye.set( m_eye );
	// TODO: implement

}


// doc in parent
void Orbit3DManipulator::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
{
	m_eye.set( eye );
	m_lookat.set( center );
	m_up.set( up );
}


// doc in parent
void Orbit3DManipulator::getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const
{
	center.set( m_lookat );
	eye.set( m_eye );
	up.set( m_up );
}

void Orbit3DManipulator::computeRayPointer( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return;

	m_model_screen.makeIdentity();
	// do not apply viewport here ( m_cam->getViewport()->computeWindowMatrix() ). Use normalized screen coords instead (ea.getXnormalized())
	osg::Matrixd pm( view->getCamera()->getProjectionMatrix() );
	osg::Matrixd vm( getInverseMatrix() );

	if( pm.valid() ) m_model_screen.preMult( pm );
	if( vm.valid() ) m_model_screen.preMult( vm );

	m_screen_model.invert( m_model_screen );

	m_ray_pointer_start.set( m_screen_model.preMult( osg::Vec3d( ea.getXnormalized(), ea.getYnormalized(), 0.0) ) );
	osg::Vec3d pointer_end = m_screen_model.preMult( osg::Vec3d( ea.getXnormalized(), ea.getYnormalized(), 0.1) );

	osg::Vec3d ray_direction = pointer_end - m_ray_pointer_start;
	ray_direction.normalize();
	m_ray_pointer_direction.set( ray_direction );

	double fovy, apect_ratio, zNear, zFar;
	bool perspective = pm.getPerspective( fovy, apect_ratio, zNear, zFar );
	if( perspective )
	{
		m_fovy = fovy;
	}
	
}


/** Handles events. Returns true if handled, false otherwise.*/
bool Orbit3DManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
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
		break;

	case osgGA::GUIEventAdapter::KEYDOWN:
		handled = handleKeyDown( ea, aa );
		break;

	case osgGA::GUIEventAdapter::KEYUP:
		m_control_key_down = false;
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


/// Handles GUIEventAdapter::FRAME event.
bool Orbit3DManipulator::handleFrame( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	double current_frame_time = ea.getTime();

	_delta_frame_time = current_frame_time - _last_frame_time;
	_last_frame_time = current_frame_time;

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
    return false;
}


// mouse button has been pushed down
bool Orbit3DManipulator::handleMousePush( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if( !view )
	{
		return false;
	}

	m_pointer_push_drag = false;
	flushMouseEventStack();
	addMouseEvent( ea );
	m_ga_pointer_push = &ea;

	intersectSceneRotateCenter( ea, view );

	unsigned int buttonMask = ea.getButtonMask();
	if( buttonMask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON || buttonMask == (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) )
	{
		m_pan_point.set( m_pointer_intersection );
	}
	
	if( buttonMask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
	{
		osg::Vec3d distance_intersect_eye( m_pointer_intersection - m_eye );
		double distance_eye_intersection = distance_intersect_eye.length();

		// rotate the intersection cone into mouse ray direction
		if( m_intersect_hit_geometry )
		{

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

	// if movement is very small, it is probably unintended -> handle as no drag
	/*int dx_screen = 0;
	int dy_screen = 0;
	double time_since_pointer_push = DBL_MAX;
	if( m_ga_pointer_push.valid() )// && _ga_t0.valid() )
	{
		dx_screen = m_ga_pointer_push->getX() - ea.getX();// _ga_t0->getX();
		dy_screen = m_ga_pointer_push->getY() - ea.getX();//_ga_t0->getY();
		time_since_pointer_push = (ea.getTime() - m_ga_pointer_push->getTime());
	}
	int mouse_movement_length2 = dx_screen*dx_screen + dy_screen*dy_screen;
	
	if( time_since_pointer_push < 0.05 )//if( mouse_movement_length2 < 4 ) // TODO: check also time (if time is short, drag is probably not intended)
	{

	}
	*/
	
	
	if( !m_pointer_push_drag )
	{
		// select object
		bool intersection_geometry_found = intersectSceneSelect( ea, view );

		if( !intersection_geometry_found )
		{
			// click to background -> unselect all
			if( m_system != nullptr )
			{
				m_system->clearSelection();
			}
			
		}
	}
	//}
	m_pointer_push_drag = false;


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

bool Orbit3DManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if( !view )	return false;

	osg::Group* root_node = dynamic_cast<osg::Group*>(view->getScene()->getSceneData());
	if( !root_node ) return false;

	const int key = ea.getKey();
	if( key == osgGA::GUIEventAdapter::KEY_Control_L || key == osgGA::GUIEventAdapter::KEY_Control_R )
	{
		m_control_key_down = true;
	}

	return false;
}

//#define POLYTOPE_INTERSECTOR
bool Orbit3DManipulator::intersectSceneRotateCenter( const osgGA::GUIEventAdapter& ea, osgViewer::View* view )
{
	m_intersect_hit_geometry = false;
#ifdef POLYTOPE_INTERSECTOR
	double mx = ea.getXnormalized();
	double my = ea.getYnormalized();
	double w = 0.005;
	double h = 0.005;
	osg::ref_ptr<osgUtil::PolytopeIntersector> picker = new osgUtil::PolytopeIntersector( osgUtil::Intersector::PROJECTION, mx-w, my-h, mx+w, my+h );
#else
	osg::ref_ptr<osgUtil::LineSegmentIntersector> picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, ea.getXnormalized(),ea.getYnormalized() );
#endif
	//picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );
	picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
	osgUtil::IntersectionVisitor iv( picker.get() );
	osg::Camera* cam = view->getCamera();

	osg::Group* root_node = nullptr;
	if( m_system )
	{
		root_node = m_system->getViewController()->getRootNode();
	}
	else
	{
		root_node = (osg::Group*)view->getSceneData();
	}

	if( root_node == nullptr )
	{
		return false;
	}

	//osg::Group* root_node = m_system->getViewController()->getRootNode();
	iv.apply( *cam );
	//iv.apply( root_node );

	if( picker->containsIntersections() )
	{
#ifdef POLYTOPE_INTERSECTOR
		osgUtil::PolytopeIntersector::Intersection intersection = picker->getFirstIntersection();
#else
		osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
#endif

		osg::NodePath& nodePath = intersection.nodePath;
		for( unsigned int i=0; i<nodePath.size(); ++i )
		{
			osg::Node* node = nodePath[nodePath.size()-i-1];
			//const std::string node_name = node->getName();

			// check if picked object is a representation of an IfcProduct
			//if( node_name.length() == 0 ) continue;
			//if( node_name.substr( 0, 9 ).compare( "intersect" ) == 0 ) continue;

			m_pointer_intersection.set( intersection.getWorldIntersectPoint() );
			// set rotate center to intersection point
			m_rotate_center.set( m_pointer_intersection );

			//intersection_geometry_found = true;
			m_intersect_hit_geometry = true;
		}
	}

	if( !m_intersect_hit_geometry )
	{
		// no geometry has been hit, so intersect with XY-plane
		// TODO: when angle between pointer and xy-plane is small, take the xz or yz plane
		// TODO: intersect with 3 perpendicular planes in the center of the bounding sphere. take the closest as rotation point
		// TODO: if model bounding sphere is out of sight, set rotation center such that it is intuitive to find back to the model (center of bounding sphere)
		
		//intersectRayWithPlane( m_ray_pointer_start, m_ray_pointer_direction, IntersectionPlane::XY_PLANE, m_pointer_intersection );
	}

	return false;
}

bool Orbit3DManipulator::intersectSceneSelect( const osgGA::GUIEventAdapter& ea, osgViewer::View* view )
{
	if( m_system == nullptr )
	{
		return false;
	}

#ifdef POLYTOPE_INTERSECTOR
	double mx = ea.getXnormalized();
	double my = ea.getYnormalized();
	double w = 0.005;
	double h = 0.005;
	osg::ref_ptr<osgUtil::PolytopeIntersector> picker = new osgUtil::PolytopeIntersector( osgUtil::Intersector::PROJECTION, mx-w, my-h, mx+w, my+h );
#else
	osg::ref_ptr<osgUtil::LineSegmentIntersector> picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, ea.getXnormalized(),ea.getYnormalized() );
#endif
	//picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );
	picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );
	osgUtil::IntersectionVisitor iv( picker.get() );
	osg::Camera* cam = view->getCamera();
	view->getScene();
	osg::Group* model_node = m_system->getViewController()->getModelNode();
	iv.apply( *cam );//, model_node );

	bool intersection_geometry_found = false;
	if( picker->containsIntersections() )
	{
#ifdef POLYTOPE_INTERSECTOR
		osgUtil::PolytopeIntersector::Intersection intersection = picker->getFirstIntersection();
#else
		osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
#endif

		osg::NodePath& nodePath = intersection.nodePath;
		for( unsigned int i=0; i<nodePath.size(); ++i )
		{
			osg::Node* node = nodePath[nodePath.size()-i-1];
			const std::string node_name = node->getName();

			m_pointer_intersection.set( intersection.getWorldIntersectPoint() );
			intersection_geometry_found = true;

			// check if picked object is a representation of an IfcProduct
			if( node_name.length() == 0 ) continue;
			if( node_name.at(0) != '#' ) continue;
			if( node_name.compare( 0, 9, "intersect" ) == 0 ) continue;

			osg::Group* group = dynamic_cast<osg::Group*>( node );
			if( !group )
			{
				continue;
			}
			
			// extract entity id
			std::string node_name_id = node_name.substr( 1 );
			size_t last_index = node_name_id.find_first_not_of("0123456789");
			std::string id_str = node_name_id.substr( 0, last_index );
			const int id = atoi( id_str.c_str() );

			const std::map<int, shared_ptr<selectedEntity> >& map_selected = m_system->getSelectedObjects();
			std::map<int, shared_ptr<selectedEntity> >::const_iterator it_selected = map_selected.find( id );

			if( it_selected != map_selected.end() )
			{
				shared_ptr<selectedEntity> selected_entity = it_selected->second;
				// is already selected, so deselect
				m_system->setObjectSelected( selected_entity->entity, false, selected_entity->osg_group );
				return true;
			}
			else
			{
				// select
				shared_ptr<IfcPPModel> ifc_model = m_system->getIfcModel();
				const std::map<int,shared_ptr<IfcPPEntity> >& map_ifc_objects = ifc_model->getMapIfcObjects();
				std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_find = map_ifc_objects.find(id);
				if( it_find != map_ifc_objects.end() )
				{
					shared_ptr<IfcPPEntity> entitiy_selected = it_find->second;
					if( !m_control_key_down )
					{
						m_system->clearSelection();
					}
					m_system->setObjectSelected( entitiy_selected, true, group );
				}
				return true;
			}


		}
	}

	if( !intersection_geometry_found )
	{
		// no geometry has been hit, so intersect with XY-plane
		//intersectRayWithPlane( m_ray_pointer_start, m_ray_pointer_direction, IntersectionPlane::XY_PLANE, m_pointer_intersection );
	}

	return intersection_geometry_found;
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

	m_pointer_push_drag = true;
	if( performMovement( ea, aa ) )
	{
		aa.requestRedraw();
	}

	aa.requestContinuousUpdate( false );
	_thrown = false;

	return true;
}

/// Make movement step of manipulator. Returns true if any movement was made.
bool Orbit3DManipulator::performMovement( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	// return if less then two events have been added
	if( _ga_t0.get() == nullptr || _ga_t1.get() == nullptr )
	{
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
	unsigned int buttonMask = _ga_t1->getButtonMask();
	if( buttonMask == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
	{
		rotateCamera( dx, dy );
		return true;
	}
	else if( buttonMask == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON || buttonMask == (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) )
	{
		// pan model
		float scale = getThrowScale( eventTimeDelta );
		panCamera( dx, dy, ea, aa );
		return true;
	}
	else if( buttonMask == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON )
	{
		zoomCamera( dy );
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
	}
};

/** Moves the camera such that the model remains under the cursor.*/
void Orbit3DManipulator::panCamera( const float dx, const float dy, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if( !view )
	{
		return;
	}
	
#if 0 
	osg::Matrix matrix;
	matrix.getLookAt( m_eye, m_lookat, m_up );
	matrix = matrix*osg::Matrix::translate( dx, dy, 0 );
	// set eye, lookat, up from matrix
	matrix.getLookAt( m_eye, m_lookat, m_up );

	return;
#endif

	
	//osg::Vec3d intersect_point_screen = screen_model.preMult( m_pointer_intersection );
	osg::Vec3d p1 = m_screen_model.preMult( osg::Vec3d( _ga_t0->getXnormalized(), _ga_t0->getYnormalized(), 0 ) );
	osg::Vec3d p2 = m_screen_model.preMult( osg::Vec3d( _ga_t1->getXnormalized(), _ga_t1->getYnormalized(), 0 ) );
	osg::Vec3d n = p2 - p1;

	double length = (m_pointer_intersection - m_eye).length();
	n *= length*1.25;

	m_eye.set( m_eye + n );
	m_lookat.set( m_lookat + n );
	return;


}


/** Changes the distance of camera to the focal center.
If pushForwardIfNeeded is true and minimumDistance is reached, the focal center is moved forward. Otherwise, distance is limited to its minimum value.
\sa Orbit3DManipulator::setMinimumDistance
*/
void Orbit3DManipulator::zoomCamera( const float dy )
{
	// push camera forwared along mouse ray
	osg::Vec3d zoom_direction = m_ray_pointer_direction;
	zoom_direction.normalize();
	zoom_direction *= dy*100;
	m_eye += zoom_direction;
	m_lookat += zoom_direction;


#if 0
	if( m_distance*scale > minDist )
	{
		// regular zoom
		m_distance *= scale;
	}
	else
	{
		if( pushForwardIfNeeded )
		{
			// push the camera forward
			float scale = -m_distance;
			osg::Matrixd rotation_matrix( m_rotation );
			osg::Vec3d dv = (osg::Vec3d( 0.0f, 0.0f, -1.0f ) * rotation_matrix) * (dy * scale);
			m_center += dv;
		}
		else
		{
			// set distance on its minimum value
			m_distance = minDist;
		}
	}
#endif
}





////////////////////////////////////////////////////// animation //////////////////////////////////////////////////////////////////
void Orbit3DManipulator::applyAnimationStep( const double currentProgress, const double prevProgress )
{
	osg::Vec3d new_eye( m_animation_data->m_start_eye + (m_animation_data->m_target_eye - m_animation_data->m_start_eye) * (currentProgress) );
	osg::Vec3d new_lookat( m_animation_data->m_start_lookat + (m_animation_data->m_target_lookat - m_animation_data->m_start_lookat) * (currentProgress) );
	osg::Vec3d new_up( m_animation_data->m_start_up + (m_animation_data->m_target_up - m_animation_data->m_start_up) * (currentProgress) );

	m_eye.set( new_eye );
	m_lookat.set( new_lookat );
	m_up.set( new_up );
}


Orbit3DManipulator::OrbitAnimationData::OrbitAnimationData() : AnimationData()
{
}

void Orbit3DManipulator::OrbitAnimationData::start( const double startTime )
{
	AnimationData::start( startTime );
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
        allocAnimationData();

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
            aa.requestContinuousUpdate( false );
    }

    applyAnimationStep( f, m_animation_data->_phase );

    m_animation_data->_phase = f;
    aa.requestRedraw();

    return m_animation_data->_isAnimating;
}

bool Orbit3DManipulator::isAnimating() const
{
    if( m_animation_data )
        return m_animation_data->_isAnimating;
    else
        return false;
}


/** Get the FusionDistanceMode. Used by SceneView for setting up stereo convergence.*/
osgUtil::SceneView::FusionDistanceMode Orbit3DManipulator::getFusionDistanceMode() const
{
	return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
}

/** Get the FusionDistanceValue. Used by SceneView for setting up stereo convergence.*/
float Orbit3DManipulator::getFusionDistanceValue() const
{
	return 0;//m_distance;
}


/** Set the mouse wheel zoom factor.
The amount of camera movement on each mouse wheel event is computed as the current distance to the center multiplied by this factor.
For example, value of 0.1 will short distance to center by 10% on each wheel up event.
Use negative value for reverse mouse wheel direction.*/
void Orbit3DManipulator::setWheelZoomFactor( double wheelZoomFactor )
{
	m_wheel_zoom_factor = wheelZoomFactor;
}


/** Set the minimum distance of the eye point from the center before the center is pushed forward.*/
void Orbit3DManipulator::setMinimumDistance( const double& minimumDistance, bool relativeToModelSize )
{
	m_minimum_distance = minimumDistance;
	setRelativeFlag( m_minimum_distance_flag_index, relativeToModelSize );
}


/** Get the minimum distance of the eye point from the center before the center is pushed forward.*/
double Orbit3DManipulator::getMinimumDistance( bool *relativeToModelSize ) const
{
	if( relativeToModelSize )
	{
		*relativeToModelSize = getRelativeFlag( m_minimum_distance_flag_index );
	}

	return m_minimum_distance;
}


void Orbit3DManipulator::zoomToBoundingSphere( const osg::BoundingSphere& bs, double ratio_w )
{
	osg::Vec3f bs_center( bs.center() );
	double bs_radius = bs.radius();
	if( bs_radius <= 0.0 )
	{
		bs_radius = 2.0;
		bs_center._v[2] = 0.5;
	}


	m_rotate_center.set( bs.center() );

	osg::Vec3d eye_lookat(m_eye - m_lookat);
	eye_lookat.normalize();


	// define animation path
	if( m_animation_data )
	{
		osg::Vec3d target_lookat( bs.center() );

		double d_eye = bs_radius/sin( osg::DegreesToRadians(m_fovy*0.5) );
		osg::Vec3d target_eye( target_lookat + eye_lookat*d_eye );

		//target_lookat.set( 0, 0, 0 );
		//target_eye.set( 5, 0, 5 );

		m_animation_data->m_start_eye.set( m_eye );
		m_animation_data->m_start_lookat.set( m_lookat );
		m_animation_data->m_start_up.set( m_up );

		m_animation_data->m_target_eye.set( target_eye );
		m_animation_data->m_target_lookat.set( target_lookat );
		m_animation_data->m_target_up.set( m_up );

		m_animation_data->_animationTime = 0.6;
		m_animation_data->start( -1 );
	}
}
