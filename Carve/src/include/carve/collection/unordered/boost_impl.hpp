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

#include BOOST_INCLUDE(unordered_set.hpp)
#include BOOST_INCLUDE(unordered_map.hpp)

#include <functional>

namespace std {
  template <typename Key, typename T, typename Hash = boost::hash<Key>,
            typename Pred = std::equal_to<Key> >
  class unordered_map : public boost::unordered_map<Key, T, Hash, Pred> {

  public:
    typedef T data_type;
  };

  template <typename Key, typename T, typename Hash = boost::hash<Key>,
            typename Pred = std::equal_to<Key> >
  class unordered_multimap : public boost::unordered_multimap<Key, T, Hash, Pred> {
  };

  template <typename Value, typename Hash = boost::hash<Value>,
            typename Pred = std::equal_to<Value> >
  class unordered_set : public boost::unordered_set<Value, Hash, Pred> {
  };
}

#undef UNORDERED_COLLECTIONS_SUPPORT_RESIZE
