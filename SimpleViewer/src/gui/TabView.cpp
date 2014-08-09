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

#include <qglobal.h>
#include <QSettings>
#include <QToolButton>
#include <QBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QSpinBox>

#include <osgGA/OrbitManipulator>

#include <ifcpp/model/IfcPPModel.h>
#include <ifcppgeometry/GeomUtils.h>
#include <ifcppgeometry/GeometrySettings.h>
#include <ifcppgeometry/ReaderWriterIFC.h>

#include "IfcPlusPlusSystem.h"
#include "ViewController.h"
#include "viewer/ViewerWidget.h"
#include "viewer/Orbit3DManipulator.h"

#include "MainWindow.h"
#include "TabView.h"

TabView::TabView( IfcPlusPlusSystem* sys, ViewerWidget* vw ) : m_system(sys), m_vw(vw)
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	
	m_cull_front = false;
	m_cull_back = true; // default to back face culling
	if( keys.contains( "cullFrontFaces" ) )
	{
		m_cull_front = settings.value("cullFrontFaces").toBool();
	}
	if( keys.contains( "cullBackFaces" ) )
	{
		m_cull_back = settings.value("cullBackFaces").toBool();
	}
	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->m_rootnode->getOrCreateStateSet() );

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


	// light button
	QToolButton* btn_toggle_light = new QToolButton();
	btn_toggle_light->setIcon( QIcon( ":img/bulb.png" ) );
	btn_toggle_light->setIconSize( QSize( 22, 22 ) );
	btn_toggle_light->setToolTip( "Light on/off" );
	btn_toggle_light->setCheckable( true );
	btn_toggle_light->setChecked( true );
	connect( btn_toggle_light,	SIGNAL( clicked() ),	this,	SLOT( slotToggleSceneLight() ) );

	// number of vertices per cycle
	m_spinbox_circle_vertices = new QSpinBox( this );
	m_spinbox_circle_vertices->setRange(6, 48);
	m_spinbox_circle_vertices->setSingleStep(1);
	m_spinbox_circle_vertices->setValue(10);
	if( keys.contains( "NumVerticesPerCircle" ) )
	{
		int num_vertices = settings.value("NumVerticesPerCircle").toInt();
		m_spinbox_circle_vertices->setValue(num_vertices);
		m_system->getReaderWriterIFC()->getGeomSettings()->m_num_vertices_per_circle = num_vertices;
	}
	else
	{
		int num_vertices = m_system->getReaderWriterIFC()->getGeomSettings()->m_num_vertices_per_circle;
		m_spinbox_circle_vertices->setValue(num_vertices);
	}

	QHBoxLayout* num_vertices_hbox = new QHBoxLayout();
	num_vertices_hbox->addWidget( m_spinbox_circle_vertices );
	num_vertices_hbox->addWidget( new QLabel( tr("Number of vertices per circle") ) );
	connect(m_spinbox_circle_vertices, SIGNAL(valueChanged(int)), this, SLOT( slotSetNumVertices( int ) ) );

	m_check_show_curve_representations = new QCheckBox(tr("Show curve representations"));
	bool show_curves = true;
	if( keys.contains( "ShowCurveRepresentations" ) )
	{
		show_curves = settings.value( "ShowCurveRepresentations" ).toBool();
		if( show_curves )
		{
			m_check_show_curve_representations->setChecked( true );
		}
	}
	m_system->getViewController()->m_show_curve_representation = show_curves;
	connect( m_check_show_curve_representations, SIGNAL( stateChanged(int) ), this, SLOT( slotShowCurves(int) ) );

	// layout
	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget( btn_toggle_light, 0 );
	hbox->addWidget( cull_front_faces, 0 );
	hbox->addWidget( cull_back_faces, 0 );
	hbox->addLayout( num_vertices_hbox );
	hbox->addWidget( m_check_show_curve_representations );
	hbox->addStretch( 1 );

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addLayout( hbox );
	vbox->addStretch(1);

	setLayout( vbox );
}


void TabView::slotToggleSceneLight()
{
	m_system->getViewController()->toggleSceneLight();	
}

void TabView::slotCullFrontFaces( int state )
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
	settings.setValue("cullFrontFaces", m_cull_front );

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->m_rootnode->getOrCreateStateSet() );
}

void TabView::slotCullBackFaces( int state )
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
	settings.setValue("cullBackFaces", m_cull_back );

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->m_rootnode->getOrCreateStateSet() );
}

void TabView::slotProjectionButtonClicked( int btn )
{
	if( btn == 0 )
	{
		m_vw->setProjection( ViewerWidget::PROJECTION_PERSPECTIVE );
	}
	else if( btn == 1 )
	{
		m_vw->setProjection( ViewerWidget::PROJECTION_PARALLEL );
	}
}

void TabView::slotSetNumVertices( int num_vertices )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "NumVerticesPerCircle", num_vertices );
	m_system->getReaderWriterIFC()->getGeomSettings()->m_num_vertices_per_circle = num_vertices;
}

void TabView::slotShowCurves( int state )
{
	bool curves_on = state == Qt::Checked;
	m_system->getViewController()->switchCurveRepresentation( m_system->getViewController()->m_sw_model, curves_on );
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "ShowCurveRepresentations", curves_on );
}
