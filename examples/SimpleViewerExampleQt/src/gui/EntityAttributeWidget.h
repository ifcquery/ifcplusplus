#pragma once

#include <set>
#include <unordered_set>
#include <QTreeWidget>
#include <QTableWidget>
#include <carve/mesh.hpp>
#include <ifcpp/model/BasicTypes.h>
#include "ScopedLockAndBlock.h"

class BuildingObject;
class BuildingEntity;
class IfcPlusPlusSystem;

class EntityAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	EntityAttributeWidget(IfcPlusPlusSystem* sys, QWidget* parent = 0 );
	~EntityAttributeWidget();

	void appendAttributeChildren( shared_ptr<BuildingObject>& entity, QTreeWidgetItem* item_parent, bool expand_children );
	void clearAttributeTree();
	void expandToLevel( QTreeWidgetItem* item, int& current_level, int max_level );

public slots:
	void slotObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map );
	void slotObjectsUnselected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map );
	void slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous );
	void slotItemExpanded(QTreeWidgetItem*);
	void slotItemChanged( QTreeWidgetItem*, int );

	void slotModelCleared();
	void slotModelLoadingStart();
	void slotModelLoadingDone();
	void slotExpandOneClicked();
	void slotCollapseOneClicked();

protected:
	IfcPlusPlusSystem*	m_system;
	QTreeWidget*		m_tree_widget;
	bool				m_block_selection_signals;
	ScopedBlockCount	m_block_counter;
	QTreeWidgetItem*	m_current_item;
	int					m_expand_level;
	std::unordered_set<BuildingObject*> m_expanded_entities;
	shared_ptr<BuildingObject> m_current_root;
	std::map<QTreeWidgetItem*, shared_ptr<BuildingObject> > m_map_item_objects;
	QTreeWidget*		m_computed_property_tree;
};
