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

#pragma once

#include <QtWidgets/QTreeWidget>
#include "ifcpp/model/BasicTypes.h"

class BuildingEntity;
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
	void slotObjectsSelected( std::unordered_map<std::string, shared_ptr<BuildingEntity> >& map );
	void slotTreewidgetSelectionChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous );
	void slotTreewidgetSelectionChanged();
	void slotTreeWidgetItemDoubleClick( QTreeWidgetItem* item, int column );

	void slotModelCleared();
	void slotModelLoadingStart();
	void slotModelLoadingDone();

protected:
	IfcPlusPlusSystem*	m_system;
	bool				m_block_selection_signals;
};
