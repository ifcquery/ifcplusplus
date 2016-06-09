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
#  include <carve_config.h>
#endif

#include <carve/input.hpp>
#include <gloop/model/stream.hpp>
#include <gloop/model/ply_format.hpp>
#include <gloop/model/obj_format.hpp>
#include <gloop/model/vtk_format.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <fstream>

#ifndef WIN32
#  include <stdint.h>
#endif



namespace {
  struct line : public gloop::stream::null_reader {
    carve::input::PolylineSetData *data;
    line(carve::input::PolylineSetData *_data) : data(_data) { }
    virtual void length(int /* len */) {
    }
    virtual void next() {
      data->beginPolyline();
    }
    virtual void end() {
    }
    carve::input::PolylineSetData::polyline_data_t &curr() const {
      return data->polylines.back();
    }
  };

  struct line_closed : public gloop::stream::reader<bool> {
    line *l;
    line_closed(line *_l) : l(_l) { }
    virtual void value(bool val) { l->curr().first = val; }
  };

  struct line_idx : public gloop::stream::reader<int> {
    line *l;
    line_idx(line *_l) : l(_l) { }
    virtual void length(size_t len) { if (l > 0) l->curr().second.reserve(len); }
    virtual void value(int val) { l->curr().second.push_back(val); }
  };

  template<typename container_t>
  struct vertex : public gloop::stream::null_reader {
    container_t &container;
    vertex(container_t &_container) : container(_container) {
    }
    virtual void next() {
      container.push_back(carve::geom3d::Vector());
    }
    virtual void length(int l) {
      if (l > 0) container.reserve(container.size() + l);
    }
    virtual void end() {
    }
    carve::geom3d::Vector &curr() const {
      return container.back();
    }
  };
  template<typename container_t>
  vertex<container_t> *vertex_inserter(container_t &container) { return new vertex<container_t>(container); }



  template<int idx, typename curr_t>
  struct vertex_component : public gloop::stream::reader<double> {
    const curr_t *i;
    vertex_component(const curr_t *_i) : i(_i) { }
    virtual void value(double val) {
      i->curr().v[idx] = val;
    }
  };
  template<int idx, typename curr_t>
  vertex_component<idx, curr_t> *vertex_component_inserter(const curr_t *i) { return new vertex_component<idx, curr_t>(i); }



  struct face : public gloop::stream::null_reader {
    carve::input::PolyhedronData *data;
    face(carve::input::PolyhedronData *_data) : data(_data) {
    }
    virtual void length(int l) {
      if (l > 0) data->reserveFaces(l, 3);
    }
  };

  struct face_idx : public gloop::stream::reader<int> {
    carve::input::PolyhedronData *data;
    mutable std::vector<int> vidx;

    face_idx(carve::input::PolyhedronData *_data) : data(_data), vidx() { }

    virtual void length(int l) { vidx.clear(); vidx.reserve(l); }
    virtual void value(int val) { vidx.push_back(val); }
    virtual void end() { data->addFace(vidx.begin(), vidx.end()); }
  };

  struct tristrip_idx : public gloop::stream::reader<int> {
    carve::input::PolyhedronData *data;
    mutable int a, b, c;
    mutable bool clk;

    tristrip_idx(carve::input::PolyhedronData *_data) : data(_data), a(-1), b(-1), c(-1), clk(true) { }

    virtual void value(int val) {
      a = b; b = c; c = val;
      if (a == -1 || b == -1 || c == -1) {
        clk = true;
      } else {
        if (clk) {
          data->addFace(a, b, c);
        } else {
          data->addFace(c, b, a);
        }
        clk = !clk;
      }
    }

    virtual void length(int len) {
      data->reserveFaces(len - 2, 3);
    }
  };

  struct begin_pointset : public gloop::stream::null_reader {
    gloop::stream::model_reader &sr;
    carve::input::Input &inputs;
    carve::input::PointSetData *data;

    begin_pointset(gloop::stream::model_reader &_sr, carve::input::Input &_inputs) : sr(_sr), inputs(_inputs) {
    }
    ~begin_pointset() {
    }
    virtual void begin() {
      data = new carve::input::PointSetData();
      vertex<std::vector<carve::geom3d::Vector> > *vi = vertex_inserter(data->points);
      sr.addReader("pointset.vertex", vi);
      sr.addReader("pointset.vertex.x", vertex_component_inserter<0>(vi));
      sr.addReader("pointset.vertex.y", vertex_component_inserter<1>(vi));
      sr.addReader("pointset.vertex.z", vertex_component_inserter<2>(vi));
    }
    virtual void end() {
      inputs.addDataBlock(data);
    }
    virtual void fail() {
      delete data;
    }
  };

  struct begin_polyline : public gloop::stream::null_reader {
    gloop::stream::model_reader &sr;
    carve::input::Input &inputs;
    carve::input::PolylineSetData *data;

    begin_polyline(gloop::stream::model_reader &_sr, carve::input::Input &_inputs) : sr(_sr), inputs(_inputs) {
    }
    ~begin_polyline() {
    }
    virtual void begin() {
      data = new carve::input::PolylineSetData();
      vertex<std::vector<carve::geom3d::Vector> > *vi = vertex_inserter(data->points);
      sr.addReader("polyline.vertex", vi);
      sr.addReader("polyline.vertex.x", vertex_component_inserter<0>(vi));
      sr.addReader("polyline.vertex.y", vertex_component_inserter<1>(vi));
      sr.addReader("polyline.vertex.z", vertex_component_inserter<2>(vi));

      line *li = new line(data);
      sr.addReader("polyline.polyline", li);
      sr.addReader("polyline.polyline.closed", new line_closed(li));
      sr.addReader("polyline.polyline.vertex_indices", new line_idx(li));
    }
    virtual void end() {
      inputs.addDataBlock(data);
    }
    virtual void fail() {
      delete data;
    }
  };

  struct begin_polyhedron : public gloop::stream::null_reader {
    gloop::stream::model_reader &sr;
    carve::input::Input &inputs;
    carve::input::PolyhedronData *data;

    begin_polyhedron(gloop::stream::model_reader &_sr, carve::input::Input &_inputs) : sr(_sr), inputs(_inputs) {
    }
    ~begin_polyhedron() {
    }
    virtual void begin() {
      data = new carve::input::PolyhedronData();
      vertex<std::vector<carve::geom3d::Vector> > *vi = vertex_inserter(data->points);
      sr.addReader("polyhedron.vertex", vi);
      sr.addReader("polyhedron.vertex.x", vertex_component_inserter<0>(vi));
      sr.addReader("polyhedron.vertex.y", vertex_component_inserter<1>(vi));
      sr.addReader("polyhedron.vertex.z", vertex_component_inserter<2>(vi));

      sr.addReader("polyhedron.face", new face(data));
      sr.addReader("polyhedron.face.vertex_indices", new face_idx(data));

      sr.addReader("polyhedron.tristrips.vertex_indices", new tristrip_idx(data));
    }
    virtual void end() {
      inputs.addDataBlock(data);
    }
    virtual void fail() {
      delete data;
    }
  };
 
  void modelSetup(carve::input::Input &inputs, gloop::stream::model_reader &model) {
    model.addReader("polyhedron", new begin_polyhedron(model, inputs));
    model.addReader("polyline", new begin_polyline(model, inputs));
    model.addReader("pointset", new begin_pointset(model, inputs));
  }
}



template<typename filetype_t>
bool readFile(
    std::istream &in,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform) {
  filetype_t f;

  modelSetup(inputs, f);
  if (!f.read(in)) return false;

  inputs.transform(transform);
  return true;
}



template<typename filetype_t>
bool readFile(
    const std::string &in_file,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT()) {
  std::ifstream in(in_file.c_str(),std::ios_base::binary | std::ios_base::in);

  if (!in.is_open()) {
    std::cerr << "File '" <<  in_file << "' could not be opened." << std::endl;
    return false;
  }

  std::cerr << "Loading '" << in_file << "'" << std::endl;
  return readFile<filetype_t>(in, inputs, transform);
}



template<typename filetype_t>
bool readFile(
    std::istream &in,
    carve::poly::Polyhedron *&result,
    const carve::math::Matrix &transform) {
  carve::input::Input inputs;

  if (!readFile<filetype_t>(in, inputs, transform)) {
    return false;
  }

  for (std::list<carve::input::Data *>::const_iterator i = inputs.input.begin(); i != inputs.input.end(); ++i) {
    carve::poly::Polyhedron *poly = inputs.create<carve::poly::Polyhedron>(*i);
    if (poly) {
      result = poly;
      return true;
    }
  }

  return false;
}



template<typename filetype_t>
bool readFile(
    const std::string &in_file,
    carve::poly::Polyhedron *&result,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT()) {
  std::ifstream in(in_file.c_str(),std::ios_base::binary | std::ios_base::in);

  if (!in.is_open()) {
    std::cerr << "File '" <<  in_file << "' could not be opened." << std::endl;
    return false;
  }

  std::cerr << "Loading '" << in_file << "'" << std::endl;
  return readFile<filetype_t>(in, result, transform);
}



template<typename filetype_t>
bool readFile(
    std::istream &in,
    carve::mesh::MeshSet<3> *&result,
    const carve::math::Matrix &transform) {
  carve::input::Input inputs;
  result = NULL;
  if (!readFile<filetype_t>(in, inputs, transform)) {
    return false;
  }

  for (std::list<carve::input::Data *>::const_iterator i = inputs.input.begin(); i != inputs.input.end(); ++i) {
    carve::mesh::MeshSet<3> *poly = inputs.create<carve::mesh::MeshSet<3> >(*i);
    if (poly) {
      result = poly;
      return true;
    }
  }

  return false;
}



template<typename filetype_t>
bool readFile(
    const std::string &in_file,
    carve::mesh::MeshSet<3> *&result,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT()) {
  std::ifstream in(in_file.c_str(),std::ios_base::binary | std::ios_base::in);

  if (!in.is_open()) {
    std::cerr << "File '" <<  in_file << "' could not be opened." << std::endl;
    return false;
  }

  std::cerr << "Loading '" << in_file << "'" << std::endl;
  return readFile<filetype_t>(in, result, transform);
}



bool readPLY(
    std::istream &in,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::ply::PlyReader>(in, result, transform);
}

bool readPLY(
    const std::string &in_file,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::ply::PlyReader>(in_file, result, transform);
}

carve::poly::Polyhedron *readPLY(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::ply::PlyReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::poly::Polyhedron *readPLY(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::ply::PlyReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readPLYasMesh(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;

  if (!readFile<gloop::ply::PlyReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readPLYasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;

  if (!readFile<gloop::ply::PlyReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}



bool readOBJ(
    std::istream &in,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::obj::ObjReader>(in, result, transform);
}

bool readOBJ(
    const std::string &in_file,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::obj::ObjReader>(in_file, result, transform);
}

carve::poly::Polyhedron *readOBJ(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::obj::ObjReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::poly::Polyhedron *readOBJ(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::obj::ObjReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readOBJasMesh(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;
  if (!readFile<gloop::obj::ObjReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readOBJasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;
  if (!readFile<gloop::obj::ObjReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}



bool readVTK(
    std::istream &in,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::vtk::VtkReader>(in, result, transform);
}

bool readVTK(
    const std::string &in_file,
    carve::input::Input &result,
    const carve::math::Matrix &transform) {
  return readFile<gloop::vtk::VtkReader>(in_file, result, transform);
}

carve::poly::Polyhedron *readVTK(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::vtk::VtkReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::poly::Polyhedron *readVTK(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::poly::Polyhedron *result;
  if (!readFile<gloop::vtk::VtkReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readVTKasMesh(
    std::istream &in,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;
  if (!readFile<gloop::vtk::VtkReader>(in, result, transform)) {
    return NULL;
  }
  return result;
}

carve::mesh::MeshSet<3> *readVTKasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform) {
  carve::mesh::MeshSet<3> *result;
  if (!readFile<gloop::vtk::VtkReader>(in_file, result, transform)) {
    return NULL;
  }
  return result;
}
