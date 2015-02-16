/* -*-c++-*- IfcPlusPlus - www.ifcplusplus.com  - Copyright (C) 2011 Fabian Gerold
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#pragma once

#if _MSC_VER >= 1600

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

#elif _MSC_VER >= 1500

#include <memory>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;

#elif defined __GNUC__ && !defined(__FreeBSD__)

#include <tr1/memory>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::dynamic_pointer_cast;

#define _stricmp strcasecmp

#elif defined(__FreeBSD__)

// Requires clang++ and libc++
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::dynamic_pointer_cast;

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

#endif
