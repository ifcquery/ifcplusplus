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

#include <osgViewer/Viewer>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/geometry/IncludeCarveHeaders.h>
#include <ifcpp/geometry/GeomUtils.h>

class IfcPlusPlusSystem;

class IntersectionHandler
{
public:
	IntersectionHandler(IfcPlusPlusSystem* s );
	~IntersectionHandler();

	void intersectGroup( osg::Group* group, const carve::geom::vector<3> ray_point, const carve::geom::vector<3> ray_direction, double& smallest_d, osg::Vec3f& closest_point );
	bool intersectSceneSelect( const osgGA::GUIEventAdapter& ea, osgViewer::View* view );
	bool intersectSceneSimple( const osgGA::GUIEventAdapter& ea, osgViewer::View* view, osg::Vec3d& intersect_point );

	osg::ref_ptr<osg::Group>			m_group_selected;
	osg::Vec3d							m_pointer_intersection;
	osg::Vec3f							m_snapped_point;
	bool								m_has_snapped_point;
	IfcPlusPlusSystem*					m_system;
};
