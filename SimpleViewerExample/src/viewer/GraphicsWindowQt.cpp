/* -*-c++-*- OpenSceneGraph - Copyright (C) 2009 Wang Rui
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

#include <osg/DeleteHandler>
#include <osgViewer/ViewerBase>
#include <QInputEvent>
#include <QPointer>
#include <QOpenGLContext>

#if (QT_VERSION>=QT_VERSION_CHECK(4, 6, 0))
# define USE_GESTURES
# include <QGestureEvent>
# include <QGesture>
#endif

#include "GraphicsWindowQt.h"

class QtKeyboardMap
{

public:
	QtKeyboardMap()
	{
		mKeyMap[Qt::Key_Escape] = osgGA::GUIEventAdapter::KEY_Escape;
		mKeyMap[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
		mKeyMap[Qt::Key_Home] = osgGA::GUIEventAdapter::KEY_Home;
		mKeyMap[Qt::Key_Enter] = osgGA::GUIEventAdapter::KEY_KP_Enter;
		mKeyMap[Qt::Key_End] = osgGA::GUIEventAdapter::KEY_End;
		mKeyMap[Qt::Key_Return] = osgGA::GUIEventAdapter::KEY_Return;
		mKeyMap[Qt::Key_PageUp] = osgGA::GUIEventAdapter::KEY_Page_Up;
		mKeyMap[Qt::Key_PageDown] = osgGA::GUIEventAdapter::KEY_Page_Down;
		mKeyMap[Qt::Key_Left] = osgGA::GUIEventAdapter::KEY_Left;
		mKeyMap[Qt::Key_Right] = osgGA::GUIEventAdapter::KEY_Right;
		mKeyMap[Qt::Key_Up] = osgGA::GUIEventAdapter::KEY_Up;
		mKeyMap[Qt::Key_Down] = osgGA::GUIEventAdapter::KEY_Down;
		mKeyMap[Qt::Key_Backspace] = osgGA::GUIEventAdapter::KEY_BackSpace;
		mKeyMap[Qt::Key_Tab] = osgGA::GUIEventAdapter::KEY_Tab;
		mKeyMap[Qt::Key_Space] = osgGA::GUIEventAdapter::KEY_Space;
		mKeyMap[Qt::Key_Delete] = osgGA::GUIEventAdapter::KEY_Delete;
		mKeyMap[Qt::Key_Alt] = osgGA::GUIEventAdapter::KEY_Alt_L;
		mKeyMap[Qt::Key_Shift] = osgGA::GUIEventAdapter::KEY_Shift_L;
		mKeyMap[Qt::Key_Control] = osgGA::GUIEventAdapter::KEY_Control_L;
		mKeyMap[Qt::Key_Meta] = osgGA::GUIEventAdapter::KEY_Meta_L;

		mKeyMap[Qt::Key_F1] = osgGA::GUIEventAdapter::KEY_F1;
		mKeyMap[Qt::Key_F2] = osgGA::GUIEventAdapter::KEY_F2;
		mKeyMap[Qt::Key_F3] = osgGA::GUIEventAdapter::KEY_F3;
		mKeyMap[Qt::Key_F4] = osgGA::GUIEventAdapter::KEY_F4;
		mKeyMap[Qt::Key_F5] = osgGA::GUIEventAdapter::KEY_F5;
		mKeyMap[Qt::Key_F6] = osgGA::GUIEventAdapter::KEY_F6;
		mKeyMap[Qt::Key_F7] = osgGA::GUIEventAdapter::KEY_F7;
		mKeyMap[Qt::Key_F8] = osgGA::GUIEventAdapter::KEY_F8;
		mKeyMap[Qt::Key_F9] = osgGA::GUIEventAdapter::KEY_F9;
		mKeyMap[Qt::Key_F10] = osgGA::GUIEventAdapter::KEY_F10;
		mKeyMap[Qt::Key_F11] = osgGA::GUIEventAdapter::KEY_F11;
		mKeyMap[Qt::Key_F12] = osgGA::GUIEventAdapter::KEY_F12;
		mKeyMap[Qt::Key_F13] = osgGA::GUIEventAdapter::KEY_F13;
		mKeyMap[Qt::Key_F14] = osgGA::GUIEventAdapter::KEY_F14;
		mKeyMap[Qt::Key_F15] = osgGA::GUIEventAdapter::KEY_F15;
		mKeyMap[Qt::Key_F16] = osgGA::GUIEventAdapter::KEY_F16;
		mKeyMap[Qt::Key_F17] = osgGA::GUIEventAdapter::KEY_F17;
		mKeyMap[Qt::Key_F18] = osgGA::GUIEventAdapter::KEY_F18;
		mKeyMap[Qt::Key_F19] = osgGA::GUIEventAdapter::KEY_F19;
		mKeyMap[Qt::Key_F20] = osgGA::GUIEventAdapter::KEY_F20;

		mKeyMap[Qt::Key_hyphen] = '-';
		mKeyMap[Qt::Key_Equal] = '=';

		mKeyMap[Qt::Key_division] = osgGA::GUIEventAdapter::KEY_KP_Divide;
		mKeyMap[Qt::Key_multiply] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
		mKeyMap[Qt::Key_Minus] = '-';
		mKeyMap[Qt::Key_Plus] = '+';
		//mKeyMap[Qt::Key_H              ] = osgGA::GUIEventAdapter::KEY_KP_Home;
		//mKeyMap[Qt::Key_              ] = osgGA::GUIEventAdapter::KEY_KP_Up;
		//mKeyMap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
		//mKeyMap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
		//mKeyMap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
		//mKeyMap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
		//mKeyMap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
		//mKeyMap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
		//mKeyMap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
		mKeyMap[Qt::Key_Insert] = osgGA::GUIEventAdapter::KEY_KP_Insert;
		//mKeyMap[Qt::Key_Delete        ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
	}

	~QtKeyboardMap()
	{
	}

	int remapKey( QKeyEvent* event )
	{
		KeyMap::iterator itr = mKeyMap.find( event->key() );
		if( itr == mKeyMap.end() )
		{
			return int( *(event->text().toLatin1().data()) );
		}
		else
			return itr->second;
	}

private:
	typedef std::map<unsigned int, int> KeyMap;
	KeyMap mKeyMap;
};

static QtKeyboardMap s_QtKeyboardMap;

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
#define GETDEVICEPIXELRATIO() 1.0
#else
#define GETDEVICEPIXELRATIO() devicePixelRatio()
#endif

QtOSGWidget::QtOSGWidget( QWidget* parent, Qt::WindowFlags f, bool forwardKeyEvents )
	: QOpenGLWidget( parent, f ), m_forwardKeyEvents( forwardKeyEvents )
{
	m_devicePixelRatio = GETDEVICEPIXELRATIO();
}

QtOSGWidget::QtOSGWidget( const QSurfaceFormat& format, QWidget* parent, Qt::WindowFlags f, bool forwardKeyEvents )
	: QOpenGLWidget( parent, f ),m_forwardKeyEvents( forwardKeyEvents )
{
	Q_UNUSED(format)
	m_devicePixelRatio = GETDEVICEPIXELRATIO();

	QSurfaceFormat format2;
	format2.setDepthBufferSize( 24 );
	format2.setStencilBufferSize( 8 );
	format2.setVersion( 3, 2 );
	format2.setProfile( QSurfaceFormat::CoreProfile );
	//setFormat( format2 ); // must be called before the widget or its parent window gets shown

	m_main_view = new osgViewer::View();
	m_viewer = new osgViewer::CompositeViewer();
}

QtOSGWidget::~QtOSGWidget()
{
	// close GraphicsWindowQt
	if( m_graphics_window )
	{
		m_graphics_window->close();
		m_graphics_window = NULL;
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
	double x = this->geometry().x();
	double y = this->geometry().y();
	m_graphics_window->getEventQueue()->windowResize( x, y, w, h );

	// graphics window resize
	double xx = this->x();
	double yy = this->y();
	m_graphics_window->resized( xx, yy, w, h );

	// camera viewport
	std::list<osg::Camera*>& cameras = m_graphics_window->getCameras();
	for( auto cam : cameras )
	{
		cam->setViewport( 0, 0, w, h );
	}

	QOpenGLWidget::resizeGL( w, h );
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
	int scaled_width = static_cast<int>(width()*m_devicePixelRatio);
	int scaled_height = static_cast<int>(height()*m_devicePixelRatio);
	m_graphics_window->resized( pos.x(), pos.y(), scaled_width, scaled_height );
	m_graphics_window->getEventQueue()->windowResize( pos.x(), pos.y(), scaled_width, scaled_height );
}

void QtOSGWidget::keyPressEvent( QKeyEvent* event )
{
	setKeyboardModifiers( event );
	int value = s_QtKeyboardMap.remapKey( event );
	m_graphics_window->getEventQueue()->keyPress( value );

	// this passes the event to the regular Qt key event processing,
	// among others, it closes popup windows on ESC and forwards the event to the parent widgets
	if( m_forwardKeyEvents )
	{
		QOpenGLWidget::keyPressEvent( event );
	}
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
		int value = s_QtKeyboardMap.remapKey( event );
		m_graphics_window->getEventQueue()->keyRelease( value );
	}

	// this passes the event to the regular Qt key event processing,
	// among others, it closes popup windows on ESC and forwards the event to the parent widgets
	if( m_forwardKeyEvents )
	{
		QOpenGLWidget::keyReleaseEvent( event );
	}
}

void QtOSGWidget::mousePressEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseButtonPress( event->x()*m_devicePixelRatio, event->y()*m_devicePixelRatio, button );
}

void QtOSGWidget::mouseReleaseEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseButtonRelease( event->x()*m_devicePixelRatio, event->y()*m_devicePixelRatio, button );
}

void QtOSGWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
	int button = 0;
	switch( event->button() )
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseDoubleButtonPress( event->x()*m_devicePixelRatio, event->y()*m_devicePixelRatio, button );
}

void QtOSGWidget::mouseMoveEvent( QMouseEvent* event )
{
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseMotion( event->x()*m_devicePixelRatio, event->y()*m_devicePixelRatio );
}

void QtOSGWidget::wheelEvent( QWheelEvent* event )
{
	setKeyboardModifiers( event );
	m_graphics_window->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT) );
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



GraphicsWindowQt::GraphicsWindowQt( osg::GraphicsContext::Traits* traits, QWidget* parent, Qt::WindowFlags f )
{
	_traits = traits;
	init( f, parent );
}

GraphicsWindowQt::GraphicsWindowQt( QtOSGWidget* widget )
{
	m_opengl_widget = widget;
	_traits = m_opengl_widget ? createTraits( m_opengl_widget ) : new osg::GraphicsContext::Traits;
	init( 0, nullptr );
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

bool GraphicsWindowQt::init( Qt::WindowFlags f, QWidget* parent )
{
	// update _widget and parent by WindowData
	WindowData* windowData = _traits.get() ? dynamic_cast<WindowData*>(_traits->inheritedWindowData.get()) : 0;
	if( !m_opengl_widget )
	{
		m_opengl_widget = windowData ? windowData->_widget : NULL;
	}
	if( !parent )
	{
		parent = windowData ? windowData->_parent : NULL;
	}

	// create widget if it does not exist
	m_owns_widget = m_opengl_widget == NULL;
	if( !m_opengl_widget )
	{
		// WindowFlags
		Qt::WindowFlags flags = f | Qt::Widget;
		if( _traits->windowDecoration )
		{
			flags |= Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint
#if (QT_VERSION_CHECK(4, 5, 0) <= QT_VERSION)
				| Qt::WindowCloseButtonHint
#endif
				;
		}

		// create widget
		QSurfaceFormat format = traits2qglFormat( _traits.get() );
		m_opengl_widget = new QtOSGWidget( format, parent, flags );
	}

	// set widget name and position
	// (do not set it when we inherited the widget)
	m_owns_widget = false;
	if( m_owns_widget )
	{
		m_opengl_widget->setWindowTitle( _traits->windowName.c_str() );
		m_opengl_widget->move( _traits->x, _traits->y );
		if( !_traits->supportsResize ) m_opengl_widget->setFixedSize( _traits->width, _traits->height );
		else m_opengl_widget->resize( _traits->width, _traits->height );
	}

	// initialize widget properties
	//m_opengl_widget->setAutoBufferSwap( false );
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
	getEventQueue()->syncWindowRectangleWithGraphicsContext();

	return true;
}

QSurfaceFormat GraphicsWindowQt::traits2qglFormat( const osg::GraphicsContext::Traits* traits )
{
	QSurfaceFormat format( QSurfaceFormat::defaultFormat() );

	format.setAlphaBufferSize( traits->alpha );
	format.setRedBufferSize( traits->red );
	format.setGreenBufferSize( traits->green );
	format.setBlueBufferSize( traits->blue );
	format.setDepthBufferSize( traits->depth );
	format.setStencilBufferSize( traits->stencil );
	//format.setSampleBuffers( traits->sampleBuffers );
	format.setSamples( traits->samples );

	//format.setAlpha( traits->alpha>0 );
	//format.setDepth( traits->depth>0 );
	//format.setStencil( traits->stencil>0 );
	//format.setDoubleBuffer( traits->doubleBuffer );
	format.setSwapInterval( traits->vsync ? 1 : 0 );
	format.setStereo( traits->quadBufferStereo ? 1 : 0 );

	return format;
}

void GraphicsWindowQt::qglFormat2traits( const QSurfaceFormat& format, osg::GraphicsContext::Traits* traits )
{
	traits->red = format.redBufferSize();
	traits->green = format.greenBufferSize();
	traits->blue = format.blueBufferSize();
	//traits->alpha = format.alpha() ? format.alphaBufferSize() : 0;
	//traits->depth = format.depth() ? format.depthBufferSize() : 0;
	//traits->stencil = format.stencil() ? format.stencilBufferSize() : 0;

	//traits->sampleBuffers = format.sampleBuffers() ? 1 : 0;
	traits->samples = format.samples();

	traits->quadBufferStereo = format.stereo();
	//traits->doubleBuffer = format.doubleBuffer();

	traits->vsync = format.swapInterval() >= 1;
}

osg::GraphicsContext::Traits* GraphicsWindowQt::createTraits( const QOpenGLWidget* widget )
{
	osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits;

	qglFormat2traits( widget->format(), traits );

	QRect r = widget->geometry();
	traits->x = r.x();
	traits->y = r.y();
	traits->width = r.width();
	traits->height = r.height();
	traits->windowName = widget->windowTitle().toLocal8Bit().data();
	Qt::WindowFlags f = widget->windowFlags();
	traits->windowDecoration = (f & Qt::WindowTitleHint) && (f & Qt::WindowMinMaxButtonsHint) && (f & Qt::WindowSystemMenuHint);
	QSizePolicy sp = widget->sizePolicy();
	traits->supportsResize = sp.horizontalPolicy() != QSizePolicy::Fixed || sp.verticalPolicy() != QSizePolicy::Fixed;

	return traits;
}

bool GraphicsWindowQt::setWindowRectangleImplementation( int x, int y, int width, int height )
{
	if( m_opengl_widget == NULL )
	{
		return false;
	}

	m_opengl_widget->setGeometry( x, y, width, height );
	return true;
}

void GraphicsWindowQt::getWindowRectangle( int& x, int& y, int& width, int& height )
{
	if( m_opengl_widget )
	{
		const QRect& geom = m_opengl_widget->geometry();
		x = geom.x();
		y = geom.y();
		width = geom.width();
		height = geom.height();
	}
}

bool GraphicsWindowQt::setWindowDecorationImplementation( bool windowDecoration )
{
	Qt::WindowFlags flags = Qt::Widget; //::Window|Qt::CustomizeWindowHint;//|Qt::WindowStaysOnTopHint;
	if( windowDecoration )
	{
		flags |= Qt::WindowTitleHint|Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint;
	}
	_traits->windowDecoration = windowDecoration;

	if( m_opengl_widget )
	{
		m_opengl_widget->setWindowFlags( flags );

		return true;
	}

	return false;
}

bool GraphicsWindowQt::getWindowDecoration() const
{
	return _traits->windowDecoration;
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

void GraphicsWindowQt::raiseWindow()
{
	if( m_opengl_widget )
	{
		m_opengl_widget->raise();
	}
}

void GraphicsWindowQt::setWindowName( const std::string& name )
{
	if( m_opengl_widget )
	{
		m_opengl_widget->setWindowTitle( name.c_str() );
	}
}

std::string GraphicsWindowQt::getWindowName()
{
	return m_opengl_widget ? m_opengl_widget->windowTitle().toStdString() : "";
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
	if( m_opengl_widget ) m_opengl_widget->setCursor( m_current_cursor );
}

bool GraphicsWindowQt::valid() const
{
	return m_opengl_widget && m_opengl_widget->isValid();
}

bool GraphicsWindowQt::realizeImplementation()
{
	// save the current context
	// note: this will save only Qt-based contexts
	const QOpenGLContext *savedContext = QOpenGLContext::currentContext();

	// initialize GL context for the widget
	if( !valid() )
	{
		//_widget->glInit();
		m_opengl_widget->initializeGL();
	}

	// make current
	m_realized = true;
	bool result = makeCurrent();
	m_realized = false;

	// fail if we do not have current context
	if( !result )
	{
		if( savedContext )
		{
			QSurface* surf = savedContext->surface();
			const_cast<QOpenGLContext*>(savedContext)->makeCurrent( surf );
		}

		OSG_WARN << "Window realize: Can make context current." << std::endl;
		return false;
	}

	m_realized = true;

	// make sure the event queue has the correct window rectangle size and input range
	getEventQueue()->syncWindowRectangleWithGraphicsContext();

	// make this window's context not current
	// note: this must be done as we will probably make the context current from another thread
	//       and it is not allowed to have one context current in two threads
	if( !releaseContext() )
	{
		OSG_WARN << "Window realize: Can not release context." << std::endl;
	}

	// restore previous context
	if( savedContext )
	{
		const_cast<QOpenGLContext*>(savedContext)->makeCurrent( savedContext->surface() );
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

void GraphicsWindowQt::swapBuffersImplementation()
{
	return;

	//_widget->swapBuffers();

	// FIXME: the processDeferredEvents should really be executed in a GUI (main) thread context but
	// I couln't find any reliable way to do this. For now, lets hope non of *GUI thread only operations* will
	// be executed in a QGLWidget::event handler. On the other hand, calling GUI only operations in the
	// QGLWidget event handler is an indication of a Qt bug.
	if( m_opengl_widget->getNumDeferredEvents() > 0 )
	{
		m_opengl_widget->processDeferredEvents();
	}

	// We need to call makeCurrent here to restore our previously current context
	// which may be changed by the processDeferredEvents function.

	const QOpenGLContext *current_context = QOpenGLContext::currentContext();

	if( current_context != m_opengl_widget->context() )
	{
		m_opengl_widget->makeCurrent();
	}
	//if (QGLContext::currentContext() != _widget->context())
	//  _widget->makeCurrent();
}

void GraphicsWindowQt::requestWarpPointer( float x, float y )
{
	if( m_opengl_widget )
	{
		QCursor::setPos( m_opengl_widget->mapToGlobal( QPoint( (int)x, (int)y ) ) );
	}
}
