
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
