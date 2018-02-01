/* -*-c++-*- IFC++ www.ifcquery.com
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

#include <osg/Geode>
#include <osg/Geometry>
#include <Bnd_Box.hxx>

inline void drawBoundingBoxLines( const Bnd_Box& aabb, osg::Vec3Array* vertices, osg::Geometry* geom )
{
	if( aabb.IsVoid() )
	{
		return;
	}
	double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
	aabb.Get( xmin, ymin, zmin, xmax, ymax, zmax );
	double pos_x = (xmin + xmax)*0.5;
	double pos_y = (ymin + ymax)*0.5;
	double pos_z = (zmin + zmax)*0.5;
	double extent_x = (xmax - xmin)*0.5;
	double extent_y = (ymax - ymin)*0.5;
	double extent_z = (zmax - zmin)*0.5;

	const double dex = extent_x;
	const double dey = extent_y;
	const double dez = extent_z;

	const int vert_id_offset = vertices->size();
	vertices->push_back( osg::Vec3f( pos_x - dex, pos_y - dey, pos_z - dez ) );
	vertices->push_back( osg::Vec3f( pos_x + dex, pos_y - dey, pos_z - dez ) );
	vertices->push_back( osg::Vec3f( pos_x + dex, pos_y + dey, pos_z - dez ) );
	vertices->push_back( osg::Vec3f( pos_x - dex, pos_y + dey, pos_z - dez ) );

	vertices->push_back( osg::Vec3f( pos_x - dex, pos_y - dey, pos_z + dez ) );
	vertices->push_back( osg::Vec3f( pos_x + dex, pos_y - dey, pos_z + dez ) );
	vertices->push_back( osg::Vec3f( pos_x + dex, pos_y + dey, pos_z + dez ) );
	vertices->push_back( osg::Vec3f( pos_x - dex, pos_y + dey, pos_z + dez ) );

	osg::ref_ptr<osg::DrawElementsUInt> box_lines = new osg::DrawElementsUInt( GL_LINE_STRIP, 0 );
	box_lines->push_back( vert_id_offset + 0 );
	box_lines->push_back( vert_id_offset + 1 );
	box_lines->push_back( vert_id_offset + 2 );
	box_lines->push_back( vert_id_offset + 3 );
	box_lines->push_back( vert_id_offset + 0 );
	box_lines->push_back( vert_id_offset + 4 );
	box_lines->push_back( vert_id_offset + 5 );
	box_lines->push_back( vert_id_offset + 1 );
	box_lines->push_back( vert_id_offset + 5 );
	box_lines->push_back( vert_id_offset + 6 );
	box_lines->push_back( vert_id_offset + 2 );
	box_lines->push_back( vert_id_offset + 6 );
	box_lines->push_back( vert_id_offset + 7 );
	box_lines->push_back( vert_id_offset + 3 );
	box_lines->push_back( vert_id_offset + 7 );
	box_lines->push_back( vert_id_offset + 4 );
	geom->addPrimitiveSet( box_lines );
}
inline void drawBoundingBoxLines( const Bnd_Box& aabb, osg::Geode* geode )
{
	if( aabb.IsVoid() )
	{
		return;
	}
	osg::ref_ptr<osg::Geometry> geom_bbox = new osg::Geometry();
	geode->addDrawable( geom_bbox );
	osg::ref_ptr<osg::Vec3Array> vertices_bbox = new osg::Vec3Array();
	geom_bbox->setVertexArray( vertices_bbox );
	drawBoundingBoxLines( aabb, vertices_bbox, geom_bbox );
}
