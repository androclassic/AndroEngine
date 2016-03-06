#include "Function.h"




Function::Function(void(*fn)(Variable& self, Variable& ret, Variable *args, unsigned int argCount), const TypeInfo** args, unsigned int argCount, const TypeInfo* ret, const TypeInfo* self)
	: m_signeture(self, ret, args, argCount)
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