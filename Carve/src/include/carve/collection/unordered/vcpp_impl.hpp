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

#include <hash_map>
#include <hash_set>

namespace std {

  namespace {

    template<class Value, class Hash> class hash_traits {
      Hash hash_value;
      std::less<Value> comp;
    public:
      enum {
        bucket_size = 4,
        min_buckets = 8
      };
      // hash _Keyval to size_t value
      size_t operator()(const Value& v) const {
        return ((size_t)hash_value(v));
      }
      // test if _Keyval1 ordered before _Keyval2
      bool operator()(const Value& v1, const Value& v2) const {
        return (comp(v1, v2));
      }
    };

  }

  template <typename Key, typename T, typename Hash = stdext::hash_compare<Key, less<Key> >, typename Pred = std::equal_to<Key> >
  class unordered_map 
    : public stdext::hash_map<Key, T, hash_traits<Key, Hash> > {
    typedef stdext::hash_map<Key, T, hash_traits<Key, Hash> > super;
  public:
    unordered_map() : super() {}
  };

  template <typename Value, typename Hash = stdext::hash_compare<Key, less<Key> >, typename Pred = std::equal_to<Value> >
  class unordered_set
    : public stdext::hash_set<Value, hash_traits<Value, Hash> > {
    typedef stdext::hash_set<Value, hash_traits<Value, Hash> > super;
  public:
    unordered_set() : super() {}
  };

}

#undef UNORDERED_COLLECTIONS_SUPPORT_RESIZE
