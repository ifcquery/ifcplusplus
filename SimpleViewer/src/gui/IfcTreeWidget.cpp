
#include <ifcpp/model/IfcPPObject.h>
#include <ifcpp/model/IfcPPModel.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcSpatialStructureElement.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement.h>
#include <ifcpp/IFC4/include/IfcAxis2Placement3D.h>
#include <ifcpp/IFC4/include/IfcGeometricRepresentationContext.h>
#include <ifcpp/IFC4/include/IfcDirection.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcLabel.h>


#include "viewer/ViewerUtil.h"
#include "IfcPlusPlusSystem.h"
#include "IfcTreeWidget.h"

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
	setSelectionBehavior( QAbstractItemView::SelectRows );
	setIndentation( 4 );

	connect( this, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( slotTreewidgetSelectionChanged(QTreeWidgetItem*, QTreeWidgetItem*) ) );
	connect( m_system, SIGNAL( signalObjectsSelected(std::map<int, shared_ptr<IfcPPEntity> >&) ),	this, SLOT( slotObjectsSelected(std::map<int, shared_ptr<IfcPPEntity> >&) ) );

	connect( m_system, SIGNAL( signalModelCleared() ),		this, SLOT( slotModelCleared() ) );
	connect( m_system, SIGNAL( signalModelLoadingStart() ),	this, SLOT( slotModelLoadingStart() ) );
	connect( m_system, SIGNAL( signalModelLoadingDone() ),	this, SLOT( slotModelLoadingDone() ) );
}

IfcTreeWidget::~IfcTreeWidget()
{
}

void IfcTreeWidget::slotObjectsSelected( std::map<int, shared_ptr<IfcPPEntity> >& map )
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
	shared_ptr<IfcPPEntity>& object = (*(map.begin())).second;
	int selected_id = object->getId();

	for( int i=0; i<topLevelItemCount(); ++i )
	{
		QTreeWidgetItem* toplevel_item = topLevelItem( i );
		QTreeWidgetItem* selected_item = findItemByIfcId( toplevel_item, selected_id );
		if( selected_item != 0 )
		{
			blockSignals(true);
			setCurrentItem( selected_item, 1, QItemSelectionModel::SelectCurrent );
			blockSignals(false);
			break;
		}
	}
}

void IfcTreeWidget::slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
	const std::map<int,shared_ptr<IfcPPEntity> >& map_ifc_objects = m_system->getIfcModel()->getMapIfcObjects();
	std::map<int,shared_ptr<IfcPPEntity> >::const_iterator it_find;
	if( previous )
	{
		int id = previous->text(1).toUInt();
		it_find = map_ifc_objects.find(id);
		if( it_find != map_ifc_objects.end() )
		{
			shared_ptr<IfcPPEntity> ifc_object = it_find->second;
			//const shared_ptr<IfcPPEntity> ifc_object = map_ifc_objects[id];
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
			shared_ptr<IfcPPEntity> ifc_object = it_find->second;
			m_block_selection_signals = true;
			m_system->setObjectSelected( ifc_object, true );
			m_block_selection_signals = false;
		}
	}
}

void IfcTreeWidget::slotTreewidgetSelectionChanged()
{

}

void IfcTreeWidget::slotModelCleared()
{
	blockSignals(true);
	clear();
	blockSignals(false);
}

void IfcTreeWidget::slotModelLoadingStart()
{
}

QTreeWidgetItem* resolveTreeItems( shared_ptr<IfcPPObject> obj, std::set<int>& set_visited )
{
	QTreeWidgetItem* item = nullptr;

	std::vector<shared_ptr<IfcObjectDefinition> >::iterator it_object_def;
	std::vector<shared_ptr<IfcProduct> >::iterator it_product;

	shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
	if( obj_def )
	{
		if( set_visited.find( obj_def->getId() ) != set_visited.end() )
		{
			return nullptr;
		}
		set_visited.insert( obj_def->getId() );


		item = new QTreeWidgetItem();
		
		if( obj_def->m_Name )
		{
			if( obj_def->m_Name->m_value.size() > 0 )
			{
				item->setText( 0, QString::fromLocal8Bit( obj_def->m_Name->m_value.c_str() ) );
			}
		}
		item->setText( 1, QString::number( obj_def->getId() ) );
		item->setText( 2, obj_def->classname() );

		if( obj_def->m_IsDecomposedBy_inverse.size() > 0 )
		{
			std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = obj_def->m_IsDecomposedBy_inverse;
			std::vector<weak_ptr<IfcRelAggregates> >::iterator it;
			for( it=vec_IsDecomposedBy.begin(); it!=vec_IsDecomposedBy.end(); ++it )
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
	shared_ptr<IfcProject> project = m_system->getIfcModel()->getIfcProject();
	if( project )
	{
		std::set<int> set_visited;
		QTreeWidgetItem* project_item = resolveTreeItems( project, set_visited );
		if( project_item != NULL )
		{
			blockSignals(true);
			insertTopLevelItem( 0, project_item );
			blockSignals(false);
		}
	}
}

