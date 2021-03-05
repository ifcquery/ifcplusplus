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

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>

class IfcPlusPlusSystem;
class TabReadWrite;
class ViewerWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( IfcPlusPlusSystem* sys, QWidget *parent = 0 );
	~MainWindow();

	TabReadWrite* getTabReadWrite() { return m_tab_read_write; }

protected:
	void closeEvent( QCloseEvent *event );

private:
	IfcPlusPlusSystem* m_system;
	ViewerWidget*	m_viewer_widget;
	QTabWidget*		m_tabwidget;
	QLabel*			m_label_status_cursor;
	TabReadWrite*	m_tab_read_write;
	QSplitter*		m_splitter;

	QToolBar*		m_file_toolbar;
	QToolBar*		m_edit_toolbar;
	QProgressBar*	m_progress_bar;
	double			m_current_progress_value = 0;

	void createTabWidget();

signals:
	void signalMainWindowClosed();

private slots:
	void slotBtnZoomBoundingsClicked();
	void slotBtnRemoveSelectedObjectsClicked();
	void slotProgressValue(double progress_value, const std::string& progress_type);
};
