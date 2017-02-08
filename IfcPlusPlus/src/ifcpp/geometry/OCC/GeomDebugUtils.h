/* -*-c++-*- IfcPlusPlus - www.ifcquery.com  - Copyright (C) 2011 Fabian Gerold
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

#include <AIS_shape.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Builder.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <StlMesh_Mesh.hxx>
#include <StlTransfer.hxx>

#include <osgDB/fstream>
#include <osgDB/WriteFile>
#include <ifcpp/model/shared_ptr.h>
#include <ifcpp/model/IfcPPException.h>
#include "GeometryInputData.h"
#include "ConverterOSG.h"
#include "GeomUtils.h"
#include "GeometrySettings.h"
#include "PointConverter.h"

namespace GeomDebugUtils
{
#ifdef _DEBUG
	static double dump_y_pos_geom = 0;
	static bool save_occ_brep = false;
	static TopoDS_Compound dump_shape_compound;
	static std::vector<osg::ref_ptr<osg::Geode> > vec_dump_shapes;

	inline const char* getDumpFileName() { return "dump_mesh_debug.osg"; }
	inline void clearDebugDumpFile()
	{
		std::ofstream dump_ofstream( getDumpFileName(), std::ofstream::trunc );
		dump_ofstream.close();
	}
	inline void clearDumpShape()
	{
		dump_shape_compound.Nullify();
		dump_y_pos_geom = 0;
		vec_dump_shapes.clear();
	}
	inline void writeDumpFile()
	{
		if( save_occ_brep )
		{
			std::ofstream dump_ofstream( getDumpFileName(), std::ofstream::app );
			BRepTools::Write( dump_shape_compound, dump_ofstream );
			dump_ofstream.close();
		}

		osg::ref_ptr<osg::Group> root = new osg::Group();
		for( size_t ii = 0; ii < vec_dump_shapes.size(); ++ii )
		{
			root->addChild( vec_dump_shapes[ii] );
		}

		std::string file_name = getDumpFileName();
		osgDB::writeNodeFile( *root, file_name );
	}
	static void addShapeToDump( const TopoDS_Shape& shape, const vec4& color )
	{
		if( save_occ_brep )
		{
			BRep_Builder builder;
			TopoDS_Compound Comp;
			builder.MakeCompound( Comp );
			builder.Add( Comp, shape );

			if( !dump_shape_compound.IsNull() )
			{
				builder.Add( Comp, dump_shape_compound );
			}

			dump_shape_compound = Comp;
		}

		// create osg geometry with colors and save it
		osg::ref_ptr<osg::Geode> geode = new osg::Geode();
		ConverterOSG::RenderOptions render_options( osg::Vec4f( color.r(), color.g(), color.b(), color.a() ) );
		ConverterOSG::drawShape( shape, geode, render_options );
		vec_dump_shapes.push_back( geode );
	}

	inline void dumpShape( const TopoDS_Shape& shape, const vec4& color, bool append, bool move_dump_position )
	{
		if( !append )
		{
			clearDumpShape();
		}
		clearDebugDumpFile();

		if( move_dump_position )
		{
			Bnd_Box bbox;
			BRepBndLib::Add( shape, bbox );
			double xmin = 0, ymin = 0, zmin = 0, xmax = 0, ymax = 0, zmax = 0;
			bbox.Get( xmin, ymin, zmin, xmax, ymax, zmax );

			dump_y_pos_geom += (ymax - ymin)*1.2 + 1.0;
			if( ymax > dump_y_pos_geom )
			{
				dump_y_pos_geom = ymax*1.1;
			}
		}

		TopoDS_Shape shape_transformed = shape;
		GeomUtils::applyTranslationToShape( shape_transformed, vec3( 0, dump_y_pos_geom, 0 ) );

		addShapeToDump( shape_transformed, color );
		writeDumpFile();
	}
	
	inline void dumpPolyline( const std::vector<vec3>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		TopoDS_Wire wire;
		PointConverter::createWireFromPoints( vec_polyline, wire, true );
		dumpShape( wire, color, append, move_dump_position );
	}
	inline void dumpPolyline( const std::vector<gp_Pnt>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		std::vector<vec3> vec_points;
		for( size_t ii = 0; ii < vec_polyline.size(); ++ii )
		{
			const gp_Pnt& point = vec_polyline[ii];
			vec_points.push_back( vec3( point.X(), point.Y(), point.Z() ) );
		}
		dumpPolyline( vec_points, color, append, move_dump_position );
	}

	inline void dumpPolyline( const std::vector<vec2>& vec_polyline, const vec4& color, bool append, bool move_dump_position )
	{
		std::vector<vec3> vec_polyline_3d;
		for( size_t ii = 0; ii < vec_polyline.size(); ++ii )
		{
			vec_polyline_3d.push_back( vec3( vec_polyline[ii].X(), vec_polyline[ii].Y(), 0 ) );
		}
		dumpPolyline( vec_polyline_3d, color, append, move_dump_position );
	}
#endif
}
