#pragma once
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

#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerBase>

#include <QMutex>
#include <QEvent>
#include <QQueue>
#include <QSet>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QInputEvent;
class QGestureEvent;

class QtOSGWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	QtOSGWidget( QWidget* parent, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
	QtOSGWidget( const QSurfaceFormat& format, QWidget* parent, Qt::WindowFlags f = 0, bool forwardKeyEvents = false );
	virtual ~QtOSGWidget();

	void setGraphicsWindow( osgViewer::GraphicsWindow* gw ) { m_graphics_window = gw; }
	osgViewer::View* getView() { return m_main_view; }
	osgViewer::CompositeViewer* getViewer() { return m_viewer; }
	bool getForwardKeyEvents() const { return m_forwardKeyEvents; }
	bool getTouchEventsEnabled() const { return m_touchEventsEnabled; }
	void setTouchEventsEnabled( bool e );
	void setKeyboardModifiers( QInputEvent* event );
	virtual void setForwardKeyEvents( bool f ) { m_forwardKeyEvents = f; }
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );
	virtual void mouseDoubleClickEvent( QMouseEvent* event );
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void wheelEvent( QWheelEvent* event );
	virtual bool gestureEvent( QGestureEvent* event );

protected:
	virtual void initializeGL();
	virtual void resizeGL( int w, int h );

	int getNumDeferredEvents()
	{
		QMutexLocker lock( &m_deferredEventQueueMutex );
		return m_deferredEventQueue.count();
	}
	void enqueueDeferredEvent( QEvent::Type eventType, QEvent::Type removeEventType = QEvent::None )
	{
		QMutexLocker lock( &m_deferredEventQueueMutex );

		if( removeEventType != QEvent::None )
		{
			if( m_deferredEventQueue.removeOne( removeEventType ) )
				m_eventCompressor.remove( eventType );
		}

		if( m_eventCompressor.find( eventType ) == m_eventCompressor.end() )
		{
			m_deferredEventQueue.enqueue( eventType );
			m_eventCompressor.insert( eventType );
		}
	}
	void processDeferredEvents();

	virtual void paintEvent( QPaintEvent *e );
	virtual void moveEvent( QMoveEvent* event );
	virtual bool event( QEvent* event );

	friend class GraphicsWindowQt;
	osgViewer::GraphicsWindow* m_graphics_window = nullptr;

	osg::ref_ptr<osgViewer::View> m_main_view;
	osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;

	QMutex m_deferredEventQueueMutex;
	QQueue<QEvent::Type> m_deferredEventQueue;
	QSet<QEvent::Type> m_eventCompressor;

	bool m_touchEventsEnabled = false;

	bool m_forwardKeyEvents;
	qreal m_devicePixelRatio = 1.0;
};

class GraphicsWindowQt : public osgViewer::GraphicsWindowEmbedded
{
public:
	GraphicsWindowQt( osg::GraphicsContext::Traits* traits, QWidget* parent, Qt::WindowFlags f = 0 );
	GraphicsWindowQt( QtOSGWidget* widget );
	virtual ~GraphicsWindowQt();

	QtOSGWidget* getOpenGLWidget() { return m_opengl_widget; }

	struct WindowData : public osg::Referenced
	{
		WindowData( QtOSGWidget* widget = NULL, QWidget* parent = NULL ) : _widget( widget ), _parent( parent ) {}
		QtOSGWidget* _widget;
		QWidget* _parent;
	};

	bool init( Qt::WindowFlags f, QWidget* parent );

	static QSurfaceFormat traits2qglFormat( const osg::GraphicsContext::Traits* traits );
	static void qglFormat2traits( const QSurfaceFormat& format, osg::GraphicsContext::Traits* traits );
	static osg::GraphicsContext::Traits* createTraits( const QOpenGLWidget* widget );

	virtual bool setWindowRectangleImplementation( int x, int y, int width, int height );
	virtual void getWindowRectangle( int& x, int& y, int& width, int& height );
	virtual bool setWindowDecorationImplementation( bool windowDecoration );
	virtual bool getWindowDecoration() const;
	virtual void grabFocus();
	virtual void grabFocusIfPointerInWindow();
	virtual void raiseWindow();
	virtual void setWindowName( const std::string& name );
	virtual std::string getWindowName();
	virtual void useCursor( bool cursorOn );
	virtual void setCursor( MouseCursor cursor );
	bool getTouchEventsEnabled() const { return m_opengl_widget->getTouchEventsEnabled(); }
	virtual void setTouchEventsEnabled( bool e ) { m_opengl_widget->setTouchEventsEnabled( e ); }
	virtual bool valid() const;
	virtual bool realizeImplementation();
	virtual bool isRealizedImplementation() const;
	virtual void closeImplementation();
	virtual bool makeCurrentImplementation();
	virtual bool releaseContextImplementation();
	virtual void swapBuffersImplementation();
	virtual void runOperations();
	virtual void requestWarpPointer( float x, float y );

protected:
	friend class QtOSGWidget;
	QtOSGWidget* m_opengl_widget = nullptr;
	bool m_owns_widget = false;
	QCursor m_current_cursor;
	bool m_realized = false;
};
