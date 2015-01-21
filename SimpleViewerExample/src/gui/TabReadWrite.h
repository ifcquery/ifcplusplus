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

#pragma once

#include <map>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QSplitter>
#include <QApplication>
#include <QTextEdit>

class ViewerWidget;
class IfcPPEntity;
class IfcPlusPlusSystem;

class TabReadWrite : public QWidget
{
	Q_OBJECT
public:
	TabReadWrite( IfcPlusPlusSystem* sys, ViewerWidget* viewer, QWidget* parent = 0 );
	~TabReadWrite();

	static void slotMessageWrapper( void* obj_ptr, shared_ptr<StatusCallback::Message> t );
	void closeEvent( QCloseEvent *event );

public slots:
	void slotTxtOut( QString txt );
	void slotTxtOutWarning( QString txt );
	void slotTxtOutError( QString txt );
	void slotProgressValue( double progress_value, const std::string& progress_type );

	void slotLoadIfcFile( QString& path );
	void slotRecentFilesIndexChanged(int);
	void updateRecentFilesCombo();
	
protected:
	void keyPressEvent( QKeyEvent* event );
	
	IfcPlusPlusSystem*	m_system;
	ViewerWidget*	m_viewer;
	QTextEdit*		m_txt_out;
	QProgressBar*	m_progress_bar;
	
	QComboBox*		m_combo_recent_files;
	QStringList		m_recent_files;
	QPushButton*	m_btn_load;
	QWidget*		m_io_widget;

	QLineEdit*		m_le_path_write;
	QSplitter*		m_io_splitter;

#ifdef IFCPP_OPENMP
	Mutex m_mutex_messages;
#endif

private slots:
	void slotAddOtherIfcFileClicked();
	void slotLoadRecentIfcFileClicked();
	void slotSetWritePathClicked();
	void slotWriteFileClicked();
	void slotClearRecentIfcFiles();
};
