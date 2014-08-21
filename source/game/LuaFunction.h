#ifndef LUA_FUNCTION
#define LUA_FUNCTION

#include "LuaInstance.h"

class LuaFunction
{
public:
	void readyToRunFunction(LuaInstance *instance, const char* functionName);
	void runFunction(int paramsCount, int resultsCount);
	void clearAfterFunction();

	template<typename T>
	void sendParameter(T value);

	template<typename T>
	T getReturnValue(int index);
private:
	lua_State *luaState;
	int stackState;
};


template<typename T>
void LuaFunction::sendParameter(T value)
{
	LuaInstance inst(this->luaState);
	inst.sendToLua<T>(value);
}

template<typename T>
T LuaFunction::getReturnValue(int index)
{
	LuaInstance inst(this->luaState);
	return inst.getFromLua<T>(value);
}

#endif