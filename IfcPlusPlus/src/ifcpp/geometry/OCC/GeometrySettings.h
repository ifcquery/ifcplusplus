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

#define _USE_MATH_DEFINES 
#include <math.h>
#include <ifcpp/model/IfcPPOpenMP.h>

#define GEOM_TOLERANCE  0.0000001
#ifdef _DEBUG
	#define HALF_SPACE_BOX_SIZE 100
#else
	#define HALF_SPACE_BOX_SIZE 100
#endif

//#define ROUND_IFC_COORDINATES
#ifdef ROUND_IFC_COORDINATES
	#define ROUND_IFC_COORDINATES_UP 100000.0
	#define ROUND_IFC_COORDINATES_DOWN 0.00001
#endif

//\brief Central class to hold settings that influence geometry processing.

class GeometrySettings
{
public:
	GeometrySettings()
	{
		m_num_vertices_per_circle_default = 14;
		m_num_vertices_per_circle = m_num_vertices_per_circle_default;
		m_min_num_vertices_per_arc = 6;
		m_show_text_literals = false;
		m_ignore_profile_radius = false;
		//m_min_crease_angle = M_PI*0.05;
		m_handle_styled_items = true;
		m_handle_layer_assignments = true;
	}

	// Number of discretization points per circle
	int getNumVerticesPerCircle() { return m_num_vertices_per_circle; }
	void setNumVerticesPerCircle( int num )
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_num_vertices_per_circle = num;
	}
	void resetNumVerticesPerCircle()
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_num_vertices_per_circle = m_num_vertices_per_circle_default;
	}

	// Minimum number of discretization points per arc
	int getMinNumVerticesPerArc() { return m_min_num_vertices_per_arc; }
	void resetMinNumVerticesPerArc()
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_min_num_vertices_per_arc = 6;
	}

	void setMinNumVerticesPerArc( int num )
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_min_num_vertices_per_arc = num;
	}
	void setHandleLayerAssignments( bool handle ) { m_handle_layer_assignments = handle; }
	bool handleLayerAssignments() { return m_handle_layer_assignments; }
	
	void setHandleStyledItems( bool handle ) { m_handle_styled_items = handle; }
	bool handleStyledItems() { return m_handle_styled_items; }

	bool isShowTextLiterals() { return m_show_text_literals; }
	bool isIgnoreProfileRadius() { return m_ignore_profile_radius; }
	void setIgnoreProfileRadius( bool ignore_radius )
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_ignore_profile_radius = ignore_radius;
	}
protected:
	int	m_num_vertices_per_circle;
	int m_num_vertices_per_circle_default;
	int m_min_num_vertices_per_arc;
	bool m_show_text_literals;
	bool m_ignore_profile_radius;
	bool m_handle_styled_items;
	bool m_handle_layer_assignments;

#ifdef IFCPP_OPENMP
	Mutex m_mutex_geom_settings;
#endif
};
