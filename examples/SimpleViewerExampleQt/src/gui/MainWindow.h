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

#include <ifcpp/model/BuildingObject.h>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QSplitter>
#include <QToolBar>
#include <osg/Group>

class IfcPlusPlusSystem;
class SettingsWidget;
class OpenFileWidget;
class ViewerWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( IfcPlusPlusSystem* sys, QWidget *parent = 0 );
	~MainWindow();

	OpenFileWidget* getOpenFileWidget() { return m_openFileWidget; }
	void updateOpenFileWidget();

protected:
	void closeEvent( QCloseEvent *event );
	void resizeEvent(QResizeEvent* event);
	void showEvent(QShowEvent* e);
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

private:
	IfcPlusPlusSystem* m_system;
	ViewerWidget*	m_viewerWidget;
	SettingsWidget*	m_settingsWidget;
	OpenFileWidget* m_openFileWidget;
	QPushButton*	m_buttonToggleOpenFileWidget;
	QLabel*			m_labelAbout;
	QLabel*			m_labelStatusCursor;
	QToolBar*		m_editToolbar;
	QProgressBar*	m_progressBar;
	double			m_currentProgressValue = 0;
	bool			m_autoHideFileWidget = true;

signals:
	void signalMainWindowClosed();

private slots:
	void slotBtnZoomBoundingsClicked();
	void slotSettingsClicked();
	void slotBtnRemoveSelectedObjectsClicked();
	void slotProgressValue(double progress_value, const std::string& progress_type);
	void slotClearSignalQueue();
	void slotZoomToObject(shared_ptr<BuildingEntity> object, osg::Group* node = 0);
	void slotToggleOpenFileWidget();
	void slotFileLoadingDone();
	void slotCursorCoordinates(double x, double y, double z);
};
