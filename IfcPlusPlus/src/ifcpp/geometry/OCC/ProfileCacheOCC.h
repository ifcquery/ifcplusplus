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

#include <map>
#include <ifcpp/model/OpenMPIncludes.h>
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include "ProfileConverterOCC.h"
#include "CurveConverterOCC.h"
#include "SplineConverterOCC.h"

class ProfileCacheOCC : public StatusCallback
{
protected:
	shared_ptr<CurveConverterOCC>					m_curve_converter;
	shared_ptr<SplineConverterOCC>					m_spline_converter;
	std::map<int,shared_ptr<ProfileConverterOCC> >		m_profile_cache;

#ifdef ENABLE_OPENMP
	Mutex m_writelock_profile_cache;
#endif

public:
	ProfileCacheOCC( shared_ptr<CurveConverterOCC>& cc, shared_ptr<SplineConverterOCC>& sc )
		: m_curve_converter( cc ), m_spline_converter( sc )
	{
	}

	virtual ~ProfileCacheOCC(){}

	void clearProfileCache()
	{
		m_profile_cache.clear();
	}

	shared_ptr<ProfileConverterOCC> getProfileConverter( shared_ptr<IfcProfileDef>& ifc_profile )
	{
		if( !ifc_profile )
		{
			return shared_ptr<ProfileConverterOCC>();
		}
		const int profile_id = ifc_profile->m_tag;
		if( profile_id < 0 )
		{
			std::stringstream strs;
			strs << "Entity ID is invalid, type: " << ifc_profile->className();
			throw BuildingException( strs.str().c_str(), __FUNC__ );
		}

		std::map<int, shared_ptr<ProfileConverterOCC> >::iterator it_profile_cache = m_profile_cache.find( profile_id );
		if( it_profile_cache != m_profile_cache.end() )
		{
			return it_profile_cache->second;
		}

		shared_ptr<ProfileConverterOCC> profile_converter = shared_ptr<ProfileConverterOCC>( new ProfileConverterOCC( m_curve_converter, m_spline_converter ) );
		if( !profile_converter )
		{
			throw OutOfMemoryException( __FUNC__ );
		}
		profile_converter->computeProfile( ifc_profile );

#ifdef ENABLE_OPENMP
		ScopedLock lock( m_writelock_profile_cache );
#endif
		m_profile_cache[profile_id] = profile_converter;

		return profile_converter;
	}
};
