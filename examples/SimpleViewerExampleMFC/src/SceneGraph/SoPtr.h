#pragma once

#include <Inventor/misc/SoBase.h>

template<typename T>
class SoPtr
{
public:
	T* m_ptr = nullptr;

	SoPtr(){}
	SoPtr( T* ptr )
	{
		m_ptr = ptr;
		if( m_ptr )
		{
			m_ptr->ref();
		}
	}
	~SoPtr()
	{
		if( m_ptr )
		{
			m_ptr->unref();
		}
		m_ptr = nullptr;
	}

	bool valid()
	{
		if( m_ptr == nullptr )
		{
			return false;
		}
		if( m_ptr->getRefCount() > 0 )
		{
			return true;
		}
		return false;
	}
	void reset()
	{
		if( m_ptr == nullptr )
		{
			return;
		}
		m_ptr->unref();
		m_ptr = nullptr;
	}
	T* get()
	{
		return m_ptr;
	}

	T* operator -> ()
	{
		return m_ptr;
	}

	SoPtr& operator = ( const SoPtr& rp )
	{
		assign( rp );
		return *this;
	}

	template<class Other> SoPtr& operator = ( const SoPtr<Other>& rp )
	{
		assign( rp );
		return *this;
	}

	inline SoPtr& operator = ( T* ptr )
	{
		if( m_ptr==ptr ) return *this;
		T* tmp_ptr = m_ptr;
		m_ptr = ptr;
		if( m_ptr ) m_ptr->ref();
		// unref second to prevent any deletion of any object which might
		// be referenced by the other object. i.e rp is child of the
		// original _ptr.
		if( tmp_ptr ) tmp_ptr->unref();
		return *this;
	}

	// comparison operators for SoPtr.
	//bool operator == ( const SoPtr& rp ) const { return ( m_ptr==rp._ptr ); }
	//bool operator == ( const T* ptr ) const { return ( m_ptr==ptr ); }
	//friend bool operator == ( const T* ptr, const SoPtr& rp ) { return ( ptr==rp.m_ptr ); }

	//bool operator != ( const SoPtr& rp ) const { return ( m_ptr!=rp.m_ptr ); }
	//bool operator != ( const T* ptr ) const { return ( m_ptr!=ptr ); }
	//friend bool operator != ( const T* ptr, const SoPtr& rp ) { return ( ptr!=rp.m_ptr ); }

private:

	template<class Other> void assign( const SoPtr<Other>& rp )
	{
		if( m_ptr==rp.m_ptr ) return;
		T* tmp_ptr = m_ptr;
		m_ptr = rp.m_ptr;
		if( m_ptr ) m_ptr->ref();
		// unref second to prevent any deletion of any object which might
		// be referenced by the other object. i.e rp is child of the
		// original _ptr.
		if( tmp_ptr ) tmp_ptr->unref();
	}

	template<class Other> friend class SoPtr;
};

