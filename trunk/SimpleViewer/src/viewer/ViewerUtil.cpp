#include <osg/AnimationPath>
#include <osgGA/AnimationPathManipulator>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ViewerWidget.h"
#include "ViewerUtil.h"

QTreeWidgetItem* findItemByIfcId( QTreeWidgetItem* item, int ifc_id )
{
	int num_children = item->childCount();
	for( int i=0; i<num_children; ++i )
	{
		QTreeWidgetItem* child = item->child( i );
		int id = child->text(1).toUInt();
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
