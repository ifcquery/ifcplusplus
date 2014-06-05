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

#include "ifcpp/IFC4/include/IfcProfileDef.h"

#include "ProfileConverter.h"
#include "ProfileCache.h"


ProfileCache::ProfileCache( shared_ptr<GeometrySettings> geom_settings, shared_ptr<UnitConverter> uc )
	: m_geom_settings(geom_settings), m_unit_converter(uc)
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
	const int profile_id = ifc_profile->getId();
	
	std::map<int,shared_ptr<ProfileConverter> >::iterator it_profile_cache = m_profile_cache.find(profile_id);
	if( it_profile_cache != m_profile_cache.end() )
	{
		return it_profile_cache->second;
	}

	shared_ptr<ProfileConverter> profile_converter = shared_ptr<ProfileConverter>( new ProfileConverter( m_geom_settings, m_unit_converter ) );
	profile_converter->computeProfile( ifc_profile );

#ifdef IFCPP_OPENMP
	ScopedLock lock( m_writelock_profile_cache );
#endif
	m_profile_cache[profile_id] = profile_converter;
		
	return profile_converter;
}
