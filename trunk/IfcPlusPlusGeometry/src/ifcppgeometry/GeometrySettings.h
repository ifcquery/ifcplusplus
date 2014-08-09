/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
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
	GeometrySettings();
	~GeometrySettings();
	int	m_num_vertices_per_circle;
	int m_num_vertices_per_circle_default;
	int m_min_num_vertices_per_arc;
	bool m_show_text_literals;
	bool m_ignore_profile_radius;
	
	/** if angle between two faces is smaller than m_intermediate_normal_angle, the normals will be added and normalized.
	set to negative value to deactivate intermediate normals */
	double m_intermediate_normal_angle;

	double m_min_colinearity;
	double m_min_delta_v;
	double m_min_normal_angle;
	double m_min_length;
};
