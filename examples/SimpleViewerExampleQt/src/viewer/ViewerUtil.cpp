#include <osg/AnimationPath>
#include <osgGA/AnimationPathManipulator>
#include <osgUtil/DelaunayTriangulator>
#include <ifcpp/IFC4X3/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4X3/include/IfcRoot.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "ViewerWidget.h"
#include "ViewerUtil.h"

void SelectedEntity::setSelected(osg::Material* materialSelectedObjects)
{
	if (m_osg_group)
	{
		osg::ref_ptr<osg::StateSet> stateset = m_osg_group->getOrCreateStateSet();
		osg::Material* material_previous = (osg::Material*)stateset->getAttribute(osg::StateAttribute::MATERIAL);
		if (material_previous)
		{
			m_material_previous = material_previous;
		}

		stateset->setAttribute(materialSelectedObjects, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		m_material_selected = materialSelectedObjects;
	}
}

void SelectedEntity::setUnselected()
{
	if (m_osg_group)
	{
		osg::ref_ptr<osg::StateSet> stateset_selected_node = m_osg_group->getOrCreateStateSet();
		if (m_material_previous)
		{
			stateset_selected_node->setAttribute(m_material_previous, osg::StateAttribute::ON);
		}
		else if (m_material_selected)
		{
			stateset_selected_node->removeAttribute(m_material_selected);
		}
	}
}

void intersectRayWithPlane( const osg::Vec3d& ray_origin, const osg::Vec3d& ray_direction, const IntersectionPlane& plane, osg::Vec3d& intersection_point )
{
	double n = 0;
	if( plane == IntersectionPlane::INTERSECTION_PLANE_XY )
	{
		// intersect mouse ray with x-y-plane
		// g = vec1 + n*(vec2-vec1),  z = 0
		// 0 = vec1.z + n*(vec2.z - vec1.z)
		n = -ray_origin.z() / (ray_direction.z());
	}
	else if( plane == IntersectionPlane::INTERSECTION_PLANE_XZ )
	{
		n = -ray_origin.y() / (ray_direction.y());
	}

	intersection_point.set( ray_origin.x() + n * (ray_direction.x()),  ray_origin.y() + n * (ray_direction.y()), ray_origin.z() + n * (ray_direction.z() ) );
}

std::string getGUID(const shared_ptr<BuildingEntity>& ent)
{
	std::string guid;
	shared_ptr<IFC4X3::IfcRoot> ifc_root = dynamic_pointer_cast<IFC4X3::IfcRoot>(ent);
	if (ifc_root)
	{
		if (ifc_root->m_GlobalId)
		{
			guid = ifc_root->m_GlobalId->m_value;
		}
	}
	return guid;
}

QTreeWidgetItem* findItemByIfcId( QTreeWidgetItem* item, int ifc_id )
{
	int num_children = item->childCount();
	for( int i=0; i<num_children; ++i )
	{
		QTreeWidgetItem* child_item = item->child( i );
		if( child_item->columnCount() > 0 )
		{
			QString text = child_item->text( 1 );
			int id = text.toUInt();
			if( id == ifc_id )
			{
				return child_item;
			}
			QTreeWidgetItem* child_of_child = findItemByIfcId( child_item, ifc_id );
			if( child_of_child != 0 )
			{
				return child_of_child;
			}
		}
	}
	return 0;
}

void applyMatrix( osg::Group* grp, osg::Matrix& mat )
{
	for( size_t i = 0; i < grp->getNumChildren(); ++i )
	{
		osg::Node* node = grp->getChild( i );

		osg::Group* child_grp = dynamic_cast<osg::Group*>( node );
		if( child_grp )
		{
			applyMatrix( child_grp, mat );
			continue;
		}

		osg::Geode* child_geode = dynamic_cast<osg::Geode*>( node );
		if( child_geode )
		{
			for( size_t i_drawable = 0; i_drawable < child_geode->getNumDrawables(); ++i_drawable )
			{
				osg::Drawable* drawable = child_geode->getDrawable( i_drawable );
				osg::Geometry* geometry = dynamic_cast<osg::Geometry*>( drawable );
				if( geometry )
				{
					osg::Vec3Array* vertex_array = dynamic_cast<osg::Vec3Array*>( geometry->getVertexArray() );
					if( vertex_array )
					{

						//osg::ref_ptr<osgUtil::DelaunayTriangulator> dt = new osgUtil::DelaunayTriangulator( vertex_array );
						//dt->triangulate(); // Generate the triangles 
						//geometry->setVertexArray( vertex_array );
						//geometry->addPrimitiveSet( dt->getTriangles() );


						osg::Vec3Array& vertex_array_ref = *vertex_array;
						for( size_t i_vertex = 0; i_vertex < vertex_array->size(); ++i_vertex )
						{
							osg::Vec3& vec3 = vertex_array_ref[i_vertex];
							vec3 = mat*vec3;
						}
						vertex_array->dirty();
						child_geode->dirtyBound();
						geometry->dirtyDisplayList();
						geometry->dirtyBound();
					}
				}
			}
		}
	}
}

