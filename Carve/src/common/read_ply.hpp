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

#include <carve/poly.hpp>
#include <carve/matrix.hpp>

#include <istream>
#include <fstream>

#include <carve/input.hpp>



bool readPLY(
    std::istream &in,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

bool readPLY(
    const std::string &in_file,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readPLY(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readPLY(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readPLYasMesh(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readPLYasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());



bool readOBJ(
    std::istream &in,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

bool readOBJ(
    const std::string &in_file,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readOBJ(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readOBJ(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readOBJasMesh(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readOBJasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());



bool readVTK(
    std::istream &in,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

bool readVTK(
    const std::string &in_file,
    carve::input::Input &inputs,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readVTK(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::poly::Polyhedron *readVTK(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readVTKasMesh(
    std::istream &in,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());

carve::mesh::MeshSet<3> *readVTKasMesh(
    const std::string &in_file,
    const carve::math::Matrix &transform = carve::math::Matrix::IDENT());
