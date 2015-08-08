#pragma once

template <typename T>
class AutoList
{
public:
	AutoList()
	{
		next = Head();
		Head() = static_cast<const T *>(this);
	}

	const T *Next(void) const
	{
		return next;
	}

	static const T *& Head(void)
	{
		static const T *p = 0;
		return p;
	}

private:
	const T *next;
};
