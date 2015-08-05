#pragma once

#include "AndroUtils.h"

namespace androutils
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
			wchar_t buffer[512];
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

		}

		inline auto Trace(wchar_t const * format, ...) -> void
		{
			
			va_list args;
			va_start(args, format);

			wchar_t buffer[256];

			VERIFY(-1 != _vsnwprintf_s(buffer,
										_countof(buffer) - 1,
										format,
										args));

			va_end(args);

			OutputDebugStringW(buffer);
		}
	};
}

	#define TRACE androutils::Tracer(__FILE__, __LINE__)
