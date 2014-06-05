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

#include "StepHighlighter.h"

StepHighlighter::StepHighlighter(QTextDocument *parent)
	 : QSyntaxHighlighter(parent)
 {
	HighlightingRule rule;

	{
		QTextCharFormat ifc_tag;
		ifc_tag.setForeground( QColor( 127, 0, 85 ) );
		//ifc_tag.setFontWeight(QFont::Bold);
		rule.pattern = QRegExp("\\#\\[0-9]+\\b*=");
		rule.format = ifc_tag;
		m_highlightingRules.append(rule);
	}
	{
		QTextCharFormat basic_types_format;
		basic_types_format.setForeground(Qt::blue);
		//m_keywordFormat.setFontWeight(QFont::Bold);
		QStringList basic_types_patterns;
		basic_types_patterns 
			<< "\\bBOOLEAN\\b" 
			<< "\\bINTEGER\\b"
			<< "\\bSTRING\\b"
			<< "\\bREAL\\b"
			<< "\\bNUMBER\\b"
			<< "\\bLOGICAL\\b"
			<< "\\bBINARY\\b"
			<< "\\bENUMERATION OF\\b";
						 
		foreach (const QString &pattern, basic_types_patterns) {
			rule.pattern = QRegExp(pattern);
			rule.format = basic_types_format;
			m_highlightingRules.append(rule);
		}
	}
	{
		QTextCharFormat type_entity_format;
		type_entity_format.setFontWeight(QFont::Bold);
		type_entity_format.setForeground(Qt::darkBlue);
		QStringList type_entity_patterns;
		type_entity_patterns << "\\bTYPE\\b" << "\\bEND_TYPE\\b" << "\\bENTITY\\b" << "\\bEND_ENTITY\\b";
		foreach (const QString &pattern, type_entity_patterns) {
			rule.pattern = QRegExp(pattern);
			rule.format = type_entity_format;
			m_highlightingRules.append(rule);
		}
	}

	{
		QTextCharFormat schema_format;
		schema_format.setFontWeight(QFont::Bold);
		schema_format.setForeground(Qt::darkBlue);
		QStringList schema_patterns;
		schema_patterns << "\\bSCHEMA\\s*([a-zA-Z0-9]+);" << "\\bEND_SCHEMA;";
		foreach (const QString &pattern, schema_patterns) {
			rule.pattern = QRegExp(pattern);
			rule.format = schema_format;
			m_highlightingRules.append(rule);
		}
	}

	QTextCharFormat format_gray;
	format_gray.setForeground(Qt::lightGray);
	
	{
		QString single_derive( "\\s*([a-zA-Z0-9_\\.\\\\]+)\\s*:\\s*([a-zA-Z0-9_\\-\\[\\]:,\\?<>\\.\\|\\s=\\(\\)\\*\\\\]+);\\n" );
		QRegExp rx_derive( "\\s*DERIVE\\s*\\n(" + single_derive + ")+" );
		rx_derive.setMinimal( true );
		rule.pattern = rx_derive;
		rule.format = format_gray;
		m_highlightingRules.append(rule);
	}

	{
		QString single_where( "\\s*([a-zA-Z0-9_]+)\\s*:\\s*([a-zA-Z0-9_\\-\\[\\]{}:,\\+\\/\\s=\\|\\.\\(\\)\\<\\>\\'\\*\\\\]+);\\n" );
		QRegExp rx_where( "\\s*WHERE\\s*\\n(" + single_where + ")+" );
		rx_where.setMinimal( true );
		rule.pattern = rx_where;
		rule.format = format_gray;
		m_highlightingRules.append(rule);
	}

	{
		QString single_inverse = "\\s*([a-zA-Z0-9_]+)\\s*:\\s*([a-zA-Z0-9_\\?\\[\\]:\\s=\\(\\)]+);\\n*";
		QRegExp rx_inverse( "\\s*INVERSE\\s*\\n(" + single_inverse + ")+" );
		rule.pattern = rx_inverse;
		rule.format = format_gray;
		m_highlightingRules.append(rule);	
	}

	{
		QString rx_unique_single( "\\s*([a-zA-Z0-9_]+)\\s*:\\s*([a-zA-Z0-9_\\s\\[\\]:,\\?]+);\\r?\\n*" );
		QRegExp rx_unique( "\\s*UNIQUE\\s*\\n(" + rx_unique_single + ")+" );
		rule.pattern = rx_unique;
		rule.format = format_gray;
		m_highlightingRules.append(rule);	
	}

	{
//		FUNCTION IfcPathHeadToTail
//	(APath : IfcPath)
//	: LOGICAL;
//	 LOCAL
//	   N : INTEGER := 0;
//	   P : LOGICAL := UNKNOWN;
//	 END_LOCAL;
//	   N := SIZEOF (APath.EdgeList);
//	 REPEAT i := 2 TO N;
//		P := P AND (APath.EdgeList[i-1].EdgeEnd :=:
//					APath.EdgeList[i].EdgeStart);
//	 END_REPEAT;
//	 RETURN (P);
//END_FUNCTION;

		//QRegExp rx_function( "\\bFUNCTION\\s*([.\\n\\s\\ba-zA-Z0-9_\\(\\):;=\\[\\]]*)END_FUNCTION;" );
		QRegExp rx_function( "\\bFUNCTION\\s*([a-zA-Z0-9_\\s\\t\\r\\n\\(\\)\\[\\]\\?:;\\.,]*)" );
		//rx_function.setMinimal( true );
		rule.pattern = rx_function;
		rule.format = format_gray;
		m_highlightingRules.append(rule);	
	}

	m_classFormat.setFontWeight(QFont::Bold);
	m_classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = m_classFormat;
	m_highlightingRules.append(rule);

	m_quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = m_quotationFormat;
	m_highlightingRules.append(rule);

	 //functionFormat.setFontItalic(true);
	 //functionFormat.setForeground(Qt::blue);
	 //rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	 //rule.format = functionFormat;
	 //highlightingRules.append(rule);

	m_singleLineCommentFormat.setForeground( QColor( 0, 128, 0 ) );
	//rule.pattern = QRegExp("//[^\n]*");
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = m_singleLineCommentFormat;
	m_highlightingRules.append(rule);

	m_multiLineCommentFormat.setForeground( QColor( 0, 128, 0 ) );
	m_commentStartExpression = QRegExp("\\(\\*");
	m_commentEndExpression = QRegExp("\\*\\)");


	m_functionFormat.setForeground( Qt::lightGray );
	m_functionStartExpression = QRegExp("\\bFUNCTION\\s");
	m_functionEndExpression = QRegExp("\\bEND_FUNCTION;");
	

 }

void StepHighlighter::highlightBlock(const QString &text)
 {
	foreach (const HighlightingRule &rule, m_highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	// multiline comments
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
	{
		startIndex = m_commentStartExpression.indexIn(text);
	}

	while (startIndex >= 0)
	{
		int endIndex = m_commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, m_multiLineCommentFormat);
		startIndex = m_commentStartExpression.indexIn(text, startIndex + commentLength);
	}

	// functions
	//setCurrentBlockState(0);

	//startIndex = 0;
	//if (previousBlockState() != 1)
	//{
	//	startIndex = m_functionStartExpression.indexIn(text);
	//}

	//while (startIndex >= 0)
	//{
	//	int endIndex = m_functionEndExpression.indexIn(text, startIndex);
	//	int commentLength;
	//	if (endIndex == -1)
	//	{
	//		setCurrentBlockState(1);
	//		commentLength = text.length() - startIndex;
	//	}
	//	else
	//	{
	//		commentLength = endIndex - startIndex + m_functionEndExpression.matchedLength();
	//	}
	//	setFormat(startIndex, commentLength, m_functionFormat);
	//	startIndex = m_functionStartExpression.indexIn(text, startIndex + commentLength);
	//}
 }
