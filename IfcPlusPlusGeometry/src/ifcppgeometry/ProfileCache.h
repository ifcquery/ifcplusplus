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

#include <map>
#include <ifcpp/model/IfcPPOpenMP.h>
#include <ifcpp/model/shared_ptr.h>
class GeometrySettings;
class UnitConverter;
class IfcProfileDef;
class ProfileConverter;

class ProfileCache
{
public:	
	ProfileCache( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> uc );
	~ProfileCache();

	shared_ptr<ProfileConverter> getProfileConverter( shared_ptr<IfcProfileDef>& ifc_profile );
	void clearProfileCache();

protected:
	shared_ptr<GeometrySettings>				m_geom_settings;
	shared_ptr<UnitConverter>					m_unit_converter;
	std::map<int,shared_ptr<ProfileConverter> >	m_profile_cache;

#ifdef IFCPP_OPENMP
	Mutex m_writelock_profile_cache;
#endif
};
