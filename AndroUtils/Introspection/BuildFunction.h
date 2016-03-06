#pragma once

//----------------------------------------------------------------------- 0 arg
template<typename FunctionType, FunctionType FunctionPtr>
void Call(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 0);
	(*FunctionPtr)();
}

template<typename FunctionType, FunctionType FunctionPtr>
Function BuildFuncion(void(*fn)())
{
	return Function(&Call<FunctionType, FunctionPtr>, NULL, 0);
}

//----------------------------------------------------------------------- 1 arg
template<typename FunctionType, FunctionType FunctionPtr, typename Arg1>
void Call(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 1);
	(*FunctionPtr)(args[0].GetValue<Arg1>());
}
template<typename FunctionType, FunctionType FunctionPtr, typename Arg1>
Function BuildFuncion(void(*fn)(Arg1 a))
{
	const TypeInfo* type = GET_TYPE(Arg1);
	return Function(&Call<FunctionType, FunctionPtr, Arg1>, &type, 1);
}

//----------------------------------------------------------------------- 2 arg
template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2>
Function BuildFuncion(void(*fn)(Arg1, Arg2))
{
	const TypeInfo* types[2];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);

	return Function(&Call<FunctionType, FunctionPtr, Arg1, Arg2>, types, 2);
}

template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2>
void Call(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 2);
	(*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>());
}

//----------------------------------------------------------------------- 3 arg
template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3>
Function BuildFuncion(void(*fn)(Arg1, Arg2, Arg3))
{
	const TypeInfo* types[3];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);

	return Function(&Call<FunctionType, FunctionPtr, Arg1, Arg2, Arg3>, types, 3);
}

template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3>
void Call(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 3);
	(*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>());
}
//----------------------------------------------------------------------- 4 arg
template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Function BuildFuncion(void(*fn)(Arg1, Arg2, Arg3, Arg4))
{
	const TypeInfo* types[4];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);
	types[3] = GET_TYPE(Arg4);

	return Function(&Call<FunctionType, FunctionPtr, Arg1, Arg2, Arg3, Arg4>, types, 4);
}


template<typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Call(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 4);
	(*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>(), args[3].GetValue<Arg4>());
}


//-------------------------------------------------------------------------------------------------------------
// call with return value
//-------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------- 0 arg
template<typename FunctionType, FunctionType FunctionPtr, typename R>
void CallReturn(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 0);
	ret.GetValue<R>() = (*FunctionPtr)();
}

template<typename FunctionType, FunctionType FunctionPtr, typename R>
Function BuildFuncion(R(*fn)())
{
	const TypeInfo* retType = GET_TYPE(R);
	return Function(&CallReturn<FunctionType, FunctionPtr, R>, NULL, 0, retType);
}

//----------------------------------------------------------------------- 1 arg
template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1>
Function BuildFuncion(R(*fn)(Arg1))
{
	const TypeInfo* type = GET_TYPE(Arg1);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturn<FunctionType, FunctionPtr, R, Arg1>, &type, 1, retType);
}

template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1>
void CallReturn(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 1);
	ret.GetValue<R>() = (*FunctionPtr)(args[0].GetValue<Arg1>());
}


//----------------------------------------------------------------------- 2 arg
template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2>
Function BuildFuncion(R(*fn)(Arg1, Arg2))
{
	const TypeInfo* types[2];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);

	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturn<FunctionType, FunctionPtr, R, Arg1, Arg2>, types, 2, retType);
}

template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2>
void CallReturn(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 2);
	ret.GetValue<R>() = (*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>());
}

//----------------------------------------------------------------------- 3 arg
template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2, typename Arg3>
Function BuildFuncion(R(*fn)(Arg1, Arg2, Arg3))
{
	const TypeInfo* types[3];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);

	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturn<FunctionType, FunctionPtr, R, Arg1, Arg2, Arg3>, types, 3, retType);
}

template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2, typename Arg3>
void CallReturn(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 3);
	ret.GetValue<R>() = (*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>());
}

//----------------------------------------------------------------------- 4 arg
template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Function BuildFuncion(R(*fn)(Arg1, Arg2, Arg3, Arg4))
{
	const TypeInfo* types[4];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);
	types[3] = GET_TYPE(Arg4);

	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturn<FunctionType, FunctionPtr, R, Arg1, Arg2, Arg3, Arg4>, types, 4, retType);
}

template<typename FunctionType, FunctionType FunctionPtr, typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void CallReturn(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 4);
	ret.GetValue<R>() = (*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>(), args[3].GetValue<Arg4>());
}






//------------------------------------------------------------------------------------------------------------------
// Methods no return 
//------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------- 4 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Function BuildFuncion(void(S::*fn)(Arg1, Arg2, Arg3, Arg4))
{
	const TypeInfo* types[4];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);
	types[3] = GET_TYPE(Arg4);

	const TypeInfo* slef_type = GET_TYPE(S);
	return Function(&CallVoidMethod<FunctionType, FunctionPtr, S, Arg1, Arg2, Arg3, Arg4>, types, 4, nullptr, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void CallVoidMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 4);
	(((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>(), args[3].GetValue<Arg4>());
}


//----------------------------------------------------------------------- 3 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2, typename Arg3>
Function BuildFuncion(void(S::*fn)(Arg1, Arg2, Arg3))
{
	const TypeInfo* types[3];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);

	const TypeInfo* slef_type = GET_TYPE(S);
	return Function(&CallVoidMethod<FunctionType, FunctionPtr, S, Arg1, Arg2, Arg3>, types, 3, nullptr, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2, typename Arg3>
void CallVoidMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 3);
	(((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>());
}


//----------------------------------------------------------------------- 2 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2>
Function BuildFuncion(void(S::*fn)(Arg1, Arg2))
{
	const TypeInfo* types[2];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);

	const TypeInfo* slef_type = GET_TYPE(S);
	return Function(&CallVoidMethod<FunctionType, FunctionPtr, S, Arg1, Arg2>, types, 2, nullptr, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1, typename Arg2>
void CallVoidMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 2);
	(((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>());
}

//----------------------------------------------------------------------- 1 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1>
Function BuildFuncion(void(S::*fn)(Arg1))
{
	const TypeInfo* types[1];
	types[0] = GET_TYPE(Arg1);

	const TypeInfo* slef_type = GET_TYPE(S);
	return Function(&CallVoidMethod<FunctionType, FunctionPtr, S, Arg1>, types, 1, nullptr, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename Arg1>
void CallVoidMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 1);
	(((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>());
}

//----------------------------------------------------------------------- 0  arg
template<typename FunctionType, FunctionType FunctionPtr, typename S>
Function BuildFuncion(void(S::*fn)())
{
	const TypeInfo* slef_type = GET_TYPE(S);
	return Function(&CallVoidMethod<FunctionType, FunctionPtr, S>, nullptr, 0, nullptr, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S>
void CallVoidMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 0);
	(((S *)context.GetVoidPtr())->*FunctionPtr)();
}


//------------------------------------------------------------------------------------------------------------------
// Methods  with return 
//------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------- 4 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R ,typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Function BuildFuncion(R (S::*fn)(Arg1, Arg2, Arg3, Arg4))
{
	const TypeInfo* types[4];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);
	types[3] = GET_TYPE(Arg4);

	const TypeInfo* slef_type = GET_TYPE(S);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturnMethod<FunctionType, FunctionPtr, S, R, Arg1, Arg2, Arg3, Arg4>, types, 4, retType, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S,typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void CallReturnMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 4);
	ret.GetValue<R>() = (((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>(), args[3].GetValue<Arg4>());
}

//----------------------------------------------------------------------- 3 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1, typename Arg2, typename Arg3>
Function BuildFuncion(R(S::*fn)(Arg1, Arg2, Arg3))
{
	const TypeInfo* types[3];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);
	types[2] = GET_TYPE(Arg3);

	const TypeInfo* slef_type = GET_TYPE(S);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturnMethod<FunctionType, FunctionPtr, S, R, Arg1, Arg2, Arg3>, types, 3, retType, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1, typename Arg2, typename Arg3>
void CallReturnMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 3);
	ret.GetValue<R>() = (((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>(), args[2].GetValue<Arg3>());
}

//----------------------------------------------------------------------- 2 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1, typename Arg2>
Function BuildFuncion(R(S::*fn)(Arg1, Arg2))
{
	const TypeInfo* types[2];
	types[0] = GET_TYPE(Arg1);
	types[1] = GET_TYPE(Arg2);

	const TypeInfo* slef_type = GET_TYPE(S);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturnMethod<FunctionType, FunctionPtr, S, R, Arg1, Arg2>, types, 2, retType, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1, typename Arg2>
void CallReturnMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 2);
	ret.GetValue<R>() = (((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>(), args[1].GetValue<Arg2>());
}

//----------------------------------------------------------------------- 1 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1>
Function BuildFuncion(R(S::*fn)(Arg1))
{
	const TypeInfo* types[1];
	types[0] = GET_TYPE(Arg1);

	const TypeInfo* slef_type = GET_TYPE(S);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturnMethod<FunctionType, FunctionPtr, S, R, Arg1>, types, 1, retType, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R, typename Arg1>
void CallReturnMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 1);
	ret.GetValue<R>() = (((S *)context.GetVoidPtr())->*FunctionPtr)(args[0].GetValue<Arg1>());
}

//----------------------------------------------------------------------- 0 arg
template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R>
Function BuildFuncion(R(S::*fn)())
{
	const TypeInfo* slef_type = GET_TYPE(S);
	const TypeInfo* retType = GET_TYPE(R);

	return Function(&CallReturnMethod<FunctionType, FunctionPtr, S, R>, nullptr, 0, retType, slef_type);
}

template<typename FunctionType, FunctionType FunctionPtr, typename S, typename R>
void CallReturnMethod(Variable& context, Variable& ret, Variable* args, unsigned int argCount)
{
	ASSERT(argCount == 0);
	ret.GetValue<R>() = (((S *)context.GetVoidPtr())->*FunctionPtr)();
}
