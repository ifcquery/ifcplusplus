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

#include <ifcpp/model/BuildingModel.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewerWidget.h"
#include "viewer/OrbitCameraManipulator.h"

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
	SceneGraphUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getRootNode()->getOrCreateStateSet() );

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
		m_system->getGeometryConverter()->getGeomSettings()->setNumVerticesPerCircle( num_vertices );
	}
	else
	{
		int num_vertices = m_system->getGeometryConverter()->getGeomSettings()->getNumVerticesPerCircle();
		m_spinbox_circle_vertices->setValue(num_vertices);
	}

	QHBoxLayout* num_vertices_hbox = new QHBoxLayout();
	num_vertices_hbox->addWidget( m_spinbox_circle_vertices );
	num_vertices_hbox->addWidget( new QLabel( tr("Number of vertices per circle") ) );
	connect(m_spinbox_circle_vertices, SIGNAL(valueChanged(int)), this, SLOT( slotSetNumVertices( int ) ) );

	// ignore profile radius
	QCheckBox* ignore_profile_radius_checkbox = new QCheckBox("Ignore profile radius");
	if (m_system->getGeometryConverter()->getGeomSettings()->isIgnoreProfileRadius())
	{
		ignore_profile_radius_checkbox->setChecked(true);
	}
	if (keys.contains("IgnoreProfileRadius"))
	{
		bool ignore_profile_radius = settings.value("IgnoreProfileRadius").toBool();
		if (ignore_profile_radius)
		{
			ignore_profile_radius_checkbox->setChecked(true);
			m_system->getGeometryConverter()->getGeomSettings()->setIgnoreProfileRadius(true);
		}
	}
	connect(ignore_profile_radius_checkbox, &QCheckBox::stateChanged, this, &TabView::slotIgnoreProfileRadius);


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
	connect( m_check_show_curve_representations, SIGNAL( stateChanged(int) ), this, SLOT( slotShowCurves(int) ) );

	// layout
	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget( btn_toggle_light, 0 );
	hbox->addWidget( cull_front_faces, 0 );
	hbox->addWidget( cull_back_faces, 0 );
	hbox->addLayout( num_vertices_hbox );
	hbox->addWidget( ignore_profile_radius_checkbox, 0);
	hbox->addWidget( m_check_show_curve_representations );
	hbox->addStretch( 1 );

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addLayout( hbox );
	vbox->addStretch(1);

	setLayout( vbox );
}

void TabView::slotToggleSceneLight()
{
	m_system->toggleSceneLight();
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

	SceneGraphUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getRootNode()->getOrCreateStateSet() );
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

	SceneGraphUtils::cullFrontBack( m_cull_front, m_cull_back, m_system->getRootNode()->getOrCreateStateSet() );
}

void TabView::slotSetNumVertices( int num_vertices )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "NumVerticesPerCircle", num_vertices );
	m_system->getGeometryConverter()->getGeomSettings()->setNumVerticesPerCircle( num_vertices );
}

void TabView::slotShowCurves( int state )
{
	bool curves_on = state == Qt::Checked;
	m_system->switchCurveRepresentation( m_system->getModelNode(), curves_on );
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "ShowCurveRepresentations", curves_on );
}

void TabView::slotIgnoreProfileRadius(int state)
{
	if (state == Qt::Checked)
	{
		m_system->getGeometryConverter()->getGeomSettings()->setIgnoreProfileRadius(true);
	}
	else
	{
		m_system->getGeometryConverter()->getGeomSettings()->setIgnoreProfileRadius(false);
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("IgnoreProfileRadius", m_system->getGeometryConverter()->getGeomSettings()->isIgnoreProfileRadius());
}
