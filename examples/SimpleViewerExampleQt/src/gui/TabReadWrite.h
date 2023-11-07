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

#include <map>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>

#include <QtCore/qglobal.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextEdit>

class ViewerWidget;
class BuildingEntity;
class IfcPlusPlusSystem;

class TabReadWrite : public QWidget
{
	Q_OBJECT
public:
	TabReadWrite( IfcPlusPlusSystem* sys, ViewerWidget* viewer, QWidget* parent = 0 );
	~TabReadWrite();

	static void messageTarget( void* obj_ptr, shared_ptr<StatusCallback::Message> t );
	void closeEvent( QCloseEvent *event );

	void txtOut( QString txt );
	void txtOutWarning( QString txt );
	void txtOutError( QString txt );
	void progressValue( double progress_value, const std::string& progress_type );
	void clearTxtOut();
	void loadIfcFile( QString& path );
	void updateRecentFilesCombo();

protected:
	void keyPressEvent( QKeyEvent* event );
	
	IfcPlusPlusSystem*	m_system;
	ViewerWidget*	m_viewer;
	QTextEdit*		m_txt_out;
	
	QComboBox*		m_combo_recent_files;
	QStringList		m_recent_files;
	QPushButton*	m_btn_load;
	QWidget*		m_io_widget;

	QLineEdit*		m_le_path_write;
	QSplitter*		m_io_splitter;
	std::mutex m_mutex_messages;

signals:
	void signalProgressValue(double progress_value, const std::string& progress_type);

private slots:
	void slotAddOtherIfcFileClicked();
	void slotLoadRecentIfcFileClicked();
	void slotSetWritePathClicked();
	void slotWriteFileClicked();
	void slotClearRecentIfcFiles();
	void slotRecentFilesIndexChanged(int);
};
