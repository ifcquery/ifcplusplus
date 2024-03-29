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

#pragma warning (disable: 4250 4251 4996)
#ifndef BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
	#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#endif

// IFCQUERY_LIB needs to be defined in the settings of the library (IfcPlusPlus). Do not define IFCQUERY_LIB in your application though

#if defined(_MSC_VER)
	#ifdef IFCQUERY_LIB
		#define IFCQUERY_EXPORT __declspec(dllexport)
	#else
		#ifdef IFCQUERY_STATIC_LIB
			#define IFCQUERY_EXPORT
		#else
			#define IFCQUERY_EXPORT __declspec(dllimport)
		#endif
	#endif
#endif

#ifndef IFCQUERY_EXPORT
#define IFCQUERY_EXPORT // Define empty if not necessary
#endif
