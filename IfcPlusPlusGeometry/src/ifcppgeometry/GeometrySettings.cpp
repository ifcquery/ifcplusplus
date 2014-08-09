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

#define _USE_MATH_DEFINES 
#include <math.h>
#include "GeometrySettings.h"

GeometrySettings::GeometrySettings()
{
	m_num_vertices_per_circle_default = 10;
	m_num_vertices_per_circle = m_num_vertices_per_circle_default;
	m_min_num_vertices_per_arc = 6;
	m_show_text_literals = false;
	m_ignore_profile_radius = false;
	m_intermediate_normal_angle = M_PI*0.05;

	m_min_colinearity = 0.1;
	m_min_delta_v = 1.0;
	m_min_normal_angle = M_PI / 180.0;
	m_min_length = 0.0002;
}

GeometrySettings::~GeometrySettings()
{
}
