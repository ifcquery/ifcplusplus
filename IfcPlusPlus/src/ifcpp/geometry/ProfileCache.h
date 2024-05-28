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
#include <ifcpp/model/BasicTypes.h>
#include <ifcpp/model/StatusCallback.h>
#include "ProfileConverter.h"
#include "CurveConverter.h"
#include "SplineConverter.h"

class ProfileCache : public StatusCallback
{
protected:
	shared_ptr<CurveConverter>					m_curve_converter;
	shared_ptr<SplineConverter>					m_spline_converter;
	std::map<int,shared_ptr<ProfileConverter> >	m_profile_cache;

	std::mutex m_writelock_profile_cache;

public:
	ProfileCache( shared_ptr<CurveConverter>& cc, shared_ptr<SplineConverter>& sc )
		: m_curve_converter( cc ), m_spline_converter( sc )
	{
	}

	virtual ~ProfileCache()
	{
	}

	void clearProfileCache()
	{
		m_profile_cache.clear();
	}

	shared_ptr<ProfileConverter> getProfileConverter( const shared_ptr<IfcProfileDef>& ifc_profile, bool simplifyPaths)
	{
		if( !ifc_profile )
		{
			return shared_ptr<ProfileConverter>();
		}
		const int profile_id = ifc_profile->m_tag;
		if( profile_id < 0 )
		{
			std::stringstream strs;
			strs << "Entity ID is invalid, type: " << ifc_profile->classID();
			throw BuildingException( strs.str().c_str(), __FUNC__ );
		}

		auto it_profile_cache = m_profile_cache.find( profile_id );
		if( it_profile_cache != m_profile_cache.end() )
		{
			if( it_profile_cache->second->m_simplifyPathsByDefault == simplifyPaths )
			{
				return it_profile_cache->second;
			}
		}

		double eps = m_curve_converter->getGeomSettings()->getEpsilonMergePoints();
		int numVerticesPerCircleDefault = m_curve_converter->getGeomSettings()->getNumVerticesPerCircle();
		int numVerticesPerCircle = numVerticesPerCircleDefault;
		shared_ptr<ProfileConverter> profile_converter = shared_ptr<ProfileConverter>(new ProfileConverter(m_curve_converter, m_spline_converter));
		profile_converter->m_simplifyPathsByDefault = simplifyPaths;
		profile_converter->computeProfile(ifc_profile);

		for (size_t retryCount = 0; retryCount < 5; ++retryCount)
		{
			bool selfintersectionFound = false;
			const std::vector<std::vector<vec2> >& coords = profile_converter->getCoordinates();

			if (coords.size() > 5000)
			{
				// too slow
				continue;
			}

			for (size_t ii = 0; ii < coords.size(); ++ii)
			{
				const std::vector<vec2>& loop = coords[ii];
				if (GeomUtils::isPolygonSelfIntersecting(loop, eps))
				{
#ifdef _DEBUG
					vec4 color(0.4, 0.6, 0.6, 1.0);
					GeomDebugDump::dumpLocalCoordinateSystem();
					GeomDebugDump::dumpPolyline(loop, color, 1.0, true, false);
#endif

					selfintersectionFound = true;
					break;
				}
			}

			// TODO: check if discretization changed number of points in polygon. If not, break
			// TODO: performance improvement: throw exception in earcut upon self intersection detection. catch and restart higher up

			if (selfintersectionFound)
			{
				numVerticesPerCircle += 10;
				m_curve_converter->getGeomSettings()->setNumVerticesPerCircle(numVerticesPerCircle);
				// retry with higher accuracy
				profile_converter = shared_ptr<ProfileConverter>(new ProfileConverter(m_curve_converter, m_spline_converter));
				profile_converter->computeProfile(ifc_profile);
			}
			else
			{
				break;
			}
		}
		m_curve_converter->getGeomSettings()->setNumVerticesPerCircle(numVerticesPerCircleDefault);

		std::lock_guard<std::mutex> lock(m_writelock_profile_cache);
		m_profile_cache[profile_id] = profile_converter;

		return profile_converter;
	}
};
