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

#include <ifcpp/IFC4/include/IfcProfileDef.h>
#include <ifcpp/model/IfcPPException.h>

#include "ProfileConverter.h"
#include "PointConverter.h"
#include "ProfileCache.h"


ProfileCache::ProfileCache( shared_ptr<GeometrySettings>& geom_settings, shared_ptr<UnitConverter>& uc, shared_ptr<PointConverter>& pc, shared_ptr<SplineConverter>& sc )
	: m_geom_settings(geom_settings), m_unit_converter(uc), m_point_converter(pc), m_spline_converter(sc)
{
}

ProfileCache::~ProfileCache()
{
}

void ProfileCache::clearProfileCache()
{
	m_profile_cache.clear();
}

shared_ptr<ProfileConverter> ProfileCache::getProfileConverter( shared_ptr<IfcProfileDef>& ifc_profile )
{
	if( !ifc_profile )
	{
		return shared_ptr<ProfileConverter>();
	}
	const int profile_id = ifc_profile->m_id;
	if( profile_id < 0 )
	{
		std::stringstream strs;
		strs << "Entity ID is invalid, type: " << ifc_profile->classname();
		throw IfcPPException( strs.str().c_str(), __FUNC__ );
	}

	std::map<int,shared_ptr<ProfileConverter> >::iterator it_profile_cache = m_profile_cache.find(profile_id);
	if( it_profile_cache != m_profile_cache.end() )
	{
		return it_profile_cache->second;
	}

	shared_ptr<ProfileConverter> profile_converter = shared_ptr<ProfileConverter>( new ProfileConverter( m_geom_settings, m_unit_converter, m_point_converter, m_spline_converter ) );
	if( !profile_converter )
	{
		throw IfcPPException( "out of memory", __FUNC__ );
	}
	profile_converter->computeProfile( ifc_profile );

#ifdef IFCPP_OPENMP
	ScopedLock lock( m_writelock_profile_cache );
#endif
	m_profile_cache[profile_id] = profile_converter;
		
	return profile_converter;
}
