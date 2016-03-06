#pragma once
#include "TypeInfo.h"

class Variable
{
public:
	Variable();

	
	Variable::Variable(const Variable& rhs)
	{
		m_data = rhs.m_data;
		m_typeinfo = rhs.m_typeinfo;
	}
	Variable& Variable::operator=(const Variable& rhs)
	{
		m_data = rhs.m_data;
		m_typeinfo = rhs.m_typeinfo;

		return *this;
	}

	template<typename T>
	Variable(const T& data);

	template<typename T>
	Variable(T* data);

	Variable(void* data, const TypeInfo* type);

	template<typename T>
	T& operator=(const T& data);

	template<typename T>
	T* operator=(T* data);



	inline void* GetVoidPtr();

	inline const TypeInfo* GetType() const;


	// Cast helper exists to properly handle pointer types
	template <typename T>
	struct CastHelper
	{
		static T& Cast(void *& data)
		{
			return *(T *&)(data);
		}
	};

	template <typename T>
	struct CastHelper<T *>
	{
		static T *& Cast(void *& data)
		{
			return (T *&)data;
		}
	};

	template <typename T>
	T& GetValue(void)
	{
		ASSERT(GET_TYPE(T) == m_typeinfo);
		return CastHelper<T>::Cast(m_data);
	}

	void ToLua(lua_State* L) 
	{
		m_typeinfo->ToLua(L, *this);
	}

	void FromLua(lua_State *L, int index) 
	{
		m_typeinfo->FromLua(L, index, this);
	}

private:

	void* m_data;
	const TypeInfo* m_typeinfo;
};

template<typename T>
Variable::Variable(const T& data)
{
	m_data = (void*)&data;
	m_typeinfo = GET_TYPE(T);
}
template<typename T>
Variable::Variable(T* data)
{
	m_data = (void*)data;
	m_typeinfo = GET_TYPE(T*);
}

template<typename T>
T& Variable::operator=(const T& data)
{
	m_data = (void*)&data;
	m_typeinfo = GET_TYPE(T);
	return data;
}

template<typename T>
T* Variable::operator=(T* data)
{
	m_data = (void*)data;
	m_typeinfo = GET_TYPE(T*);
	return data;
}