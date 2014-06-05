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

#ifdef _DEBUG

#include <osg/PolygonMode>
#include <osg/Material>
#include <osgText/Text>

#include <QtCore/qglobal.h>

#include <QAction>
#include <QToolBar>
#include <QCheckBox>
#include <QtCore/QSettings>
#include <QtCore/QFile>

#include "ViewController.h"
#include "viewer/ViewerWidget.h"
#include "viewer/Orbit3DManipulator.h"
#include "ifcppgeometry/GeomUtils.h"
#include "ifcppgeometry/DebugViewerCallback.h"
#include "ifcppgeometry/ConverterOSG.h"
#include "ifcppgeometry/CSG_Adapter.h"

#include "DebugViewer.h"

DebugViewer::DebugViewer() : QMainWindow()
{
	setWindowTitle("IFC++ DebugViewer");
	setWindowIcon( QIcon( ":img/IfcPlusPlusViewerWindowIcon.png" ) );
	
	// global style sheet definitions
	QFile file( ":styles.css" );
	file.open( QFile::ReadOnly );
	QString styleSheet = QLatin1String( file.readAll() );
	setStyleSheet( styleSheet );

	// viewer
	m_view_controller = new ViewController();
	m_viewer_widget = new ViewerWidget();
	m_viewer_widget->setParent( this );
	Orbit3DManipulator* camera_manip = new Orbit3DManipulator( nullptr );
	m_viewer_widget->getMainView()->setCameraManipulator( camera_manip );
	m_viewer_widget->setRootNode( m_view_controller->getRootNode() );


	setRenderPolyhedronCallBack( this, &DebugViewer::renderPolyhedronWrapper );
	setRenderMeshsetCallBack( this, &DebugViewer::renderMeshsetWrapper );
	setRenderPolylineCallBack( this, &DebugViewer::renderPolylineWrapper );
	setRenderPathsCallBack( this, &DebugViewer::renderPathsWrapper );

	// gui
	QAction* zoom_bounds_btn = new QAction(QIcon(":img/zoomBoundings.png"), "&Zoom to boundings", this );
	zoom_bounds_btn->setShortcut(tr("Ctrl+Z"));
	zoom_bounds_btn->setStatusTip("Zoom to boundings");
	connect(zoom_bounds_btn, SIGNAL(triggered()), this, SLOT(slotBtnZoomBoundingsClicked()));

	QAction* wireframe = new QAction(QIcon(":img/TabViewWireframe.png"), "&Wireframe [w]", this );
	wireframe->setCheckable( true );
	//wireframe->setShortcut(tr("w"));
	wireframe->setStatusTip("Wireframe [w]");
	connect(wireframe, SIGNAL(triggered()), this, SLOT(slotBtnWireframeClicked()));

	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	
	m_cull_front = false;
	m_cull_back = false;
	if( keys.contains( "DebugViewerCullFrontFaces" ) )
	{
		m_cull_front = settings.value("DebugViewerCullFrontFaces").toBool();
	}
	if( keys.contains( "DebugViewerCullBackFaces" ) )
	{
		m_cull_back = settings.value("DebugViewerCullBackFaces").toBool();
	}
	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_view_controller->getRootNode()->getOrCreateStateSet() );

	// cull face buttons
	QCheckBox* cull_front_faces = new QCheckBox( "Cull front faces" );
	if( m_cull_front )
	{
		cull_front_faces->setChecked( true );
	}
	connect( cull_front_faces, SIGNAL( stateChanged( int ) ), this, SLOT( slotCullFrontFaces( int ) ) );

	QCheckBox* cull_back_faces = new QCheckBox( "Cull back faces" );
	if( m_cull_back )
	{
		cull_back_faces->setChecked( true );
	}
	connect( cull_back_faces, SIGNAL( stateChanged( int ) ), this, SLOT( slotCullBackFaces( int ) ) );

	QToolBar * m_file_toolbar = new QToolBar();
	m_file_toolbar->setObjectName("FileToolbar");
	m_file_toolbar->addAction(zoom_bounds_btn);
	m_file_toolbar->addAction(wireframe);
	m_file_toolbar->addWidget(cull_front_faces);
	m_file_toolbar->addWidget(cull_back_faces);
	addToolBar( Qt::LeftToolBarArea, m_file_toolbar );

	// central widget
	setCentralWidget( m_viewer_widget );

	// restore geometry
	if( keys.contains( "DebugViewerGeometry" ) )
	{
		restoreGeometry(settings.value("DebugViewerGeometry").toByteArray());
	}
	else
	{
		showMaximized();
	}
	if( keys.contains( "DebugViewerState" ) )
	{
		restoreState(settings.value("DebugViewerState").toByteArray());
	}
}

DebugViewer::~DebugViewer()
{

}

void DebugViewer::closeEvent( QCloseEvent *event )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("DebugViewerGeometry", saveGeometry());
	settings.setValue("DebugViewerState", saveState());
	QMainWindow::closeEvent( event );
}


void DebugViewer::slotBtnZoomBoundingsClicked()
{
	osg::BoundingSphere bs = m_view_controller->getModelNode()->computeBound();
	
	osgViewer::View* main_view = m_viewer_widget->getMainView();
	if( main_view )
	{
		osgGA::CameraManipulator* camera_manip = main_view->getCameraManipulator();
		Orbit3DManipulator* orbit_manip = dynamic_cast<Orbit3DManipulator*>( camera_manip );
		if( orbit_manip )
		{
			orbit_manip->zoomToBoundingSphere( bs );
		}
	}
}

void DebugViewer::slotBtnWireframeClicked()
{
	QAction* toggle_btn = (QAction*)sender();

	if( toggle_btn->isChecked() )
	{
		m_view_controller->setViewerMode( ViewController::VIEWER_MODE_WIREFRAME );
		//m_viewer_widget->setViewerMode( ViewerWidget::VIEWER_MODE_WIREFRAME );
	}
	else
	{
		m_view_controller->setViewerMode( ViewController::VIEWER_MODE_SHADED );
		//m_viewer_widget->setViewerMode( ViewerWidget::VIEWER_MODE_SHADED );
	}
}

void DebugViewer::renderPolyhedronWrapper( void* ptr, const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe )
{
	DebugViewer* myself = (DebugViewer*)ptr;
	if( myself )
	{
		myself->renderPolyhedron( poly, color, wireframe );
	}
}

void DebugViewer::renderMeshsetWrapper( void* ptr, const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe )
{
	DebugViewer* myself = (DebugViewer*)ptr;
	if( myself )
	{
		myself->renderMeshset( meshset, color, wireframe );
	}
}

void DebugViewer::renderPolylineWrapper(void* ptr, const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color)
{
	DebugViewer* myself = (DebugViewer*)ptr;
	if( myself )
	{
		myself->renderPolyline( poly_line, color );
	}
}

void DebugViewer::renderPathsWrapper(void* ptr, const std::vector<std::vector<carve::geom::vector<2> > >& paths )
{
	DebugViewer* myself = (DebugViewer*)ptr;
	if( myself )
	{
		myself->renderPaths( paths );
	}
}

void DebugViewer::renderPolyhedron( const carve::input::PolyhedronData* poly, const osg::Vec4f& color, const bool wireframe )
{
	if( !poly )
	{
		return;
	}
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	if( wireframe )
	{
		osg::ref_ptr<osg::PolygonMode> polygon_mode = new osg::PolygonMode();
		polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
		geode->getOrCreateStateSet()->setAttribute( polygon_mode );
	}
	osg::Material* material = new osg::Material();//(osg::Material *) geode->getStateSet()->getAttribute(osg::StateAttribute::MATERIAL); 
	material->setColorMode(osg::Material::EMISSION); 
	material->setEmission(osg::Material::FRONT_AND_BACK, color ); 
	geode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE); 

	carve::mesh::MeshSet<3> * meshset = poly->createMesh( carve::input::opts() );
	ConverterOSG::drawMeshSet( meshset, geode );
	m_view_controller->getModelNode()->addChild(geode);

	osg::ref_ptr<osg::Geode> geode_vertex_numbers = new osg::Geode();
	m_view_controller->getModelNode()->addChild(geode_vertex_numbers);
	drawVertexNumbers( poly, color, geode_vertex_numbers );

	m_viewer_widget->getViewer().frame();
}

void DebugViewer::renderMeshset( const carve::mesh::MeshSet<3>* meshset, const osg::Vec4f& color, const bool wireframe )
{
	if( !meshset )
	{
		return;
	}
	if( meshset->meshes.size() < 1 )
	{
		return;
	}

	//std::stringstream strs_err;
	//CSG_Adapter::checkMeshSetValidAndClosed( meshset, strs_err, -1 );
	
	const int nmeshes = meshset->meshes.size();
	for( int i=0; i<nmeshes; ++i )
	{
		osg::Vec4f color_mesh( color.r()*float(i)/float(nmeshes), color.g()*float(i)/float(nmeshes), color.b()*float(i)/float(nmeshes), color.a() );

		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		if( wireframe )
		{
			osg::ref_ptr<osg::PolygonMode> polygon_mode = new osg::PolygonMode();
			polygon_mode->setMode(  osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
			geode->getOrCreateStateSet()->setAttribute( polygon_mode );
		}

		osg::Material* material = new osg::Material();//(osg::Material *) geode->getStateSet()->getAttribute(osg::StateAttribute::MATERIAL); 
		material->setColorMode(osg::Material::EMISSION); 
		material->setEmission(osg::Material::FRONT_AND_BACK, color ); 
		geode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE); 

		ConverterOSG::drawMesh( meshset->meshes[i], geode );

		m_view_controller->getModelNode()->addChild(geode);
	}

	osg::ref_ptr<osg::Geode> geode_vertex_numbers = new osg::Geode();
	m_view_controller->getModelNode()->addChild(geode_vertex_numbers);

	shared_ptr<carve::poly::Polyhedron> poly_from_mesh( carve::polyhedronFromMesh(meshset, -1) );
	if( poly_from_mesh )
	{
		shared_ptr<carve::input::PolyhedronData> poly_data( new carve::input::PolyhedronData() );
		for( int i=0; i<poly_from_mesh->vertices.size(); ++i )
		{
			poly_data->addVertex( poly_from_mesh->vertices[i].v );
		}
		for( int i=0; i<poly_from_mesh->faces.size(); ++i )
		{
			carve::poly::Face<3>& f = poly_from_mesh->faces[i];
			if( f.nVertices() == 3 )
			{
				poly_data->addFace( poly_from_mesh->vertexToIndex( f.vertex(0) ), poly_from_mesh->vertexToIndex( f.vertex(1) ), poly_from_mesh->vertexToIndex( f.vertex(2) ) );
			}
			else if( f.nVertices() == 4 )
			{
				poly_data->addFace( poly_from_mesh->vertexToIndex( f.vertex(0) ), poly_from_mesh->vertexToIndex( f.vertex(1) ), poly_from_mesh->vertexToIndex( f.vertex(2) ), poly_from_mesh->vertexToIndex( f.vertex(3) ) );
			}
		}

		drawVertexNumbers( poly_data.get(), color, geode_vertex_numbers );
	}

	m_viewer_widget->getViewer().frame();
}

void DebugViewer::renderPolyline( const carve::input::PolylineSetData* poly_line, const osg::Vec4f& color )
{
	if( m_view_controller->getRootNode() )
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
			
		osg::Material* material = new osg::Material();//(osg::Material *) geode->getStateSet()->getAttribute(osg::StateAttribute::MATERIAL); 
		material->setColorMode(osg::Material::EMISSION); 
		material->setEmission(osg::Material::FRONT_AND_BACK, color ); 
		geode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE); 

		ConverterOSG::drawPolyline( poly_line, geode );
		m_view_controller->getRootNode()->addChild(geode);
		m_viewer_widget->getViewer().frame();
	}
}

double render_path_x_pos = 0.1;
double render_path_y_pos = 0.1;
void DebugViewer::renderPaths( const std::vector<std::vector<carve::geom::vector<2> > >& paths )
{
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	double x_min = DBL_MAX;
	double x_max = -DBL_MAX;
	double y_min = DBL_MAX;
	double y_max = -DBL_MAX;
	std::vector<carve::geom::vector<2> > vec_merged;
	for( std::vector<std::vector<carve::geom::vector<2> > >::const_iterator it = paths.begin(); it!=paths.end(); ++it )
	{
		const std::vector<carve::geom::vector<2> >& vec_path = (*it);
		
		for( std::vector<carve::geom::vector<2> >::const_iterator it2 = vec_path.begin(); it2!=vec_path.end(); ++it2 )
		{
			carve::geom::vector<2> point = (*it2);
			vec_merged.push_back( point );
			
			x_min = std::min( x_min, point.x );
			x_max = std::max( x_max, point.x );
			y_min = std::min( y_min, point.y );
			y_max = std::max( y_max, point.y );
		}
	}

	double max_extend = std::max( x_max-x_min, y_max-y_min );
	double min_extend = std::min( x_max-x_min, y_max-y_min );

	double scale = 1.0;
	if( min_extend < 1 )
	{
		scale = 1.0/min_extend;
	}

	//double font_size = 0.5*(max_extend + min_extend)*0.1;
	double font_size = min_extend*0.05*scale;
	for( int i=0; i<vec_merged.size(); ++i )
	{
		carve::geom::vector<2>& point = vec_merged[i];
		vertices->push_back( osg::Vec3f( point.x*scale + render_path_x_pos, point.y*scale + render_path_y_pos, 0 ) );

		osgText::Text* txt = new osgText::Text;
		txt->setFont("fonts/arial.ttf");
		txt->setColor( osg::Vec4f( 0.1, 0.1, 0.1, 1 ) );
		txt->setCharacterSize( font_size );
		txt->setAutoRotateToScreen( true );
		txt->setPosition( osg::Vec3( point.x*scale + render_path_x_pos, point.y*scale + render_path_y_pos, 0 ) );

		std::stringstream strs;
		strs << i;
		txt->setText( strs.str().c_str() );
		geode->addDrawable(txt);

		
	}
	render_path_x_pos += (x_max-x_min)*scale;

	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( vertices );
	geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size()) );
	geometry->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	
	geode->addDrawable( geometry );
	m_view_controller->getRootNode()->addChild(geode);
}

void DebugViewer::slotCullFrontFaces( int state )
{
	if( state == Qt::Checked )
	{
		m_cull_front = true;
	}
	else
	{
		m_cull_front = false;
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("DebugViewerCullFrontFaces", m_cull_front );

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_view_controller->getRootNode()->getOrCreateStateSet() );
}

void DebugViewer::slotCullBackFaces( int state )
{
	if( state == Qt::Checked )
	{
		m_cull_back = true;
	}
	else
	{
		m_cull_back = false;
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("DebugViewerCullBackFaces", m_cull_back );

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_view_controller->getRootNode()->getOrCreateStateSet() );
}

#endif
