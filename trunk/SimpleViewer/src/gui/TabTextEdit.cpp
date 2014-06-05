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

#include <QtCore/qglobal.h>
#include <QBoxLayout>
#include <QMessageBox>

#include <QtGui/QKeyEvent>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include "StepHighlighter.h"
#include "TabTextEdit.h"

TabTextEdit::TabTextEdit()
{
	m_le_path_schema_file = new QLineEdit();
	m_le_path_schema_file->setReadOnly( true );

	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);
	
	m_textedit = new QTextEdit();
	m_textedit->setFont(font);
	m_textedit->setTabStopWidth( 20 );
#ifndef _DEBUG
	StepHighlighter* highlighter = new StepHighlighter( m_textedit->document() );
#endif
	connect( m_textedit, SIGNAL( textChanged() ), this, SLOT( slotTextChanged() ) );

	m_btn_save = new QPushButton();
	m_btn_save->setIcon(  QIcon( ":Resources/img/save.png" ) );
	m_btn_save->setDisabled( true );
	m_btn_save->setContentsMargins( 2, 2, 2, 2 );
	connect( m_btn_save, SIGNAL( clicked() ), this, SLOT( slotSaveClicked() ) );

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget( m_le_path_schema_file, 1 );
	hbox->addWidget( m_btn_save, 0 );

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addLayout( hbox ); 
	vbox->addWidget( m_textedit, 0 );
	setLayout( vbox );
}

TabTextEdit::~TabTextEdit()
{
}

void TabTextEdit::setPathFile( QString path_file )
{
	m_path_file = path_file;
	m_le_path_schema_file->setText( m_path_file );
	m_textedit->blockSignals( true );

	QFile file( path_file );
	if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		m_textedit->setText( "couldn't open file " + path_file );
	}
	else
	{
		QTextStream file_stream( &file );
		QString file_content = file_stream.readAll();
		file.close();

		m_textedit->setText( file_content );
		
		if( !path_file.startsWith( ":" ) )
		{
			m_btn_save->setDisabled( false );
		}
	}
	emit( signalFileWidgetLoaded() );
	m_textedit->blockSignals( false );
}

void TabTextEdit::slotTextChanged()
{
	if( !m_path_file.startsWith( ":" ) )
	{
		m_btn_save->setDisabled( false );
	}
}

void TabTextEdit::slotSaveClicked()
{
	QFile file( m_path_file );
	if( !file.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text) )
	{
		QMessageBox msgBox;
		msgBox.setText( "couldn't open file " + m_path_file );
		msgBox.exec();
		return;
	}

	QString file_content = m_textedit->toPlainText();

	QTextStream file_stream( &file );
	file_stream << file_content;
	file.close();
	m_btn_save->setDisabled( true );
}

void TabTextEdit::keyPressEvent( QKeyEvent * event )
{
	if( event->modifiers() == Qt::CTRL )
	{
		if( event->key() == Qt::Key_S )
		{
			slotSaveClicked();
		}
	}
}
