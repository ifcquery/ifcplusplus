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

#include <regex>
#include <QSlider>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <osg/MatrixTransform>
#include <osg/Switch>

#include <ifcpp/model/BuildingModel.h>
#include <IfcProject.h>
#include <IfcRelAggregates.h>
#include <IfcSite.h>
#include <IfcBuilding.h>
#include <IfcBuildingStorey.h>
#include <IfcLengthMeasure.h>
#include <ifcpp/geometry/Carve/GeometryConverter.h>

#include "ScopedLockAndBlock.h"
#include "StoreyShiftWidget.h"
#include "IfcPlusPlusSystem.h"

StoreyShiftWidget::StoreyShiftWidget(IfcPlusPlusSystem* system )
{
	m_system = system;

	m_slide_x = new QSlider();
	m_slide_x->setOrientation( Qt::Horizontal );
	m_slide_x->setTickInterval(10);
    m_slide_x->setSingleStep(1);
	m_slide_x->setRange( -100, 100 );

	m_slide_y = new QSlider();
	m_slide_y->setOrientation( Qt::Horizontal );
	m_slide_y->setTickInterval(10);
    m_slide_y->setSingleStep(1);
	m_slide_y->setRange( -100, 100 );

	m_slide_z = new QSlider();
	m_slide_z->setOrientation( Qt::Horizontal );
	m_slide_z->setTickInterval(10);
    m_slide_z->setSingleStep(1);
	m_slide_z->setRange( -100, 100 );

	connect( m_slide_x, SIGNAL(valueChanged(int)), this, SLOT(slotSlideX(int)));
	connect( m_slide_y, SIGNAL(valueChanged(int)), this, SLOT(slotSlideY(int)));
	connect( m_slide_z, SIGNAL(valueChanged(int)), this, SLOT(slotSlideZ(int)));

	QLabel* label_x = new QLabel("x:");
	QLabel* label_y = new QLabel("y:");
	QLabel* label_z = new QLabel("z:");

	label_x->setMargin(0);
	label_y->setMargin(0);
	label_z->setMargin(0);
	label_x->setStyleSheet( "QLabel { height:8px; }" );

	QPushButton* btn_reset = new QPushButton( "Reset" );
	connect( btn_reset, SIGNAL(clicked()), this, SLOT(slotResetSliders()));

	QGridLayout* grid = new QGridLayout();
	grid->setContentsMargins( 0, 0, 0, 0 );
	grid->addWidget( new QLabel("Storey shift"), 0, 0 );
	grid->addWidget( btn_reset, 1, 0, 2, 1 );
	grid->addWidget( label_x, 0, 1 );
	grid->addWidget( m_slide_x, 0, 2 );
	grid->addWidget( label_y, 1, 1 );
	grid->addWidget( m_slide_y, 1, 2 );
	grid->addWidget( label_z, 2, 1 );
	grid->addWidget( m_slide_z, 2, 2 );
	
	setLayout( grid );
	setContentsMargins( 4, 0, 4, 0 );

	connect( m_system, SIGNAL( signalModelLoadingDone() ),	this, SLOT( slotModelLoadingDone() ) );
}

StoreyShiftWidget::~StoreyShiftWidget()
{
}

void StoreyShiftWidget::slotModelLoadingDone()
{
	slotResetSliders();
}

void StoreyShiftWidget::slotResetSliders()
{
	ScopedBlockSignals block1( m_slide_x, m_block_counter );
	m_slide_x->setValue( 0 );

	ScopedBlockSignals block2( m_slide_y, m_block_counter );
	m_slide_y->setValue( 0 );
	
	ScopedBlockSignals block3( m_slide_z, m_block_counter );
	m_slide_z->setValue( 0 );
	shiftStoreys();
}

void collectBuildingStoreys( osg::Group* grp, std::vector<std::pair<int, osg::MatrixTransform*> >& vec_transform )
{
	for( int i=0; i<grp->getNumChildren(); ++i )
	{
		osg::Node* child = grp->getChild( i );

		osg::Group* child_group = dynamic_cast<osg::Group*>( child );
		if( child_group )
		{
			const std::string& child_name = child->getName();

			if( child_name.size() > 0 )
			{
				if( child_name.at(0) == '#' )
				{
					if( child_name.find( "IfcBuildingStorey" ) != std::string::npos )
					{
						// building storeys need a switch and also a transform node

						// extract entity id
						std::cmatch res;
						std::regex rx( "#([0-9]+)" );
						if( std::regex_search( child_name.c_str(), res, rx ) )
						{
							const int id = atoi( res[1].str().c_str() );

							osg::MatrixTransform* child_transform = dynamic_cast<osg::MatrixTransform*>( child_group );
							if( child_transform )
							{
								vec_transform.push_back( std::make_pair( id, child_transform ) );
								//return;
							}
						}
					}
				}
			}

			collectBuildingStoreys( child_group, vec_transform );
		}
	}
}

void StoreyShiftWidget::shiftStoreys()
{
	const std::map<std::string, shared_ptr<ProductShapeData> >&	map_input_data = m_system->m_geometry_converter->getShapeInputData();
	const std::map<int,shared_ptr<BuildingEntity> >&	map_entities = m_system->m_geometry_converter->getBuildingModel()->getMapIfcEntities();

	std::vector<std::pair<int, osg::MatrixTransform*> > vec_transform;
	std::vector<std::pair<int, osg::MatrixTransform*> > vec_transform_no_elevation;
	osg::Switch* grp = m_system->getModelNode();
	collectBuildingStoreys( grp, vec_transform );

	std::map<double, std::pair<int, osg::MatrixTransform*> > map_transform;

	// order by elevation
	for( auto it = vec_transform.begin(); it != vec_transform.end(); ++it )
	{
		osg::MatrixTransform* transform = it->second;
		int id = it->first;
		auto it_find = map_entities.find( id );
		bool elevation_found = false;
		if( it_find != map_entities.end() )
		{
			const shared_ptr<BuildingEntity> entity = it_find->second;
			shared_ptr<IfcBuildingStorey> storey = dynamic_pointer_cast<IfcBuildingStorey>( entity );

			if( storey )
			{
				if( storey->m_Elevation )
				{
					double elevation = storey->m_Elevation->m_value;
					if( map_transform.find( elevation ) == map_transform.end() )
					{
						map_transform[elevation] = *it;
						elevation_found = true;
					}
				}
			}
		}

		if( !elevation_found )
		{
			vec_transform_no_elevation.push_back( *it );
		}
	}

	std::vector<osg::MatrixTransform*> vec_transform_sorted;

	for( auto it = map_transform.begin(); it != map_transform.end(); ++it )
	{
		osg::MatrixTransform* transform = it->second.second;
		vec_transform_sorted.push_back( transform );
	}

	for( size_t i = 0; i < vec_transform_no_elevation.size(); ++i )
	{
		vec_transform_sorted.push_back( vec_transform_no_elevation[i].second );
	}
	
	const osg::BoundingSphere bs = grp->getBound();
	double bs_radius = bs.radius();

	double slide_x = m_slide_x->value()*0.01*bs_radius;
	double slide_y = m_slide_y->value()*0.01*bs_radius;
	double slide_z = m_slide_z->value()*0.01*bs_radius;

	for( int i = 0; i < vec_transform_sorted.size(); ++i )
	{
		osg::MatrixTransform* transform = vec_transform_sorted[i];
		transform->setMatrix( osg::Matrix::translate( slide_x*double(i+1), slide_y*double(i+1), slide_z*double(i+1) ) );
	}
}

void StoreyShiftWidget::slotSlideX(int pos)
{
	shiftStoreys();
}

void StoreyShiftWidget::slotSlideY(int pos)
{
	shiftStoreys();
}

void StoreyShiftWidget::slotSlideZ(int pos)
{
	shiftStoreys();
}
