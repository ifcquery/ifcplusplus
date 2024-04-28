/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2024 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <iomanip>
#include <regex>
#include <QApplication>
#include <osg/BoundsChecking>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/PolygonStipple>
#include <osg/LineStipple>
#include <osg/MatrixTransform>
#include <osg/Hint>
#include <osgViewer/View>
#include <osgFx/Scribe>
#include <osgText/Text>

#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/UnitConverter.h>
#include <IfcRelationship.h>
#include <IfcProduct.h>
#include <ifcpp/geometry/GeometryConverter.h>
#include <ifcpp/geometry/ConverterOSG.h>

#include "IfcPlusPlusSystem.h"
#include "Orbit3DManipulator.h"
#include "ViewerUtil.h"
#include "ViewController.h"
#include "IntersectionHandler.h"

IntersectionHandler::IntersectionHandler(IfcPlusPlusSystem* s ) : m_system( s )
{

}

IntersectionHandler::~IntersectionHandler()
{
}

void IntersectionHandler::intersectGroup( osg::Group* group, const carve::geom::vector<3> ray_point, const carve::geom::vector<3> ray_direction, double& smallest_d, osg::Vec3f& closest_point )
{
	for( size_t ii = 0; ii < group->getNumChildren(); ++ii )
	{
		osg::Node* node = group->getChild( ii );

		const osg::BoundingSphere& bs = node->computeBound();
		const carve::geom::vector<3> bs_center( carve::geom::VECTOR( bs.center().x(), bs.center().y(), bs.center().z() ) );
		double d = GeomUtils::distancePoint2LineUnitDirection( bs_center, ray_point, ray_direction );

		if( d < bs.radius() )
		{
			// node is close to pointer ray
			osg::Group* child_group = dynamic_cast<osg::Group*>( node );
			if( child_group )
			{
				intersectGroup( child_group, ray_point, ray_direction, smallest_d, closest_point );
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
						const osg::BoundingSphere& bs_geometry = geometry->computeBound();
						const carve::geom::vector<3> bs_center( carve::geom::VECTOR( bs_geometry.center().x(), bs_geometry.center().y(), bs_geometry.center().z() ) );

						//double d = GeomUtils::distancePoint2Line( const carve::geom::vector<3>& point, const carve::geom::vector<3>& line_p0, const carve::geom::vector<3>& line_p1 );
						double d = GeomUtils::distancePoint2LineUnitDirection( bs_center, ray_point, ray_direction );

						if( d < bs_geometry.radius() )
						{
							osg::Vec3Array* vertex_array = dynamic_cast<osg::Vec3Array*>( geometry->getVertexArray() );
							if( vertex_array )
							{
								osg::Vec3Array& vertex_array_ref = *vertex_array;
								for( size_t i_vertex = 0; i_vertex < vertex_array->size(); ++i_vertex )
								{
									osg::Vec3& vec3_osg = vertex_array_ref[i_vertex];
									const carve::geom::vector<3> vec3( carve::geom::VECTOR( vec3_osg.x(), vec3_osg.y(), vec3_osg.z() ) );
									double d = GeomUtils::distancePoint2LineUnitDirection( vec3, ray_point, ray_direction );
									if( d < smallest_d && d < 0.5 )
									{
										smallest_d = d;
										closest_point = vertex_array_ref[i_vertex];
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

bool IntersectionHandler::intersectSceneSimple( const osgGA::GUIEventAdapter& ea, osgViewer::View* view, osg::Vec3d& intersect_point )
{
#ifdef POLYTOPE_INTERSECTOR
	double mx = ea.getXnormalized();
	double my = ea.getYnormalized();
	double w = 0.005;
	double h = 0.005;
	osg::ref_ptr<osgUtil::PolytopeIntersector> picker = new osgUtil::PolytopeIntersector( osgUtil::Intersector::PROJECTION, mx - w, my - h, mx + w, my + h );
#else
	osg::ref_ptr<osgUtil::LineSegmentIntersector> picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized() );
#endif
	//picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );
	//picker->setIntersectionLimit( osgUtil::Intersector::LIMIT_NEAREST );

	osgUtil::IntersectionVisitor iv( picker.get() );
	osg::Camera* cam = view->getCamera();
	cam->accept( iv );

	if( picker->containsIntersections() )
	{
#ifdef POLYTOPE_INTERSECTOR
		osgUtil::PolytopeIntersector::Intersection intersection = picker->getFirstIntersection();
#else
		osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
#endif

		osg::NodePath& nodePath = intersection.nodePath;
		for( unsigned int i = 0; i<nodePath.size(); ++i )
		{
			osg::Node* node = nodePath[nodePath.size() - i - 1];
			if( !node )
			{
				continue;
			}
			const std::string node_name = node->getName();

			// check if picked object is a representation of an IfcProduct
			if( node_name.length() == 0 ) continue;
			if( node_name.substr( 0, 9 ).compare( "intersect" ) == 0 ) continue;

			intersect_point = intersection.getWorldIntersectPoint();
			return true;
		}
	}

	return false;
}

enum selectEraseMode { SELECT_ERASE_NONE, SELECT_ERASE_TRIANGLE, SELECT_ERASE_PRIMITIVE_SET };

bool IntersectionHandler::intersectSceneSelect( const osgGA::GUIEventAdapter& ea, osgViewer::View* view )
{
	//m_group_selected->removeChildren( 0, m_group_selected->getNumChildren() );
	bool only_first_intesection = false;
	double closest_intersection_distance = DBL_MAX;
	selectEraseMode select_erase_mode = SELECT_ERASE_NONE;
	// selectEraseMode select_erase_mode = SELECT_ERASE_PRIMITIVE_SET;
	// TODO: select object/primitive set/mesh/triangle/face (=all connected planar triangles) button in GUI
	// TODO: rubberband area selection

#ifdef POLYTOPE_INTERSECTOR
	double mx = ea.getXnormalized();
	double my = ea.getYnormalized();
	double w = 0.005;
	double h = 0.005;
	osg::ref_ptr<osgUtil::PolytopeIntersector> picker = new osgUtil::PolytopeIntersector( osgUtil::Intersector::PROJECTION, mx - w, my - h, mx + w, my + h );
#else
	osg::ref_ptr<osgUtil::LineSegmentIntersector> picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized() );
#endif
	osgUtil::IntersectionVisitor iv( picker.get() );
	osg::Camera* cam = view->getCamera();
	iv.apply( *cam );

	osg::Vec3d eye_point = m_system->getViewController()->getOrbitManipulator3D()->getEye();
	bool intersection_geometry_found = false;
	osgUtil::LineSegmentIntersector::Intersection closest_intersection;
	int closest_intersection_nodepath_idx = 0;

	if (picker->containsIntersections())
	{
#ifdef POLYTOPE_INTERSECTOR
		osgUtil::PolytopeIntersector::Intersections& intersections = picker->getIntersections();
#else
		osgUtil::LineSegmentIntersector::Intersections& intersections = picker->getIntersections();
#endif

		for (auto it_intersections = intersections.begin(); it_intersections != intersections.end(); ++it_intersections)
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = *it_intersections;
			osg::NodePath& nodePath = intersection.nodePath;
			for (unsigned int i_nodepath = 0; i_nodepath < nodePath.size(); ++i_nodepath)
			{
				int node_path_idx = nodePath.size() - i_nodepath - 1;
				osg::Node* node = nodePath[node_path_idx];
				std::string node_name = node->getName();

				// check if picked object is IfcProduct
				if (node_name.length() < 22) continue;

				node_name = node_name.substr(0, 22);

				std::string guid;
				std::regex re("[a-zA-Z0-9_$]{22}");
				std::smatch match;
				if (std::regex_search(node_name, match, re))
				{
					guid = match.str(0);
				}
				else
				{
					continue;
				}

				if (node_name.compare(0, 9, "intersect") == 0) continue;

				osg::Group* group = dynamic_cast<osg::Group*>(node);
				if (!group)
				{
					continue;
				}

				osg::Vec3d world_intersect_point = intersection.getWorldIntersectPoint();
				m_pointer_intersection.set(world_intersect_point);
				double distance_to_eye = (eye_point - world_intersect_point).length();
				if (distance_to_eye < closest_intersection_distance)
				{
					intersection_geometry_found = true;
					closest_intersection_distance = distance_to_eye;
					closest_intersection = intersection;
					closest_intersection_nodepath_idx = node_path_idx;
				}

				continue;
			}
		}

		if( intersection_geometry_found )
		{
			osg::NodePath& nodePath = closest_intersection.nodePath;
			if (closest_intersection_nodepath_idx < nodePath.size())
			{
				osg::Node* node = nodePath[closest_intersection_nodepath_idx];
				std::string node_name = node->getName();

				// extract guid
				if (node_name.size() >= 22)
				{
					node_name = node_name.substr(0, 22);

					std::string guid_selected;
					std::regex re("[a-zA-Z0-9_$]{22}");
					std::smatch match;
					if (std::regex_search(node_name, match, re))
					{
						guid_selected = match.str(0);
					}

					osg::Vec3d world_intersect_point = closest_intersection.getWorldIntersectPoint();
					m_pointer_intersection.set(world_intersect_point);

					osg::Group* group = dynamic_cast<osg::Group*>(node);
					if (group)
					{
						const std::unordered_map<std::string, shared_ptr<SelectedEntity> >& map_selected = m_system->getSelectedObjects();
						auto it_selected = map_selected.find(guid_selected);

						if (it_selected != map_selected.end())
						{
							shared_ptr<SelectedEntity> selected_entity = it_selected->second;
							// is already selected, so deselect
							m_system->setObjectSelected(selected_entity->m_entity, false, selected_entity->m_osg_group);
							return true;
						}
						else
						{
							// select
							shared_ptr<BuildingModel> ifc_model = m_system->getGeometryConverter()->getBuildingModel();
							const std::unordered_map<int, shared_ptr<BuildingEntity> >& map_ifc_objects = ifc_model->getMapIfcEntities();
							for (auto it_find : map_ifc_objects)
							{
								shared_ptr<BuildingEntity> entity = it_find.second;
								std::string guid = getGUID(entity);
								if (guid.compare(guid_selected) == 0)
								{
									shared_ptr<BuildingEntity> entitiy_selected = entity;
									if (!m_system->isCtrlKeyDown())
									{
										m_system->clearSelection();
									}
									m_system->setObjectSelected(entitiy_selected, true, group);
									break;
								}
							}
							return true;
						}
					}
				}
			}
		}
	}
	else	
	{
		// no geometry has been hit, so intersect with XY-plane
		//intersectRayWithPlane( m_ray_pointer_start, m_ray_pointer_direction, IntersectionPlane::XY_PLANE, m_pointer_intersection );
	}

	return intersection_geometry_found;
}
