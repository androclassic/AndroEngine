#pragma once

#include "AndroUtils.h"

#ifdef _WIN32
#include<Windows.h>
#endif

namespace andro
{
	struct Tracer
	{
		char*	m_filename;
		int		m_line;

		Tracer(char* filename, int line) : m_filename{ filename }, m_line{ line }
		{
		}



		template<typename... Args>
		auto operator()(wchar_t const* format, Args... args) const -> void
		{
			wchar_t buffer[1024];
#ifdef _WIN32
			auto count = swprintf_s(buffer,
									L"[ANDRO]%S(%d) ",
									m_filename,
									m_line);
			
			VERIFY(-1 != _snwprintf_s(buffer + count,
										_countof(buffer) - count,
										_countof(buffer) - count - 1,
										format,
										args...));

			OutputDebugStringW(buffer);
#else			
			printf("%S \n", format);
#endif
		}

		template<typename... Args>
		auto operator()(char const* format, Args... args) const -> void
		{
			const int buffer_size = 1024;
			char buffer[buffer_size];
#ifdef _WIN32
			auto count = sprintf_s(buffer,
				buffer_size,
				"[ANDRO]%s(%d)", m_filename, m_line);

			VERIFY(-1 != _snprintf_s(buffer + count,
				_countof(buffer) - count,
				_countof(buffer) - count - 1,
				format,
				args...));

			OutputDebugStringA(buffer);
#else			
			printf("%S \n", format);
#endif
		}




		inline auto Trace(wchar_t const * format, ...) -> void
		{
			
#ifdef _WIN32
			va_list args;
			va_start(args, format);

			wchar_t buffer[256];

			VERIFY(-1 != _vsnwprintf_s(buffer,
										_countof(buffer) - 1,
										format,
										args));

			va_end(args);

			OutputDebugStringW(buffer);
#else			
			printf("%S \n", format);
#endif
		}

		inline auto Trace(char const * format, ...) -> void
		{
			printf("%s \n", format);
		}
	};
}

#define TRACE andro::Tracer(__FILE__, __LINE__)
