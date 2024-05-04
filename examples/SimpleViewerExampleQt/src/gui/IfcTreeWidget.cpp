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

#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/BuildingObject.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/IFC4X3/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4X3/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4X3/include/IfcDirection.h>
#include <ifcpp/IFC4X3/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4X3/include/IfcLabel.h>
#include <ifcpp/IFC4X3/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4X3/include/IfcProject.h>
#include <ifcpp/IFC4X3/include/IfcRelAggregates.h>
#include <ifcpp/IFC4X3/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4X3/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4X3/include/IfcTypeProduct.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "IfcTreeWidget.h"
#include "viewer/ViewerUtil.h"

#include <QtWidgets/QHeaderView>

IfcTreeWidget::IfcTreeWidget( IfcPlusPlusSystem* sys, QWidget* parent ) : QTreeWidget(parent), m_system(sys)
{
	setColumnCount( 2 );
	QStringList tree_headers;
	tree_headers << "Label" << "Object id" << "Class name";
	setHeaderLabels( tree_headers );
	setColumnWidth( 0, 100 );
	setColumnWidth( 1, 60 );
	setColumnWidth( 2, 60 );
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setIndentation( 12 );

	connect( this,		&QTreeWidget::currentItemChanged,			this, &IfcTreeWidget::slotTreewidgetSelectionChanged );
	connect( this,		&QTreeWidget::itemDoubleClicked,			this, &IfcTreeWidget::slotTreeWidgetItemDoubleClick);
	connect( m_system,	&IfcPlusPlusSystem::signalObjectsSelected,	this, &IfcTreeWidget::slotObjectsSelected );
	connect( m_system,	&IfcPlusPlusSystem::signalModelCleared,		this, &IfcTreeWidget::slotModelCleared );
	connect( m_system,	&IfcPlusPlusSystem::signalModelLoadingStart,this, &IfcTreeWidget::slotModelLoadingStart );
	connect( m_system,	&IfcPlusPlusSystem::signalModelLoadingDone,	this, &IfcTreeWidget::slotModelLoadingDone );
}

IfcTreeWidget::~IfcTreeWidget(){}

void IfcTreeWidget::slotObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map )
{
	if( m_block_selection_signals )
	{
		return;
	}

	if( map.size() < 1 )
	{
		return;
	}

	// take the first object from map and highlight it
	shared_ptr<BuildingEntity> object = (*(map.begin())).second;
	int selected_id = object->m_tag;

	for( int i=0; i<topLevelItemCount(); ++i )
	{
		QTreeWidgetItem* toplevel_item = topLevelItem( i );
		QTreeWidgetItem* selected_item = findItemByIfcId( toplevel_item, selected_id );
		if( selected_item != 0 )
		{
			blockSignals(true);
			m_block_selection_signals = true;
			setCurrentItem( selected_item, 1, QItemSelectionModel::SelectCurrent );
			blockSignals(false);
			m_block_selection_signals = false;
			break;
		}
	}
}

void IfcTreeWidget::slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
	if( m_block_selection_signals )
	{
		return;
	}
	const std::unordered_map<int,shared_ptr<BuildingEntity> >& map_ifc_objects = m_system->getIfcModel()->getMapIfcEntities();

	if (!m_system->isCtrlKeyDown())
	{
		m_system->clearSelection();
	}
	else
	{
		if (previous)
		{
			int id = previous->text(1).toUInt();
			auto it_find = map_ifc_objects.find(id);
			if (it_find != map_ifc_objects.end())
			{
				shared_ptr<BuildingEntity> ifc_object = it_find->second;
				m_block_selection_signals = true;
				m_system->setObjectSelected(ifc_object, false);
				m_block_selection_signals = false;
			}
		}
	}

	if( current )
	{
		int id = current->text(1).toUInt();
		auto it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_block_selection_signals = true;
			m_system->setObjectSelected( ifc_object, true );
			m_block_selection_signals = false;
		}
	}
}

void IfcTreeWidget::slotTreeWidgetItemDoubleClick( QTreeWidgetItem* item, int column )
{
	if( m_block_selection_signals )
	{
		return;
	}

	if( !item )
	{
		return;
	}

	const std::unordered_map<int,shared_ptr<BuildingEntity>>& map_ifc_objects = m_system->getIfcModel()->getMapIfcEntities();
	const int id = item->text(1).toUInt();
	auto it_find = map_ifc_objects.find(id);

	if( it_find == map_ifc_objects.end() )
	{
		return;
	}

	m_block_selection_signals = true;
	emit( signalZoomToObject(it_find->second) );
	m_block_selection_signals = false;
}

void IfcTreeWidget::slotModelCleared()
{
	m_block_selection_signals = true;
	blockSignals(true);
	clear();
	blockSignals(false);
	m_block_selection_signals = false;
}

void IfcTreeWidget::slotModelLoadingStart()
{
	slotModelCleared();
}

bool hasParentInBuildingStructure(shared_ptr<BuildingObject> obj)
{
	shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
	if (!obj_def)
	{
		return true;
	}

	shared_ptr<IfcTypeProduct> typeProduct = dynamic_pointer_cast<IfcTypeProduct>(obj);
	if (typeProduct)
	{
		return true;
	}

	if (obj_def->m_Decomposes_inverse.size() > 0)
	{
		return true;
	}

	shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(obj_def);
	if (spatial_ele)
	{
		std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
		if (vec_contained.size() > 0)
		{

			for (auto it_rel_contained = vec_contained.begin(); it_rel_contained!=vec_contained.end(); ++it_rel_contained)
			{
				shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(*it_rel_contained);

				//std::vector<shared_ptr<IfcProduct> >	m_RelatedElements;
				//shared_ptr<IfcSpatialElement>			m_RelatingStructure;

				std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

				if (rel_contained->m_RelatingStructure == spatial_ele)
				{
					if (vec_related_elements.size() > 0)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

QTreeWidgetItem* resolveTreeItems( shared_ptr<BuildingObject> obj, std::unordered_set<int>& set_visited )
{
	QTreeWidgetItem* item = nullptr;

	std::vector<shared_ptr<IfcObjectDefinition> >::iterator it_object_def;
	std::vector<shared_ptr<IfcProduct> >::iterator it_product;

	shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
	if( obj_def )
	{
		if( set_visited.find( obj_def->m_tag ) != set_visited.end() )
		{
			return nullptr;
		}
		set_visited.insert( obj_def->m_tag );

		shared_ptr<IfcOpeningElement> opening = dynamic_pointer_cast<IfcOpeningElement>(obj);
		if (opening)
		{
			return nullptr;
		}

		item = new QTreeWidgetItem();
		
		if( obj_def->m_Name )
		{
			std::string name = obj_def->m_Name->m_value;
			if( name.size() > 0 )
			{
				item->setText(0, QString::fromStdString(name));
			}
		}

		item->setText( 1, QString::number( obj_def->m_tag ) );
		item->setText( 2, EntityFactory::getStringForClassID(obj_def->classID()) );

		if( obj_def->m_IsDecomposedBy_inverse.size() > 0 )
		{
			std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = obj_def->m_IsDecomposedBy_inverse;
			for( auto it=vec_IsDecomposedBy.begin(); it!=vec_IsDecomposedBy.end(); ++it )
			{
				shared_ptr<IfcRelAggregates> rel_agg( *it );
				std::vector<shared_ptr<IfcObjectDefinition> >& vec = rel_agg->m_RelatedObjects;
	
				for( it_object_def=vec.begin(); it_object_def!=vec.end(); ++it_object_def )
				{
					shared_ptr<IfcObjectDefinition> child_obj_def = (*it_object_def);
					QTreeWidgetItem* child_tree_item = resolveTreeItems( child_obj_def, set_visited );
					if( child_tree_item != nullptr )
					{
						item->addChild( child_tree_item );
					}
				}
			}
		}

		shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(obj_def);
		if( spatial_ele )
		{
			std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
			if( vec_contained.size() > 0 )
			{
				std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >::iterator it_rel_contained;
				for( it_rel_contained=vec_contained.begin(); it_rel_contained!=vec_contained.end(); ++it_rel_contained )
				{
					shared_ptr<IfcRelContainedInSpatialStructure> rel_contained( *it_rel_contained );
					std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;
					std::vector<shared_ptr<IfcProduct> >::iterator it;
			
					for( it=vec_related_elements.begin(); it!=vec_related_elements.end(); ++it )
					{
						shared_ptr<IfcProduct> related_product = (*it);
				
						QTreeWidgetItem* child_tree_item = resolveTreeItems( related_product, set_visited );
						if( child_tree_item != NULL )
						{
							item->addChild( child_tree_item );
						}
					}
				}
			}
		}
	}
	
	return item;
}

void IfcTreeWidget::slotModelLoadingDone()
{
	std::unordered_set<int> set_visited;
	shared_ptr<IfcProject> project = m_system->getIfcModel()->getIfcProject();
	if( project )
	{
		
		QTreeWidgetItem* project_item = resolveTreeItems( project, set_visited );
		if( project_item != NULL )
		{
			m_block_selection_signals = true;
			blockSignals(true);
			insertTopLevelItem( 0, project_item );
			setCurrentItem( project_item );
			blockSignals(false);
			m_block_selection_signals = false;
		}
	}

	std::unordered_map<std::string, shared_ptr<BuildingObject> >&	map_outside = m_system->getGeometryConverter()->getObjectsOutsideSpatialStructure();
	
	if( map_outside.size() > 0 )
	{
		QTreeWidgetItem* item_outside = new QTreeWidgetItem();
		item_outside->setText( 0, "OutsideSpatialStructure" );

		for( auto it = map_outside.begin(); it != map_outside.end(); ++it )
		{
			shared_ptr<BuildingObject>& ifc_object = it->second;

			shared_ptr<IfcOpeningElement> opening = dynamic_pointer_cast<IfcOpeningElement>(ifc_object);
			if (opening)
			{
				continue;
			}

			if (hasParentInBuildingStructure(ifc_object))
			{
				continue;
			}

			QTreeWidgetItem* object_item = resolveTreeItems( ifc_object, set_visited );
			if( object_item != NULL )
			{
				blockSignals( true );
				item_outside->addChild( object_item );
				blockSignals( false );
			}
		}

		if (item_outside->childCount() > 0)
		{
			m_block_selection_signals = true;
			blockSignals(true);
			insertTopLevelItem(topLevelItemCount(), item_outside);
			setCurrentItem(item_outside);
			blockSignals(false);
			m_block_selection_signals = false;
		}
	}
	expandToDepth(3);
}
