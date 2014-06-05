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

#include <QtCore/qglobal.h>
#include <QtCore/QSettings>
#include <QToolButton>
#include <QBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>

#include <osgGA/OrbitManipulator>

#include <ifcpp/reader/IfcPlusPlusReader.h>
#include <ifcpp/writer/IfcStepWriter.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcppgeometry/GeomUtils.h>

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
	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );

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

	// perspective/parallel projection
	//QRadioButton* radio_perspective = new QRadioButton("Perspective");
	//QRadioButton* radio_parallel = new QRadioButton("Parallel");
	//radio_parallel->setChecked(true);
	//QButtonGroup* btn_group_projection = new QButtonGroup();
	//btn_group_projection->addButton( radio_perspective, 0 );
	//btn_group_projection->addButton( radio_parallel, 1 );
	//connect( btn_group_projection, SIGNAL( buttonClicked( int ) ), this, SLOT( slotProjectionButtonClicked( int ) ) );

	// positive z axis
	QCheckBox* check_pos_z_down = new QCheckBox("Positive z-axis down");
	connect( check_pos_z_down, SIGNAL( stateChanged(int) ), this, SLOT( slotZAxisDown(int) ) );

	// layout
	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget( btn_toggle_light, 0 );
	hbox->addWidget( cull_front_faces, 0 );
	hbox->addWidget( cull_back_faces, 0 );
	hbox->addWidget( check_pos_z_down, 0 );
	hbox->addStretch( 1 );

	//QHBoxLayout* hbox_projection = new QHBoxLayout();
	//hbox_projection->addWidget( new QLabel("<b>Projection:</b>") );
	//hbox_projection->addSpacing(10);
	//hbox_projection->addWidget( radio_perspective );
	//hbox_projection->addWidget( radio_parallel );
	//hbox_projection->addStretch( 1 );

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addLayout( hbox );
//	vbox->addLayout( hbox_projection );
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

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );
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

	GeomUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );
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

void TabView::slotZAxisDown( int state )
{
	if( state == Qt::Checked )
	{
//		m_vw->getCameraManager()->setZAxisDown( true );
	}
	else
	{
	//	m_vw->getCameraManager()->setZAxisDown( false );
	}
}
