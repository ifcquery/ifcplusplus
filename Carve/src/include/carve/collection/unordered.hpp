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

#if defined(HAVE_STD_UNORDERED_COLLECTIONS)

#  include <carve/collection/unordered/std_impl.hpp>

#elif defined(HAVE_TR1_UNORDERED_COLLECTIONS)

#  include <carve/collection/unordered/tr1_impl.hpp>

#elif defined(HAVE_BOOST_UNORDERED_COLLECTIONS)

#  include <carve/collection/unordered/boost_impl.hpp>

#elif defined(HAVE_LIBSTDCPP_UNORDERED_COLLECTIONS)

#  include <carve/collection/unordered/libstdcpp_impl.hpp>

#elif defined(_MSC_VER) && _MSC_VER >= 1300

#  include <carve/collection/unordered/vcpp_impl.hpp>

#else

#  include <carve/collection/unordered/fallback_impl.hpp>

#endif
