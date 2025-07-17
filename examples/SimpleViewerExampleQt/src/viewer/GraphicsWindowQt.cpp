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
#include <osg/Version>
#include <osgViewer/ViewerBase>
#include <QtGui/QInputEvent>
#include <QtGui/QOpenGLContext>

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
	#define USE_GESTURES
	#include <QtWidgets/QGestureEvent>
	#include <QtWidgets/QGesture>
#endif

#include "GraphicsWindowQt.h"

QtOSGWidget::QtOSGWidget( QWidget* parent, Qt::WindowFlags f ) : QOpenGLWidget( parent, f )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
	m_device_pixel_ratio = devicePixelRatio();
#endif
	m_main_view = new osgViewer::View();
	m_viewer = new osgViewer::CompositeViewer();

	m_key_map[Qt::Key_Escape] = osgGA::GUIEventAdapter::KEY_Escape;
	m_key_map[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
	m_key_map[Qt::Key_Home] = osgGA::GUIEventAdapter::KEY_Home;
	m_key_map[Qt::Key_Enter] = osgGA::GUIEventAdapter::KEY_KP_Enter;
	m_key_map[Qt::Key_End] = osgGA::GUIEventAdapter::KEY_End;
	m_key_map[Qt::Key_Return] = osgGA::GUIEventAdapter::KEY_Return;
	m_key_map[Qt::Key_PageUp] = osgGA::GUIEventAdapter::KEY_Page_Up;
	m_key_map[Qt::Key_PageDown] = osgGA::GUIEventAdapter::KEY_Page_Down;
	m_key_map[Qt::Key_Left] = osgGA::GUIEventAdapter::KEY_Left;
	m_key_map[Qt::Key_Right] = osgGA::GUIEventAdapter::KEY_Right;
	m_key_map[Qt::Key_Up] = osgGA::GUIEventAdapter::KEY_Up;
	m_key_map[Qt::Key_Down] = osgGA::GUIEventAdapter::KEY_Down;
	m_key_map[Qt::Key_Backspace] = osgGA::GUIEventAdapter::KEY_BackSpace;
	m_key_map[Qt::Key_Tab] = osgGA::GUIEventAdapter::KEY_Tab;
	m_key_map[Qt::Key_Space] = osgGA::GUIEventAdapter::KEY_Space;
	m_key_map[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
	m_key_map[Qt::Key_Alt] = osgGA::GUIEventAdapter::KEY_Alt_L;
	m_key_map[Qt::Key_Shift] = osgGA::GUIEventAdapter::KEY_Shift_L;
	m_key_map[Qt::Key_Control] = osgGA::GUIEventAdapter::KEY_Control_L;
	m_key_map[Qt::Key_Meta] = osgGA::GUIEventAdapter::KEY_Meta_L;
	m_key_map[Qt::Key_F1] = osgGA::GUIEventAdapter::KEY_F1;
	m_key_map[Qt::Key_F2] = osgGA::GUIEventAdapter::KEY_F2;
	m_key_map[Qt::Key_F3] = osgGA::GUIEventAdapter::KEY_F3;
	m_key_map[Qt::Key_F4] = osgGA::GUIEventAdapter::KEY_F4;
	m_key_map[Qt::Key_F5] = osgGA::GUIEventAdapter::KEY_F5;
	m_key_map[Qt::Key_F6] = osgGA::GUIEventAdapter::KEY_F6;
	m_key_map[Qt::Key_F7] = osgGA::GUIEventAdapter::KEY_F7;
	m_key_map[Qt::Key_F8] = osgGA::GUIEventAdapter::KEY_F8;
	m_key_map[Qt::Key_F9] = osgGA::GUIEventAdapter::KEY_F9;
	m_key_map[Qt::Key_F10] = osgGA::GUIEventAdapter::KEY_F10;
	m_key_map[Qt::Key_F11] = osgGA::GUIEventAdapter::KEY_F11;
	m_key_map[Qt::Key_F12] = osgGA::GUIEventAdapter::KEY_F12;
	m_key_map[Qt::Key_F13] = osgGA::GUIEventAdapter::KEY_F13;
	m_key_map[Qt::Key_F14] = osgGA::GUIEventAdapter::KEY_F14;
	m_key_map[Qt::Key_F15] = osgGA::GUIEventAdapter::KEY_F15;
	m_key_map[Qt::Key_F16] = osgGA::GUIEventAdapter::KEY_F16;
	m_key_map[Qt::Key_F17] = osgGA::GUIEventAdapter::KEY_F17;
	m_key_map[Qt::Key_F18] = osgGA::GUIEventAdapter::KEY_F18;
	m_key_map[Qt::Key_F19] = osgGA::GUIEventAdapter::KEY_F19;
	m_key_map[Qt::Key_F20] = osgGA::GUIEventAdapter::KEY_F20;
	m_key_map[Qt::Key_hyphen] = '-';
	m_key_map[Qt::Key_Equal] = '=';
	m_key_map[Qt::Key_division] = osgGA::GUIEventAdapter::KEY_KP_Divide;
	m_key_map[Qt::Key_multiply] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
	m_key_map[Qt::Key_Minus] = '-';
	m_key_map[Qt::Key_Plus] = '+';
	m_key_map[Qt::Key_Insert] = osgGA::GUIEventAdapter::KEY_KP_Insert;
}

QtOSGWidget::~QtOSGWidget()
{
	if( m_graphics_window )
	{
		m_graphics_window->close();
		m_graphics_window = NULL;
	}
}

int QtOSGWidget::convertQKeyEnventToOSG( QKeyEvent* event )
{
	auto itr = m_key_map.find( event->key() );
	if( itr == m_key_map.end() )
	{
		return int( *(event->text().toLatin1().data()) );
	}
	else
	{
		return itr->second;
	}
}

void QtOSGWidget::setTouchEventsEnabled( bool e )
{
#ifdef USE_GESTURES
	if( e==m_touchEventsEnabled )
	{
		return;
	}

	m_touchEventsEnabled = e;

	if( m_touchEventsEnabled )
	{
		grabGesture( Qt::PinchGesture );
	}
	else
	{
		ungrabGesture( Qt::PinchGesture );
	}
#endif
}

void QtOSGWidget::processDeferredEvents()
{
	QQueue<QEvent::Type> deferredEventQueueCopy;
	{
		QMutexLocker lock( &m_deferredEventQueueMutex );
		deferredEventQueueCopy = m_deferredEventQueue;
		m_eventCompressor.clear();
		m_deferredEventQueue.clear();
	}

	while( !deferredEventQueueCopy.isEmpty() )
	{
		QEvent event( deferredEventQueueCopy.dequeue() );
		QOpenGLWidget::event( &event );
	}
}

void QtOSGWidget::initializeGL()
{
	// Set up the rendering context, load shaders and other resources, etc.:
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

}

void QtOSGWidget::resizeGL( int w, int h )
{
	double x = this->geometry().x() * m_device_pixel_ratio;
	double y = this->geometry().y() * m_device_pixel_ratio;
	m_graphics_window->getEventQueue()->windowResize( x, y, w * m_device_pixel_ratio, h * m_device_pixel_ratio);

	// graphics window resize
	double xx = this->x() * m_device_pixel_ratio;
	double yy = this->y() * m_device_pixel_ratio;
	m_graphics_window->resized( xx, yy, w, h );

	// camera viewport
	std::list<osg::Camera*>& cameras = m_graphics_window->getCameras();
	for( auto cam : cameras )
	{
		cam->setViewport( 0, 0, w * m_device_pixel_ratio, h * m_device_pixel_ratio);
	}

	QOpenGLWidget::resizeGL( w * m_device_pixel_ratio, h * m_device_pixel_ratio);
}

bool QtOSGWidget::event( QEvent* event )
{
#ifdef USE_GESTURES
	if( event->type()==QEvent::Gesture )
	{
		return gestureEvent( static_cast<QGestureEvent*>(event) );
	}
#endif

	// QEvent::Hide
	//
	// workaround "Qt-workaround" that does glFinish before hiding the widget
	// (the Qt workaround was seen at least in Qt 4.6.3 and 4.7.0)
	//
	// Qt makes the context current, performs glFinish, and releases the context.
	// This makes the problem in OSG multithreaded environment as the context
	// is active in another thread, thus it can not be made current for the purpose
	// of glFinish in this thread.

	// QEvent::ParentChange
	//
	// Reparenting QtOSGWidget may create a new underlying window and a new GL context.
	// Qt will then call doneCurrent on the GL context about to be deleted. The thread
	// where old GL context was current has no longer current context to render to and
	// we cannot make new GL context current in this thread.

	// We workaround above problems by deferring execution of problematic event requests.
	// These events has to be enqueue and executed later in a main GUI thread (GUI operations
	// outside the main thread are not allowed) just before makeCurrent is called from the
	// right thread. The good place for doing that is right after swap in a swapBuffersImplementation.

	if( event->type() == QEvent::Hide )
	{
		// enqueue only the last of QEvent::Hide and QEvent::Show
		enqueueDeferredEvent( QEvent::Hide, QEvent::Show );
		return true;
	}
	else if( event->type() == QEvent::Show )
	{
		// enqueue only the last of QEvent::Show or QEvent::Hide
		enqueueDeferredEvent( QEvent::Show, QEvent::Hide );
		return true;
	}
	else if( event->type() == QEvent::ParentChange )
	{
		// enqueue only the last QEvent::ParentChange
		enqueueDeferredEvent( QEvent::ParentChange );
		return true;
	}

	// perform regular event handling
	return QOpenGLWidget::event( event );
}

void QtOSGWidget::setKeyboardModifiers( QInputEvent* event )
{
	int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
	unsigned int mask = 0;
	if( modkey & Qt::ShiftModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
	if( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
	if( modkey & Qt::AltModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
	m_graphics_window->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

void QtOSGWidget::paintEvent( QPaintEvent* )
{
	m_graphics_window->requestRedraw();
}

void QtOSGWidget::moveEvent( QMoveEvent* event )
{
	const QPoint& pos = event->pos();
	int scaled_width = static_cast<int>(width()*m_device_pixel_ratio);
	int scaled_height = static_cast<int>(height()*m_device_pixel_ratio);
	m_graphics_window->resized( pos.x() * m_device_pixel_ratio, pos.y() * m_device_pixel_ratio, scaled_width, scaled_height );
	m_graphics_window->getEventQueue()->windowResize( pos.x() * m_device_pixel_ratio, pos.y() * m_device_pixel_ratio, scaled_width, scaled_height );
}

void QtOSGWidget::keyPressEvent( QKeyEvent* event )
{
	setKeyboardModifiers( event );
	int value = convertQKeyEnventToOSG( event );
	m_graphics_window->getEventQueue()->keyPress( value );
}

void QtOSGWidget::keyReleaseEvent( QKeyEvent* event )
{
	if( event->isAutoRepeat() )
	{
		event->ignore();
	}
	else
	{
		setKeyboardModifiers( event );
		int value = convertQKeyEnventToOSG( event );
		m_graphics_window->getEventQueue()->keyRelease( value );
	}
}

void QtOSGWidget::mousePressEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MiddleButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseButtonPress( event->x()*m_device_pixel_ratio, event->y()*m_device_pixel_ratio, button );
}

void QtOSGWidget::mouseReleaseEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MiddleButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseButtonRelease( event->x()*m_device_pixel_ratio, event->y()*m_device_pixel_ratio, button );
}



void QtOSGWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MiddleButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseDoubleButtonPress( event->x()*m_device_pixel_ratio, event->y()*m_device_pixel_ratio, button );
}

void QtOSGWidget::mouseMoveEvent( QMouseEvent* event )
{
	setKeyboardModifiers( event );
	double x = event->x() * m_device_pixel_ratio;
	double y = event->y() * m_device_pixel_ratio;
	osgGA::EventQueue* eventQueue = m_graphics_window->getEventQueue();
	eventQueue->mouseMotion( x, y );
}

void QtOSGWidget::wheelEvent( QWheelEvent* event )
{
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseScroll(
		(event->angleDelta().y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN));
}

#ifdef USE_GESTURES
static osgGA::GUIEventAdapter::TouchPhase translateQtGestureState( Qt::GestureState state )
{
	osgGA::GUIEventAdapter::TouchPhase touchPhase;
	switch( state )
	{
	case Qt::GestureStarted:
		touchPhase = osgGA::GUIEventAdapter::TOUCH_BEGAN;
		break;
	case Qt::GestureUpdated:
		touchPhase = osgGA::GUIEventAdapter::TOUCH_MOVED;
		break;
	case Qt::GestureFinished:
	case Qt::GestureCanceled:
		touchPhase = osgGA::GUIEventAdapter::TOUCH_ENDED;
		break;
	default:
		touchPhase = osgGA::GUIEventAdapter::TOUCH_UNKNOWN;
	};

	return touchPhase;
}
#endif


bool QtOSGWidget::gestureEvent( QGestureEvent* qevent )
{
#ifndef USE_GESTURES
	return false;
#else

	bool accept = false;

	if( QPinchGesture* pinch = static_cast<QPinchGesture *>(qevent->gesture( Qt::PinchGesture )) )
	{
		const QPointF qcenterf = pinch->centerPoint();
		const float angle = pinch->totalRotationAngle();
		const float scale = pinch->totalScaleFactor();

		const QPoint pinchCenterQt = mapFromGlobal( qcenterf.toPoint() );
		const osg::Vec2 pinchCenter( pinchCenterQt.x(), pinchCenterQt.y() );

		//We don't have absolute positions of the two touches, only a scale and rotation
		//Hence we create pseudo-coordinates which are reasonable, and centered around the
		//real position
		const float radius = float( width()+height() )/4.0f;
		const osg::Vec2 vector( scale*cos( angle )*radius, scale*sin( angle )*radius );
		const osg::Vec2 p0 = pinchCenter+vector;
		const osg::Vec2 p1 = pinchCenter-vector;

		osg::ref_ptr<osgGA::GUIEventAdapter> event = 0;
		const osgGA::GUIEventAdapter::TouchPhase touchPhase = translateQtGestureState( pinch->state() );
		if( touchPhase==osgGA::GUIEventAdapter::TOUCH_BEGAN )
		{
			event = m_graphics_window->getEventQueue()->touchBegan( 0, touchPhase, p0[0], p0[1] );
		}
		else if( touchPhase==osgGA::GUIEventAdapter::TOUCH_MOVED )
		{
			event = m_graphics_window->getEventQueue()->touchMoved( 0, touchPhase, p0[0], p0[1] );
		}
		else
		{
			event = m_graphics_window->getEventQueue()->touchEnded( 0, touchPhase, p0[0], p0[1], 1 );
		}

		if( event )
		{
			event->addTouchPoint( 1, touchPhase, p1[0], p1[1] );
			accept = true;
		}
	}

	if( accept )
		qevent->accept();

	return accept;
#endif
}



GraphicsWindowQt::GraphicsWindowQt( QWidget* parent, Qt::WindowFlags f )
{
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	osg::setNotifyLevel( osg::NotifySeverity::FATAL );

	_traits = new osg::GraphicsContext::Traits();
	_traits->windowDecoration = false;
	_traits->x = 0;
	_traits->y = 0;
	_traits->width = 100;
	_traits->height = 100;
	_traits->doubleBuffer = true;
	if( ds )
	{
		_traits->alpha = ds->getMinimumNumAlphaBits();
		_traits->stencil = ds->getMinimumNumStencilBits();
		_traits->sampleBuffers = ds->getMultiSamples();
		_traits->samples = ds->getNumMultiSamples();
	}
	if( parent )
	{
		_traits->width = parent->width();
		_traits->height = parent->height();
	}

	// create widget if it does not exist
	if( !m_opengl_widget )
	{
		// WindowFlags
		Qt::WindowFlags flags = f | Qt::Widget;
		// create widget
		m_opengl_widget = new QtOSGWidget( parent, flags );
	}

	m_opengl_widget->setMouseTracking( true );
	m_opengl_widget->setFocusPolicy( Qt::WheelFocus );
	m_opengl_widget->setGraphicsWindow( this );
	useCursor( _traits->useCursor );

	// initialize State
	setState( new osg::State );
	getState()->setGraphicsContext( this );

	// initialize contextID
	if( _traits.valid() && _traits->sharedContext.valid() )
	{
		getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
		incrementContextIDUsageCount( getState()->getContextID() );
	}
	else
	{
		getState()->setContextID( osg::GraphicsContext::createNewContextID() );
	}

	// make sure the event queue has the correct window rectangle size and input range
#if (OPENSCENEGRAPH_MAJOR_VERSION == 3) && (OPENSCENEGRAPH_MINOR_VERSION == 2)
	getEventQueue()->syncWindowRectangleWithGraphcisContext();
#else
	getEventQueue()->syncWindowRectangleWithGraphicsContext();
#endif
}

GraphicsWindowQt::~GraphicsWindowQt()
{
	close();

	// remove reference from QtOSGWidget
	if( m_opengl_widget )
	{
		m_opengl_widget->m_graphics_window = NULL;
	}
}

void GraphicsWindowQt::grabFocus()
{
	if( m_opengl_widget )
	{
		m_opengl_widget->setFocus( Qt::ActiveWindowFocusReason );
	}
}

void GraphicsWindowQt::grabFocusIfPointerInWindow()
{
	if( m_opengl_widget->underMouse() )
	{
		m_opengl_widget->setFocus( Qt::ActiveWindowFocusReason );
	}
}

void GraphicsWindowQt::useCursor( bool cursorOn )
{
	if( m_opengl_widget )
	{
		_traits->useCursor = cursorOn;
		if( !cursorOn ) m_opengl_widget->setCursor( Qt::BlankCursor );
		else m_opengl_widget->setCursor( m_current_cursor );
	}
}

void GraphicsWindowQt::setCursor( MouseCursor cursor )
{
	if( cursor==InheritCursor && m_opengl_widget )
	{
		m_opengl_widget->unsetCursor();
	}

	switch( cursor )
	{
	case NoCursor: m_current_cursor = Qt::BlankCursor; break;
	case RightArrowCursor: case LeftArrowCursor: m_current_cursor = Qt::ArrowCursor; break;
	case InfoCursor: m_current_cursor = Qt::SizeAllCursor; break;
	case DestroyCursor: m_current_cursor = Qt::ForbiddenCursor; break;
	case HelpCursor: m_current_cursor = Qt::WhatsThisCursor; break;
	case CycleCursor: m_current_cursor = Qt::ForbiddenCursor; break;
	case SprayCursor: m_current_cursor = Qt::SizeAllCursor; break;
	case WaitCursor: m_current_cursor = Qt::WaitCursor; break;
	case TextCursor: m_current_cursor = Qt::IBeamCursor; break;
	case CrosshairCursor: m_current_cursor = Qt::CrossCursor; break;
	case HandCursor: m_current_cursor = Qt::OpenHandCursor; break;
	case UpDownCursor: m_current_cursor = Qt::SizeVerCursor; break;
	case LeftRightCursor: m_current_cursor = Qt::SizeHorCursor; break;
	case TopSideCursor: case BottomSideCursor: m_current_cursor = Qt::UpArrowCursor; break;
	case LeftSideCursor: case RightSideCursor: m_current_cursor = Qt::SizeHorCursor; break;
	case TopLeftCorner: m_current_cursor = Qt::SizeBDiagCursor; break;
	case TopRightCorner: m_current_cursor = Qt::SizeFDiagCursor; break;
	case BottomRightCorner: m_current_cursor = Qt::SizeBDiagCursor; break;
	case BottomLeftCorner: m_current_cursor = Qt::SizeFDiagCursor; break;
	default: break;
	};
	if( m_opengl_widget )
	{
		m_opengl_widget->setCursor( m_current_cursor );
	}
}

bool GraphicsWindowQt::valid() const
{
	if( m_opengl_widget == nullptr )
	{
		return false;
	}
	return m_opengl_widget->isValid();
}

bool GraphicsWindowQt::realizeImplementation()
{
	const QOpenGLContext *saved_context = QOpenGLContext::currentContext();

	// initialize GL context for the widget
	if( !valid() )
	{
		m_opengl_widget->initializeGL();
	}

	// make current
	m_realized = true;
	bool result = makeCurrent();
	m_realized = false;

	// fail if we do not have current context
	if( !result )
	{
		if( saved_context )
		{
			QSurface* surf = saved_context->surface();
			const_cast<QOpenGLContext*>(saved_context)->makeCurrent( surf );
		}

		OSG_WARN << "Window realize: Can make context current." << std::endl;
		return false;
	}

	m_realized = true;

	// make sure the event queue has the correct window rectangle size and input range
#if (OPENSCENEGRAPH_MAJOR_VERSION == 3) && (OPENSCENEGRAPH_MINOR_VERSION == 2)
	getEventQueue()->syncWindowRectangleWithGraphcisContext();
#else
	getEventQueue()->syncWindowRectangleWithGraphicsContext();
#endif

	// make this window's context not current
	// note: this must be done as we will probably make the context current from another thread
	//       and it is not allowed to have one context current in two threads
	if( !releaseContext() )
	{
		OSG_WARN << "Window realize: Can not release context." << std::endl;
	}

	// restore previous context
	if( saved_context )
	{
		const_cast<QOpenGLContext*>(saved_context)->makeCurrent( saved_context->surface() );
	}

	return true;
}

bool GraphicsWindowQt::isRealizedImplementation() const
{
	return m_realized;
}

void GraphicsWindowQt::closeImplementation()
{
	if( m_opengl_widget )
	{
		m_opengl_widget->close();
	}
	m_realized = false;
}

void GraphicsWindowQt::runOperations()
{
	// While in graphics thread this is last chance to do something useful before
	// graphics thread will execute its operations.
	if( m_opengl_widget->getNumDeferredEvents() > 0 )
	{
		m_opengl_widget->processDeferredEvents();
	}

	if( QOpenGLContext::currentContext() != m_opengl_widget->context() )
	{
		m_opengl_widget->makeCurrent();
	}

	GraphicsWindow::runOperations();
}

bool GraphicsWindowQt::makeCurrentImplementation()
{
	if( m_opengl_widget->getNumDeferredEvents() > 0 )
	{
		m_opengl_widget->processDeferredEvents();
	}

	m_opengl_widget->makeCurrent();
	
	return true;
}

bool GraphicsWindowQt::releaseContextImplementation()
{
	m_opengl_widget->doneCurrent();
	return true;
}

void GraphicsWindowQt::requestWarpPointer( float x, float y )
{
	if( m_opengl_widget )
	{
		QCursor::setPos( m_opengl_widget->mapToGlobal( QPoint( (int)x, (int)y ) ) );
	}
}
