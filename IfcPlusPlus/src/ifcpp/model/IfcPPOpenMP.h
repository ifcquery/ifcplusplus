/* -*-c++-*- IFC++ www.ifcquery.com
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

#ifdef IFCPP_OPENMP

#include <omp.h>

class Mutex 
{ 
public: 
	Mutex()
	{
		omp_init_lock(&m_lock);
	}
	~Mutex()
	{
		omp_destroy_lock(&m_lock);
	}
	
	void lock()
	{
		omp_set_lock(&m_lock);
	}

	void unlock()
	{
		omp_unset_lock(&m_lock);
	}
	bool try_to_lock()
	{
		int lock_result = omp_test_lock(&m_lock);
		return lock_result != 0;
	}

protected:
	omp_lock_t m_lock; 
}; 

class ScopedLock
{ 
public: 
	ScopedLock( Mutex& m )  : m_mutex(m)
	{
		m_mutex.lock();
	}
	~ScopedLock()
	{
		m_mutex.unlock();
	}

protected:
	Mutex&	m_mutex; 
};

#endif
