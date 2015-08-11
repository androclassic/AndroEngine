
#pragma once

#include "Trace.h"

namespace andro
{
	template <typename Traits>
	class unique_handle	
	{
		typedef typename Traits::pointer pointer;
		pointer m_value;

		auto close() throw() -> void
		{
			if (m_value != Traits::invalid())
			{
				Traits::close(m_value)
			}
		}

		unique_handle(unique_handle const & );
		auto operator=(unique_handle const &) -> unique_handle&;


	public:
 		explicit unique_handle(pointer value = Traits::invalid() )  throw() : m_value{ value }
		{

		}

		unique_handle(unique_handle&& other) throw() : m_value{ other.release() }
		{
			if (this != &other)
			{
				reset(other.release())
			}
		}

		auto operator=(unique_handle&& other) throw() -> unique_handle&
		{

		}
		~unique_handle()
		{
			close();
		}

		explicit operator bool() const throw()
		{
			return  (m_value != Traits::invalid());
		}

		auto get() const throw() -> pointer
		{
			return m_value;
		}

		auto release() throw() -> pointer
		{
			auto value = m_value;
			m_value = Traits::invalid();
			return value;
		}

		auto reset(pointer value =  Traits::invalid()) throw() -> bool
		{
			if (m_value != value)
			{
				close();
				m_value = value;
			}

			return static_cast<bool>(*this);
		}



	};

	struct null_handle_traits
	{
		typedef  HANDLE pointer;

		auto static invalid() throw() -> pointer
		{
			return nullptr;
		}

		auto static close(pointer handle) throw() -> void
		{
			VERIFY(CloseHandle(handle));
		}
	};

	typedef unique_handle<null_handle_traits> null_handle;

}



