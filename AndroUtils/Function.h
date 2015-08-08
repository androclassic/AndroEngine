#pragma once
#include "Variable.h"
#define max_arg_num 10

#define make_gen( func ) BuildFuncion<decltype(&func), &func>(&func)


class Signeture
{
public:
	Signeture(const TypeInfo* self,const TypeInfo* ret,const TypeInfo** args, unsigned int argc)
	{
		for (unsigned int i = 0; i < argc; i++)
			m_args[i] = args[i];

		m_argCount = argc;
		m_ret = ret;
		m_self = self;
	}
	Signeture(Signeture& rhs)
	{
		for (unsigned int i = 0; i < rhs.m_argCount; i++)
			m_args[i] = rhs.m_args[i];
		m_argCount = rhs.m_argCount;
		m_ret = rhs.m_ret;
		m_self = rhs.m_self;
	}

	Signeture& operator=(const Signeture& rhs)
	{
		for (unsigned int i = 0; i < rhs.m_argCount; i++)
			m_args[i] = rhs.m_args[i];
		m_argCount = rhs.m_argCount;
		m_ret = rhs.m_ret;
		m_self = rhs.m_self;

		return *this;
	}

	inline const TypeInfo* GetArg(unsigned int i) const
	{
		ASSERT(i < m_argCount);
		return m_args[i];
	}

	inline const TypeInfo* GetReturnType() const
	{
		return m_ret;
	}

	inline const TypeInfo* GetSelfType() const
	{
		return m_self;
	}

	inline unsigned int ArgCount() const
	{
		return m_argCount;
	}

private:
	unsigned m_argCount;
	const TypeInfo*  m_args[max_arg_num];
	const TypeInfo*  m_ret;
	const TypeInfo*  m_self;
};


class Function
{
public:
		Function(void(*fn)(Variable& self, Variable& ret, Variable *args, unsigned int argCount), const TypeInfo** args, unsigned int argCount, const TypeInfo* ret = nullptr, const TypeInfo* self = nullptr);
		Function(Function& f);
		Function& operator=(const Function& f);
		void operator()(Variable& self, Variable& ret, Variable *args, unsigned int argCount);
		const Signeture*  GetSigneture() const;


private:
	Signeture m_signeture;
	void(*m_callFunction)(Variable& self, Variable& ret, Variable *args, unsigned int argCount);
};




Function::Function(void(*fn)(Variable& self, Variable& ret, Variable *args, unsigned int argCount),const TypeInfo** args, unsigned int argCount, const TypeInfo* ret, const TypeInfo* self)
	: m_signeture(self,ret,args,argCount)
	, m_callFunction(fn)
{}

Function::Function(Function& f)
	: m_signeture(f.m_signeture)
	, m_callFunction(f.m_callFunction)
{}
Function& Function::operator=(const Function& f)
{
	m_callFunction = f.m_callFunction;
	m_signeture = f.m_signeture;
	return *this;
}
void Function::operator()(Variable& self, Variable& ret, Variable *args, unsigned int argCount)
{
	(*m_callFunction)(self, ret, args, argCount);
}


const Signeture* Function::GetSigneture() const
{
	return &m_signeture;
}

#include "BuildFunction.h"
