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
	QtOSGWidget( QWidget* parent, Qt::WindowFlags f );
	virtual ~QtOSGWidget();

	void setGraphicsWindow( osgViewer::GraphicsWindow* gw ) { m_graphics_window = gw; }
	osgViewer::View* getView() { return m_main_view; }
	osgViewer::CompositeViewer* getViewer() { return m_viewer; }
	bool getTouchEventsEnabled() const { return m_touchEventsEnabled; }
	void setTouchEventsEnabled( bool e );
	void setKeyboardModifiers( QInputEvent* event );
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
	int convertQKeyEnventToOSG( QKeyEvent* event );
	virtual void paintEvent( QPaintEvent *e );
	virtual void moveEvent( QMoveEvent* event );
	virtual bool event( QEvent* event );

	friend class GraphicsWindowQt;
	osgViewer::GraphicsWindow* m_graphics_window = nullptr;

	osg::ref_ptr<osgViewer::View> m_main_view;
	osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;

	std::map<unsigned int, int> m_key_map;
	QMutex m_deferredEventQueueMutex;
	QQueue<QEvent::Type> m_deferredEventQueue;
	QSet<QEvent::Type> m_eventCompressor;
	bool m_touchEventsEnabled = false;
	qreal m_device_pixel_ratio = 1.0;
};

class GraphicsWindowQt : public osgViewer::GraphicsWindowEmbedded
{
public:
	GraphicsWindowQt( QWidget* parent, Qt::WindowFlags f );
	virtual ~GraphicsWindowQt();

	QtOSGWidget* getOpenGLWidget() { return m_opengl_widget; }
	virtual void grabFocus();
	virtual void grabFocusIfPointerInWindow();
	virtual void useCursor( bool cursorOn );
	virtual void setCursor( MouseCursor cursor );
	virtual bool valid() const;
	virtual bool realizeImplementation();
	virtual bool isRealizedImplementation() const;
	virtual void closeImplementation();
	virtual bool makeCurrentImplementation();
	virtual bool releaseContextImplementation();
	virtual void runOperations();
	virtual void requestWarpPointer( float x, float y );

protected:
	friend class QtOSGWidget;
	QtOSGWidget* m_opengl_widget = nullptr;
	QCursor m_current_cursor;
	bool m_realized = false;
};
