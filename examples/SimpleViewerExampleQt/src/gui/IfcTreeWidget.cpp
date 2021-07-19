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
#include <ifcpp/IFC4/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4/include/IfcTypeProduct.h>

#include "IncludeGeometryHeaders.h"
#include "IfcPlusPlusSystem.h"
#include "IfcTreeWidget.h"

#include <QtWidgets/QHeaderView>

QTreeWidgetItem* findItemByIfcId( QTreeWidgetItem* item, int ifc_id )
{
	int num_children = item->childCount();
	for( int i = 0; i<num_children; ++i )
	{
		QTreeWidgetItem* child = item->child( i );
		int id = child->text( 1 ).toUInt();
		if( id == ifc_id )
		{
			return child;
		}
		QTreeWidgetItem* child_of_child = findItemByIfcId( child, ifc_id );
		if( child_of_child != 0 )
		{
			return child_of_child;
		}
	}
	return 0;
}

IfcTreeWidget::IfcTreeWidget( IfcPlusPlusSystem* sys, QWidget* parent ) : QTreeWidget(parent), m_system(sys)
{
	m_block_selection_signals = false;
	setColumnCount( 2 );
	QStringList tree_headers;
	tree_headers << "Label" << "Object id" << "Class name";
	setHeaderLabels( tree_headers );
	setColumnWidth( 0, 100 );
	setColumnWidth( 1, 60 );
	setColumnWidth( 2, 60 );
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	header()->setSectionResizeMode(0, QHeaderView::Stretch);
#else
	header()->setResizeMode(0, QHeaderView::Stretch);
#endif
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setIndentation( 12 );

	connect( this, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( slotTreewidgetSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*) ) );
	connect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int )), this, SLOT(slotTreeWidgetItemDoubleClick(QTreeWidgetItem*, int)));
	connect( m_system, &IfcPlusPlusSystem::signalObjectsSelected,	this, &IfcTreeWidget::slotObjectsSelected );
	connect( m_system, SIGNAL( signalModelCleared() ),		this, SLOT( slotModelCleared() ) );
	connect( m_system, SIGNAL( signalModelLoadingStart() ),	this, SLOT( slotModelLoadingStart() ) );
	connect( m_system, SIGNAL( signalModelLoadingDone() ),	this, SLOT( slotModelLoadingDone() ) );
}

IfcTreeWidget::~IfcTreeWidget(){}

void IfcTreeWidget::slotObjectsSelected( std::map<std::string, shared_ptr<BuildingEntity> >& map )
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
	int selected_id = object->m_entity_id;

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
	const std::map<int,shared_ptr<BuildingEntity> >& map_ifc_objects = m_system->getIfcModel()->getMapIfcEntities();
	std::map<int,shared_ptr<BuildingEntity> >::const_iterator it_find;
	if( previous )
	{
		int id = previous->text(1).toUInt();
		it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			//const shared_ptr<BuildingEntity> ifc_object = map_ifc_objects[id];
			m_block_selection_signals = true;
			m_system->setObjectSelected( ifc_object, false );
			m_block_selection_signals = false;
		}
	}

	if( current )
	{
		int id = current->text(1).toUInt();
		it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<BuildingEntity> ifc_object = it_find->second;
			m_block_selection_signals = true;
			m_system->setObjectSelected( ifc_object, true );
			m_block_selection_signals = false;
		}
	}
}

void IfcTreeWidget::slotTreewidgetSelectionChanged()
{

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

	const std::map<int,shared_ptr<BuildingEntity>>& map_ifc_objects = m_system->getIfcModel()->getMapIfcEntities();
	const int id = item->text(1).toUInt();
	const std::map<int,shared_ptr<BuildingEntity>>::const_iterator it_find = map_ifc_objects.find(id);

	if( it_find == map_ifc_objects.end() )
	{
		return;
	}

	m_block_selection_signals = true;
	m_system->zoomToObject(it_find->second);
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
		if( set_visited.find( obj_def->m_entity_id ) != set_visited.end() )
		{
			return nullptr;
		}
		set_visited.insert( obj_def->m_entity_id );


		item = new QTreeWidgetItem();
		
		if( obj_def->m_Name )
		{
			if( obj_def->m_Name->m_value.size() > 0 )
			{
				item->setText(0, QString::fromStdWString(obj_def->m_Name->m_value));
			}
		}

		item->setText( 1, QString::number( obj_def->m_entity_id ) );
		item->setText( 2, obj_def->className() );

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

	std::map<std::string, shared_ptr<BuildingObject> >&	map_outside = m_system->getGeometryConverter()->getObjectsOutsideSpatialStructure();
	
	if( map_outside.size() > 0 )
	{
		QTreeWidgetItem* item_outside = new QTreeWidgetItem();
		item_outside->setText( 0, "OutsideSpatialStructure" );

		for( auto it = map_outside.begin(); it != map_outside.end(); ++it )
		{
			shared_ptr<BuildingObject>& ifc_object = it->second;

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

		m_block_selection_signals = true;
		blockSignals( true );
		insertTopLevelItem( topLevelItemCount(), item_outside );
		setCurrentItem( item_outside );
		blockSignals( false );
		m_block_selection_signals = false;
	}
	expandToDepth(2);
}
