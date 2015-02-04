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

#include <vector>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <QTreeWidget>

class ViewerUtil
{
public:
	static QTreeWidgetItem* findItemByIfcId( QTreeWidgetItem* item, int ifc_id )
	{
		int num_children = item->childCount();
		for( int i = 0; i<num_children; ++i )
		{
			QTreeWidgetItem* child = item->child( i );
			int id = child->text( 1 ).toUInt();
			if( id == ifc_id )
			{
				return child;
			}
			QTreeWidgetItem* child_of_child = findItemByIfcId( child, ifc_id );
			if( child_of_child != 0 )
			{
				return child_of_child;
			}
		}
		return 0;
	}
};
