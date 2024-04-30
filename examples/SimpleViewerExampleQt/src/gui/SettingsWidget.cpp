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

#include <QtCore/qglobal.h>
#include <QPainter>
#include <QSettings>
#include <QToolButton>
#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>

#include <osgGA/OrbitManipulator>

#include <ifcpp/model/BuildingModel.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "viewer/ViewerWidget.h"
#include "viewer/ViewController.h"
#include "viewer/Orbit3DManipulator.h"

#include "MainWindow.h"
#include "SettingsWidget.h"

SettingsWidget::SettingsWidget( IfcPlusPlusSystem* sys, ViewerWidget* vw, bool autoHideFileWidget) : m_system(sys), m_vw(vw)
{
	setObjectName("SettingsWidget");

	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	QStringList keys = settings.allKeys();
	
	m_cullFront = false;
	m_cullBack = true; // default to back face culling
	if( keys.contains( "cullFrontFaces" ) )
	{
		m_cullFront = settings.value("cullFrontFaces").toBool();
	}
	if( keys.contains( "cullBackFaces" ) )
	{
		m_cullBack = settings.value("cullBackFaces").toBool();
	}
	SceneGraphUtils::cullFrontBack(m_cullFront, m_cullBack, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );

	// cull face buttons
	QCheckBox* cull_front_faces = new QCheckBox( "Cull front faces" );
	if(m_cullFront)
	{
		cull_front_faces->setChecked( true );
	}
	connect( cull_front_faces, SIGNAL( stateChanged( int ) ), this, SLOT( slotCullFrontFaces( int ) ) );

	QCheckBox* cull_back_faces = new QCheckBox( "Cull back faces" );
	if(m_cullBack)
	{
		cull_back_faces->setChecked( true );
	}
	connect( cull_back_faces, SIGNAL( stateChanged( int ) ), this, SLOT( slotCullBackFaces( int ) ) );

	// light button
	QCheckBox* btn_toggle_light = new QCheckBox("Light on/off");
	btn_toggle_light->setToolTip("Light on/off");
	btn_toggle_light->setChecked(m_system->getViewController()->isSunLightOn());
	connect(btn_toggle_light, &QCheckBox::clicked, this, [this]() { m_system->getViewController()->toggleSunLight(); });

	// number of vertices per cycle
	m_spinboxCircleVertices = new QSpinBox( this );
	m_spinboxCircleVertices->setRange(6, 48);
	m_spinboxCircleVertices->setSingleStep(1);
	m_spinboxCircleVertices->setValue(10);
	m_spinboxCircleVertices->setMinimumWidth(70);
	m_spinboxCircleVertices->setMinimumHeight(28);
	if( keys.contains( "NumVerticesPerCircle" ) )
	{
		int num_vertices = settings.value("NumVerticesPerCircle").toInt();
		m_spinboxCircleVertices->setValue(num_vertices);
		m_system->getGeometrySettings()->setNumVerticesPerCircle( num_vertices );
	}
	else
	{
		int num_vertices = m_system->getGeometrySettings()->getNumVerticesPerCircle();
		m_spinboxCircleVertices->setValue(num_vertices);
	}

	QHBoxLayout* num_vertices_hbox = new QHBoxLayout();
	num_vertices_hbox->addWidget( m_spinboxCircleVertices );
	num_vertices_hbox->addWidget( new QLabel( tr("Number of vertices per circle") ) );
	connect(m_spinboxCircleVertices, SIGNAL(valueChanged(int)), this, SLOT( slotSetNumVertices( int ) ) );

	// ignore profile radius
	QCheckBox* ignore_profile_radius_checkbox = new QCheckBox("Ignore profile radius");
	if (m_system->getGeometrySettings()->isIgnoreProfileRadius())
	{
		ignore_profile_radius_checkbox->setChecked(true);
	}
	if (keys.contains("IgnoreProfileRadius"))
	{
		bool ignore_profile_radius = settings.value("IgnoreProfileRadius").toBool();
		if (ignore_profile_radius)
		{
			ignore_profile_radius_checkbox->setChecked(true);
			m_system->getGeometrySettings()->setIgnoreProfileRadius(true);
		}
	}
	connect(ignore_profile_radius_checkbox, &QCheckBox::stateChanged, this, &SettingsWidget::slotIgnoreProfileRadius);


	m_checkShowCurveRepresentations = new QCheckBox(tr("Show curve representations"));
	bool show_curves = true;
	if( keys.contains( "ShowCurveRepresentations" ) )
	{
		show_curves = settings.value( "ShowCurveRepresentations" ).toBool();
		if( show_curves )
		{
			m_checkShowCurveRepresentations->setChecked( true );
		}
	}
	connect( m_checkShowCurveRepresentations, SIGNAL( stateChanged(int) ), this, SLOT( slotShowCurves(int) ) );

	QCheckBox* checkAutoHideFileWidget = new QCheckBox("Auto hide file open widget after file is loaded");
	connect(checkAutoHideFileWidget, &QCheckBox::stateChanged, this, &SettingsWidget::slotAutoHideFileWidget);

	if (autoHideFileWidget)
	{
		checkAutoHideFileWidget->setChecked(true);
	}

	// layout
	setContentsMargins(0, 0, 0, 0);
	QHBoxLayout* hbox1 = new QHBoxLayout();
	hbox1->addWidget(checkAutoHideFileWidget, 0);
	hbox1->addStretch(1);

	QHBoxLayout* hbox2 = new QHBoxLayout();
	hbox2->addWidget( btn_toggle_light, 0 );
	hbox2->addWidget( cull_front_faces, 0 );
	hbox2->addWidget( cull_back_faces, 0 );
	hbox2->addStretch(1);

	QHBoxLayout* hbox3 = new QHBoxLayout();
	hbox3->addLayout( num_vertices_hbox );
	hbox3->addWidget( ignore_profile_radius_checkbox, 0);
	hbox3->addWidget( m_checkShowCurveRepresentations );
	hbox3->addStretch(1);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->setContentsMargins(4, 4, 4, 4);
	vbox->addWidget(new QLabel("<b>Application settings:</b>"), 0);
	vbox->addLayout(hbox1, 0);
	vbox->addSpacing(15);
	vbox->addWidget(new QLabel("<b>Visualization settings:</b>"), 0);
	vbox->addLayout(hbox2, 0);
	vbox->addSpacing(15);
	vbox->addWidget(new QLabel("<b>Model settings that require a re-load:</b>"), 0);
	vbox->addLayout(hbox3, 0);

	vbox->setSizeConstraint(QLayout::SetMinimumSize);
	
	setLayout( vbox );
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void SettingsWidget::paintEvent(QPaintEvent* event)
{
	QColor backgroundColor = palette().light().color();
	backgroundColor.setAlpha(100);
	QPainter customPainter(this);
	customPainter.fillRect(rect(), backgroundColor);
	QWidget::paintEvent(event);
}

void SettingsWidget::slotAutoHideFileWidget(int state)
{
	bool autoHide = state == Qt::Checked;
	emit(signalAutoHideFileWidget(autoHide));
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("AutoHideFileWidget", autoHide);
}

void SettingsWidget::slotCullFrontFaces( int state )
{
	if( state == Qt::Checked )
	{
		m_cullFront = true;
	}
	else
	{
		m_cullFront = false;
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("cullFrontFaces", m_cullFront );

	SceneGraphUtils::cullFrontBack(m_cullFront, m_cullBack, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );
}

void SettingsWidget::slotCullBackFaces( int state )
{
	if( state == Qt::Checked )
	{
		m_cullBack = true;
	}
	else
	{
		m_cullBack = false;
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("cullBackFaces", m_cullBack );

	SceneGraphUtils::cullFrontBack( m_cullFront, m_cullBack, m_system->getViewController()->getRootNode()->getOrCreateStateSet() );
}

void SettingsWidget::slotSetNumVertices( int num_vertices )
{
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "NumVerticesPerCircle", num_vertices );
	m_system->getGeometrySettings()->setNumVerticesPerCircle( num_vertices );
}

void SettingsWidget::slotShowCurves( int state )
{
	bool curves_on = state == Qt::Checked;
	m_system->getViewController()->switchCurveRepresentation( m_system->getViewController()->getModelNode(), curves_on );
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue( "ShowCurveRepresentations", curves_on );
}

void SettingsWidget::slotIgnoreProfileRadius(int state)
{
	if (state == Qt::Checked)
	{
		m_system->getGeometrySettings()->setIgnoreProfileRadius(true);
	}
	else
	{
		m_system->getGeometrySettings()->setIgnoreProfileRadius(false);
	}
	QSettings settings(QSettings::UserScope, QLatin1String("IfcPlusPlus"));
	settings.setValue("IgnoreProfileRadius", m_system->getGeometrySettings()->isIgnoreProfileRadius());
}
