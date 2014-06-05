#include <osg/AnimationPath>
#include <osgGA/AnimationPathManipulator>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ViewerWidget.h"
#include "ViewerUtil.h"


void intersectRayWithPlane( const osg::Vec3d& ray_origin, const osg::Vec3d& ray_direction, const IntersectionPlane& plane, osg::Vec3d& intersection_point )
{
	double n = 0;
	if( plane == IntersectionPlane::XY_PLANE )
	{
		// intersect mouse ray with x-y-plane
		// g = vec1 + n*(vec2-vec1),  z = 0
		// 0 = vec1.z + n*(vec2.z - vec1.z)
		n = -ray_origin.z() / (ray_direction.z());
	}
	else if( plane == IntersectionPlane::XZ_PLANE )
	{
		n = -ray_origin.y() / (ray_direction.y());
	}

	intersection_point.set( ray_origin.x() + n * (ray_direction.x()),  ray_origin.y() + n * (ray_direction.y()), ray_origin.z() + n * (ray_direction.z() ) );
}

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
