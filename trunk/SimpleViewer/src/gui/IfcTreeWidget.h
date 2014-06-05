#pragma once

#include <QTreeWidget>
#include "ifcpp/model/shared_ptr.h"

class IfcPPEntity;
class IfcPlusPlusSystem;

class IfcTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	IfcTreeWidget( IfcPlusPlusSystem* sys, QWidget* parent = 0 );
	~IfcTreeWidget();

	QModelIndex	indexFromItem( QTreeWidgetItem * item, int column = 0 ) const { return QTreeWidget::indexFromItem( item, column ); }
	QTreeWidgetItem* itemFromIndex( const QModelIndex & index ) const { return QTreeWidget::itemFromIndex( index ); }

public slots:
	void slotObjectsSelected( std::map<int, shared_ptr<IfcPPEntity> >& map );
	void slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous );
	void slotTreewidgetSelectionChanged();

	void slotModelCleared();
	void slotModelLoadingStart();
	void slotModelLoadingDone();

protected:
	IfcPlusPlusSystem*	m_system;
	bool				m_block_selection_signals;
};
