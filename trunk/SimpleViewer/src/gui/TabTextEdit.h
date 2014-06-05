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

#include <QtCore/qglobal.h>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

class IfcPlusPlus;

class TabTextEdit : public QWidget
{
	Q_OBJECT

public:
	TabTextEdit();
	~TabTextEdit();

	QString	getPathFile() { return m_path_file; }
	void	setPathFile( QString path_file );
	QTextEdit* getTextEdit() { return m_textedit; }

public slots:
	void slotSaveClicked();
	void slotTextChanged();

private:
	QLineEdit*	m_le_path_schema_file;
	QString		m_path_file;
	QTextEdit*	m_textedit;
	QPushButton* m_btn_save;

protected:
	void keyPressEvent( QKeyEvent * event );

signals:
	void signalFileWidgetLoaded();
};
