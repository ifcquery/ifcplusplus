// Copyright 2006-2015 Tobias Sargeant (tobias.sargeant@gmail.com).
//
// This file is part of the Carve CSG Library (http://carve-csg.com/)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma once

#include <carve/carve.hpp>

#include <carve/geom3d.hpp>

#include <carve/polyhedron_base.hpp>
#include <carve/octree_decl.hpp>
#include <carve/collection_types.hpp>

#include <assert.h>
#include <list>


namespace carve {
  namespace mesh {
    template<unsigned ndim>
    class MeshSet;
  }

  namespace poly {
    class Polyhedron;
  }

  poly::Polyhedron *polyhedronFromMesh(const mesh::MeshSet<3> *, int);

  namespace poly {

    class Polyhedron : public Geometry<3> {
    private:
      friend Polyhedron *carve::polyhedronFromMesh(const mesh::MeshSet<3> *, int);

      Polyhedron() {
      }

      Polyhedron &operator=(const Polyhedron &); // not implemented

      // *** initialization

      bool initSpatialIndex();
      void initVertexConnectivity();
      void setFaceAndVertexOwner();

      bool initConnectivity();
      bool markManifolds();
      bool calcManifoldEmbedding();

      bool init();
      void faceRecalc();

      void commonFaceInit(bool _recalc);

    public:
      static void collectFaceVertices(std::vector<face_t > &faces,
                                      std::vector<vertex_t > &vertices,
                                      std::unordered_map<const vertex_t *, const vertex_t *> &vmap);

      static void collectFaceVertices(std::vector<face_t > &faces,
                                      std::vector<vertex_t > &vertices);

      std::vector<bool> manifold_is_closed;
      std::vector<bool> manifold_is_negative;

      carve::geom3d::AABB aabb;
      carve::csg::Octree octree;



      // *** construction of Polyhedron objects

      Polyhedron(const Polyhedron &);

      // copy a single manifold
      Polyhedron(const Polyhedron &, int m_id);

      // copy a subset of manifolds
      Polyhedron(const Polyhedron &, const std::vector<bool> &selected_manifolds);

      Polyhedron(std::vector<face_t > &_faces,
                 std::vector<vertex_t > &_vertices,
                 bool _recalc = false);

      Polyhedron(std::vector<face_t > &_faces,
                 bool _recalc = false);

      Polyhedron(std::list<face_t > &_faces,
                 bool _recalc = false);

      Polyhedron(const std::vector<carve::geom3d::Vector> &vertices,
                 int n_faces,
                 const std::vector<int> &face_indices);

      ~Polyhedron();



      // *** containment queries

      void testVertexAgainstClosedManifolds(const carve::geom3d::Vector &v,
                                            std::map<int, PointClass> &result,
                                            bool ignore_orentation) const;

      PointClass containsVertex(const carve::geom3d::Vector &v,
                                const face_t **hit_face = NULL,
                                bool even_odd = false,
                                int manifold_id = -1) const;



      // *** locality queries

      void findEdgesNear(const carve::geom::aabb<3> &aabb, std::vector<const edge_t *> &edges) const;
      void findEdgesNear(const carve::geom3d::LineSegment &l, std::vector<const edge_t *> &edges) const;
      void findEdgesNear(const carve::geom3d::Vector &v, std::vector<const edge_t *> &edges) const;
      void findEdgesNear(const face_t &face, std::vector<const edge_t *> &edges) const;
      void findEdgesNear(const edge_t &edge, std::vector<const edge_t *> &edges) const;

      void findFacesNear(const carve::geom::aabb<3> &aabb, std::vector<const face_t *> &faces) const;
      void findFacesNear(const carve::geom3d::LineSegment &l, std::vector<const face_t *> &faces) const;
      void findFacesNear(const edge_t &edge, std::vector<const face_t *> &faces) const;



      // *** manifold queries

      inline bool vertexOnManifold(const vertex_t *v, int m_id) const;
      inline bool edgeOnManifold(const edge_t *e, int m_id) const;

      template<typename T>
      int vertexManifolds(const vertex_t *v, T result) const;

      template<typename T>
      int edgeManifolds(const edge_t *e, T result) const;

      size_t manifoldCount() const;

      bool hasOpenManifolds() const;




      // *** transformation

      // flip face directions
      void invertAll();
      void invert(const std::vector<bool> &selected_manifolds);

      void invert(int m_id);
      void invert();

      // matrix transform of vertices
      void transform(const carve::math::Matrix &xform);

      // arbitrary function transform of vertices
      template<typename T>
      void transform(const T &xform);

      void print(std::ostream &) const;

      void canonicalize();
    };

    std::ostream &operator<<(std::ostream &, const Polyhedron &);

  }

}
