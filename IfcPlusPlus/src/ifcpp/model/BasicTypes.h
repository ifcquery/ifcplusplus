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
#pragma warning( disable: 4305 )

#if __has_include(<version>)
#include <version>
#endif

#ifdef __cpp_lib_execution
	#include <execution>
	#if defined(_DEBUG_LOOP_SEQENTIAL) || defined(_DEBUG)
		#define FOR_EACH_LOOP std::for_each( std::execution::seq,
	#else
		#define FOR_EACH_LOOP std::for_each( std::execution::par,
	#endif
#else
	#define FOR_EACH_LOOP std::for_each(
#endif

// don't use glm::vec4, because it's x,y,z,w members depend on some defines, which can lead to lost colors
struct vec4
{
	vec4() : r(0), g(0), b(0), a(0) {}
	vec4(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	float r, g, b, a;
};

#define BuildingModelMapType std::unordered_map

#if _MSC_VER >= 1600

#include <memory>
#if _MSC_VER < 1900
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;
using std::tr1::make_shared;
#else
using std::shared_ptr;
using std::weak_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;
#endif

#elif defined __clang__
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;

#elif defined __GNUC__ && !defined(__FreeBSD__)
#if __GNUC__ < 5
#include <tr1/memory>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;
using std::tr1::make_shared;
#else
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;
#endif

#define _stricmp strcasecmp

#elif defined(__FreeBSD__)

// Requires clang++ and libc++
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::dynamic_pointer_cast;
using std::make_shared;

#define _stricmp strcasecmp

#else

#ifndef BOOST_SP_USE_QUICK_ALLOCATOR
#define BOOST_SP_USE_QUICK_ALLOCATOR
#endif
#ifndef BOOST_SP_NO_ATOMIC_ACCESS
#define BOOST_SP_NO_ATOMIC_ACCESS
#endif

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using boost::shared_ptr;
using boost::weak_ptr;
using boost::dynamic_pointer_cast;
using boost::make_shared;

#endif
