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

#if defined(HAVE_CONFIG_H)
#include <carve_config.h>
#endif

#include <carve/octree_decl.hpp>
#include <carve/octree_impl.hpp>

#include <carve/poly_decl.hpp>

namespace carve {
    namespace csg {

Octree::Node::Node(const carve::geom3d::Vector& newMin,
    const carve::geom3d::Vector& newMax)
    : parent(nullptr), is_leaf(true), min(newMin), max(newMax) {
    for (int i = 0; i < 8; ++i) {
        children[i] = nullptr;
    }
    aabb = Octree::makeAABB(this);
}

Octree::Node::Node(Node* p, double x1, double y1, double z1, double x2,
    double y2, double z2)
    : parent(p),
    is_leaf(true),
    min(carve::geom::VECTOR(x1, y1, z1)),
    max(carve::geom::VECTOR(x2, y2, z2)) {
    for (int i = 0; i < 8; ++i) {
        children[i] = nullptr;
    }
    aabb = Octree::makeAABB(this);
}

Octree::Node::~Node() {
    for (int i = 0; i < 8; ++i) {
        if (children[i] != nullptr) {
            (*children[i]).~Node();
        }
    }
    if (children[0] != nullptr) {
        char* ptr = (char*)children[0];
        delete[] ptr;
    }
}

bool Octree::Node::mightContain(const carve::poly::Face<3>& face, double eps)
{
    if (face.nVertices() == 3)
    {
        return aabb.intersects(carve::geom::tri<3>( face.vertex(0)->v, face.vertex(1)->v, face.vertex(2)->v), eps);
    }
    else
    {
        return aabb.intersects(face.aabb, eps) && aabb.intersects(face.plane_eqn, eps);
    }
}

bool Octree::Node::mightContain(const carve::poly::Edge<3>& edge, double eps)
{
    return aabb.intersectsLineSegment(edge.v1->v, edge.v2->v);
}

bool Octree::Node::mightContain(const carve::poly::Vertex<3>& p, double eps)
{
    return aabb.containsPoint(p.v);
}

bool Octree::Node::hasChildren() {
    return !is_leaf;
}

bool Octree::Node::split(double eps)
{
    if (is_leaf && hasGeometry()) {
        carve::geom3d::Vector mid = 0.5 * (min + max);
        char* ptr = new char[sizeof(Node) * 8];
        children[0] = new (ptr + sizeof(Node) * 0) Node(this, min.x, min.y, min.z, mid.x, mid.y, mid.z);
        children[1] = new (ptr + sizeof(Node) * 1) Node(this, mid.x, min.y, min.z, max.x, mid.y, mid.z);
        children[2] = new (ptr + sizeof(Node) * 2) Node(this, min.x, mid.y, min.z, mid.x, max.y, mid.z);
        children[3] = new (ptr + sizeof(Node) * 3) Node(this, mid.x, mid.y, min.z, max.x, max.y, mid.z);
        children[4] = new (ptr + sizeof(Node) * 4) Node(this, min.x, min.y, mid.z, mid.x, mid.y, max.z);
        children[5] = new (ptr + sizeof(Node) * 5) Node(this, mid.x, min.y, mid.z, max.x, mid.y, max.z);
        children[6] = new (ptr + sizeof(Node) * 6) Node(this, min.x, mid.y, mid.z, mid.x, max.y, max.z);
        children[7] = new (ptr + sizeof(Node) * 7) Node(this, mid.x, mid.y, mid.z, max.x, max.y, max.z);

        for (int i = 0; i < 8; ++i)
        {
            putInside(faces, children[i], children[i]->faces, eps);
            putInside(edges, children[i], children[i]->edges, eps);
            putInside(vertices, children[i], children[i]->vertices, eps);
        }

        faces.clear();
        edges.clear();
        vertices.clear();
        is_leaf = false;
    }
    return is_leaf;
}

template <class T>
void Octree::Node::putInside(const T& input, Node* child, T& output, double eps)
{
    for (typename T::const_iterator it = input.begin(), e = input.end(); it != e; ++it)
    {
        if (child->mightContain(**it, eps))
        {
            output.push_back(*it);
        }
    }
}

bool Octree::Node::hasGeometry() {
    return faces.size() > 0 || edges.size() > 0 || vertices.size() > 0;
}

Octree::Octree(double epsilon) : m_epsilon(epsilon)
{
    root = nullptr;
}

Octree::~Octree() {
    if (root) {
        delete root;
    }
}

void Octree::setBounds(const carve::geom3d::Vector& min,
    const carve::geom3d::Vector& max) {
    if (root) {
        delete root;
    }
    root = new Node(min, max);
}

void Octree::setBounds(carve::geom3d::AABB aabb) {
    if (root) {
        delete root;
    }
    aabb.extent = 1.1 * aabb.extent;
    root = new Node(aabb.minPoint(), aabb.maxPoint());
}

void Octree::addEdges(const std::vector<carve::poly::Edge<3> >& e) {
    root->edges.reserve(root->edges.size() + e.size());
    for (size_t i = 0; i < e.size(); ++i) {
        root->edges.push_back(&e[i]);
    }
}

void Octree::addFaces(const std::vector<carve::poly::Face<3> >& f) {
    root->faces.reserve(root->faces.size() + f.size());
    for (size_t i = 0; i < f.size(); ++i) {
        root->faces.push_back(&f[i]);
    }
}

void Octree::addVertices(const std::vector<const carve::poly::Vertex<3>*>& p) {
    root->vertices.insert(root->vertices.end(), p.begin(), p.end());
}

carve::geom3d::AABB Octree::makeAABB(const Node* node) {
    carve::geom3d::Vector centre = 0.5 * (node->min + node->max);
    carve::geom3d::Vector size = SLACK_FACTOR * 0.5 * (node->max - node->min);
    return carve::geom3d::AABB(centre, size);
}

void Octree::doFindEdges(const carve::geom::aabb<3>& aabb, Node* node, std::vector<const carve::poly::Edge<3>*>& out, unsigned int depth) const
{
    if (node == nullptr) {
        return;
    }

    if (node->aabb.intersects(aabb, m_epsilon)) {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindEdges(aabb, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH &&
                node->edges.size() > EDGE_SPLIT_THRESHOLD) {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindEdges(aabb, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Edge<3> *>::const_iterator
                it = node->edges.begin(),
                e = node->edges.end();
                it != e; ++it) {
                if ((*it)->tag_once()) {
                    out.push_back(*it);
                }
            }
        }
    }
}

void Octree::doFindEdges(const carve::geom3d::LineSegment& l, Node* node, std::vector<const carve::poly::Edge<3>*>& out, unsigned int depth) const
{
    if (node == nullptr) {
        return;
    }

    if (node->aabb.intersectsLineSegment(l.v1, l.v2)) {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindEdges(l, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH && node->edges.size() > EDGE_SPLIT_THRESHOLD)
            {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindEdges(l, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Edge<3> *>::const_iterator
                it = node->edges.begin(),
                e = node->edges.end();
                it != e; ++it) {
                if ((*it)->tag_once()) {
                    out.push_back(*it);
                }
            }
        }
    }
}

void Octree::doFindEdges(const carve::geom3d::Vector& v, Node* node, std::vector<const carve::poly::Edge<3>*>& out, unsigned int depth) const
{
    if (node == nullptr) {
        return;
    }

    if (node->aabb.containsPoint(v)) {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindEdges(v, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH && node->edges.size() > EDGE_SPLIT_THRESHOLD)
            {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindEdges(v, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Edge<3> *>::const_iterator it = node->edges.begin(), e = node->edges.end(); it != e; ++it)
            {
                if ((*it)->tag_once()) {
                    out.push_back(*it);
                }
            }
        }
    }
}

void Octree::doFindFaces(const carve::geom::aabb<3>& aabb, Node* node, std::vector<const carve::poly::Face<3>*>& out, unsigned int depth) const
{
    if (node == nullptr)
    {
        return;
    }

    if (node->aabb.intersects(aabb, m_epsilon))
    {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindFaces(aabb, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH && node->faces.size() > FACE_SPLIT_THRESHOLD)
            {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindFaces(aabb, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Face<3> *>::const_iterator it = node->faces.begin(), e = node->faces.end(); it != e; ++it)
            {
                if ((*it)->tag_once()) {
                    out.push_back(*it);
                }
            }
        }
    }
}

void Octree::doFindFaces(const carve::geom3d::LineSegment& l, Node* node, std::vector<const carve::poly::Face<3>*>& out, unsigned int depth) const
{
    if (node == nullptr) {
        return;
    }

    if (node->aabb.intersectsLineSegment(l.v1, l.v2)) {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindFaces(l, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH && node->faces.size() > FACE_SPLIT_THRESHOLD)
            {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindFaces(l, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Face<3> *>::const_iterator it = node->faces.begin(), e = node->faces.end(); it != e; ++it)
            {
                if ((*it)->tag_once()) {
                    out.push_back(*it);
                }
            }
        }
    }
}

void Octree::doFindVerticesAllowDupes(const carve::geom3d::Vector& v, Node* node, std::vector<const carve::poly::Vertex<3>*>& out, unsigned int depth) const
{
    if (node == nullptr) {
        return;
    }

    if (node->aabb.containsPoint(v)) {
        if (node->hasChildren()) {
            for (int i = 0; i < 8; ++i) {
                doFindVerticesAllowDupes(v, node->children[i], out, depth + 1);
            }
        }
        else {
            if (depth < MAX_SPLIT_DEPTH && node->vertices.size() > POINT_SPLIT_THRESHOLD)
            {
                if (!node->split(m_epsilon))
                {
                    for (int i = 0; i < 8; ++i) {
                        doFindVerticesAllowDupes(v, node->children[i], out, depth + 1);
                    }
                    return;
                }
            }
            for (std::vector<const carve::poly::Vertex<3> *>::const_iterator it = node->vertices.begin(), e = node->vertices.end(); it != e; ++it)
            {
                out.push_back(*it);
            }
        }
    }
}

void Octree::findEdgesNear(const carve::geom::aabb<3>& aabb, std::vector<const carve::poly::Edge<3>*>& out) const
{
    tagable::tag_begin();
    doFindEdges(aabb, root, out, 0);
}

void Octree::findEdgesNear(const carve::geom3d::LineSegment& l, std::vector<const carve::poly::Edge<3>*>& out) const
{
    tagable::tag_begin();
    doFindEdges(l, root, out, 0);
}

void Octree::findEdgesNear(const carve::poly::Edge<3>& e, std::vector<const carve::poly::Edge<3>*>& out) const
{
    tagable::tag_begin();
    doFindEdges(carve::geom3d::LineSegment(e.v1->v, e.v2->v), root, out, 0);
}

void Octree::findEdgesNear(
    const carve::geom3d::Vector& v,
    std::vector<const carve::poly::Edge<3>*>& out) const {
    tagable::tag_begin();
    doFindEdges(v, root, out, 0);
}

void Octree::findFacesNear(
    const carve::geom::aabb<3>& aabb,
    std::vector<const carve::poly::Face<3>*>& out) const {
    tagable::tag_begin();
    doFindFaces(aabb, root, out, 0);
}

void Octree::findFacesNear(
    const carve::geom3d::LineSegment& l,
    std::vector<const carve::poly::Face<3>*>& out) const {
    tagable::tag_begin();
    doFindFaces(l, root, out, 0);
}

void Octree::findFacesNear(
    const carve::poly::Edge<3>& e,
    std::vector<const carve::poly::Face<3>*>& out) const {
    tagable::tag_begin();
    doFindFaces(carve::geom3d::LineSegment(e.v1->v, e.v2->v), root, out, 0);
}

void Octree::findVerticesNearAllowDupes(
    const carve::geom3d::Vector& v,
    std::vector<const carve::poly::Vertex<3>*>& out) const {
    tagable::tag_begin();
    doFindVerticesAllowDupes(v, root, out, 0);
}

void Octree::doSplit(int maxSplit, Node* node, double eps) {
    // Don't split down any further than 4 levels.
    if (maxSplit <= 0 || (node->edges.size() < 5 && node->faces.size() < 5)) {
        return;
    }

    if (!node->split(eps))
    {
        for (int i = 0; i < 8; ++i) {
            doSplit(maxSplit - 1, node->children[i], eps);
        }
    }
}

void Octree::splitTree() {
    // initially split 4 levels
    doSplit(0, root, m_epsilon);
}
}  // namespace csg
}  // namespace carve
