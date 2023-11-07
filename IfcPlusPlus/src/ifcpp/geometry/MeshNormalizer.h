#pragma once
#include "GeometryInputData.h"

class CarveMeshNormalizer
{
private:
	double m_scale = 1.0;
	vec3 m_normalizeCenter;
	std::map<std::string, carve::mesh::MeshSet<3>* > m_normalizedMeshes;

public:
	bool m_disableNormalizeAll = false;
	bool m_normalizeCoordsInsteadOfEpsilon = false;

	CarveMeshNormalizer(bool normalizeCoordsInsteadOfEpsilon)
	{
		m_normalizeCoordsInsteadOfEpsilon = normalizeCoordsInsteadOfEpsilon;
	}

	CarveMeshNormalizer(const CarveMeshNormalizer& other)
	{
		this->m_disableNormalizeAll = other.m_disableNormalizeAll;
		this->m_scale = other.m_scale;
		this->m_normalizeCenter = other.m_normalizeCenter;
		//this->m_normalizedMeshes = other->m_normalizedMeshes;
		this->m_normalizeCoordsInsteadOfEpsilon = other.m_normalizeCoordsInsteadOfEpsilon;
	}

	CarveMeshNormalizer(const carve::geom::aabb<3>& bbox1, const carve::geom::aabb<3>& bbox2, bool normalizeCoordsInsteadOfEpsilon)
	{
		m_normalizeCoordsInsteadOfEpsilon = normalizeCoordsInsteadOfEpsilon;
		vec3 center1 = bbox1.pos;
		vec3 extents1 = bbox1.extent;

		if (bbox2.isEmpty())
		{
			return;
		}

		vec3 center2 = bbox2.pos;
		vec3 extents2 = bbox2.extent;

		double extent1 = std::max(extents1.x, std::max(extents1.y, extents1.z));
		double extent2 = std::max(extents2.x, std::max(extents2.y, extents2.z));
		double maxExtent = std::max(extent1, extent2);

		double scale = 1.0;
		if (maxExtent > EPS_M5)
		{
			scale = 1.0 / maxExtent;
		}
		m_scale = scale * 0.8;  // conversative approach: 80%

		vec3 center = (center1 + center2) * 0.5;
		m_normalizeCenter = center;

		if (std::abs(m_scale) > 0.5 && std::abs(m_scale) < 2.0)
		{
			// if scale is close to 1, scaling does not make a big difference. 
			// Negative effects like small inaccuracies after multiplication and time to multiply overweigh
			m_scale = 1.0;
		}
	}

	double getScale() const { return m_scale; }
	void setToZero()
	{
		m_scale = 1.0;
		m_normalizeCenter.setZero();
	}

	void normalizeMesh(shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::string tag, double eps)
	{
		if (m_disableNormalizeAll)
		{
			return;
		}
		double centerLength2 = m_normalizeCenter.length2();
		if (m_scale == 1.0 && centerLength2 < 2.0)
		{
			return;
		}

		if (tag.size() > 0)
		{
			if (m_normalizedMeshes.find(tag) != m_normalizedMeshes.end())
			{
				// already normalized
				return;
			}
		}

		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();

		for (size_t i = 0; i < num_vertices; ++i)
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vec3& point = vertex.v;

			if (m_scale != 1.0 && m_normalizeCoordsInsteadOfEpsilon)
			{
				point = (point - m_normalizeCenter) * m_scale;
			}
			else
			{
				point = (point - m_normalizeCenter);
			}
		}

		for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			mesh->recalc(eps);
		}

		m_normalizedMeshes.insert({ tag, meshset.get() });
	}

	void deNormalizeMesh(shared_ptr<carve::mesh::MeshSet<3> >& meshset, std::string tag, double eps)
	{
		if (m_disableNormalizeAll)
		{
			return;
		}
		double centerLength2 = m_normalizeCenter.length2();
		if (m_scale == 1.0 && centerLength2 < 2.0)
		{
			return;
		}

		if (tag.size() > 0)
		{
			auto it = m_normalizedMeshes.find(tag);
			if (it == m_normalizedMeshes.end())
			{
				// already denormalized, or not normalized before
				return;
			}
			m_normalizedMeshes.erase(it);
		}

		double unScaleFactor = (1.0 / m_scale);

		std::vector<carve::mesh::Vertex<3> >& vertex_storage = meshset->vertex_storage;
		const size_t num_vertices = vertex_storage.size();
		for (size_t i = 0; i < num_vertices; ++i)
		{
			carve::mesh::Vertex<3>& vertex = vertex_storage[i];
			vec3& point = vertex.v;
			if (m_scale != 1.0 && m_normalizeCoordsInsteadOfEpsilon)
			{
				point = point * unScaleFactor + m_normalizeCenter;
			}
			else
			{
				point = point + m_normalizeCenter;
			}
		}
		for (size_t kk = 0; kk < meshset->meshes.size(); ++kk)
		{
			carve::mesh::Mesh<3>* mesh = meshset->meshes[kk];
			mesh->recalc(eps);
		}
	}
};
