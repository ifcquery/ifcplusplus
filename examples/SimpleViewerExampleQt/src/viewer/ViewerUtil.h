#pragma once

#include <vector>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Group>
#include <osgFX/Scribe>
#include <osgViewer/Viewer>

#include <ifcpp/model/BuildingObject.h>
#include <carve/mesh.hpp>

#include <QTreeWidget>

enum IntersectionPlane {	INTERSECTION_PLANE_XY,	INTERSECTION_PLANE_YZ,	INTERSECTION_PLANE_XZ	};
void intersectRayWithPlane( const osg::Vec3d& ray_origin, const osg::Vec3d& ray_direction, const IntersectionPlane& plane, osg::Vec3d& intersection_point );
QTreeWidgetItem* findItemByIfcId( QTreeWidgetItem* item, int ifc_id );

void applyMatrix( osg::Group* grp, osg::Matrix& mat );
std::string getGUID(const shared_ptr<BuildingEntity>& ent);

struct SelectedEntity
{
	shared_ptr<BuildingEntity>	m_entity;
	osg::ref_ptr<osg::Group>	m_osg_group;
	osg::ref_ptr<osg::Material> m_material_previous;
	osg::ref_ptr<osg::Material> m_material_selected;

	void setSelected(osg::Material* materialSelectedObjects);
	void setUnselected();
};
