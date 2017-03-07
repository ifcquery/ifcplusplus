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
		m_min_crease_angle = M_PI*0.05;
		m_handle_styled_items = true;
		m_handle_layer_assignments = true;

		//m_min_colinearity = 0.1;
		//m_min_delta_v = 1.0;
		//m_min_normal_angle = M_PI / 180.0;
		//m_min_length = 0.0002;
		//m_min_face_area = 1e-8;
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

	double getMinCreaseAngle() { return m_min_crease_angle; }
	void setMinCreaseAngle( double min_crease )
	{
#ifdef IFCPP_OPENMP
		ScopedLock lock( m_mutex_geom_settings );
#endif
		m_min_crease_angle = min_crease;
	}

protected:
	int	m_num_vertices_per_circle;
	int m_num_vertices_per_circle_default;
	int m_min_num_vertices_per_arc;
	bool m_show_text_literals;
	bool m_ignore_profile_radius;
	bool m_handle_styled_items;
	bool m_handle_layer_assignments;
	
	/** if angle between two faces is smaller than m_min_crease_angle, the normals will be added and normalized.
	set to negative value to deactivate intermediate normals */
	double m_min_crease_angle;

	//double m_min_colinearity;
	//double m_min_delta_v;
	//double m_min_normal_angle;
	//double m_min_length;
	//double m_min_face_area;

#ifdef IFCPP_OPENMP
	Mutex m_mutex_geom_settings;
#endif
};
