#include "LuaInstance.h"

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "Log.h"

LuaInstance::LuaInstance()
{
	this->_luaState = luaL_newstate();
	luabind::open(this->_luaState);
	_isMainInstance = true;

}

LuaInstance::LuaInstance(lua_State *luaState)
{
	this->_luaState = luaState;
	_isMainInstance = false;
}

LuaInstance::~LuaInstance()
{
	if (_isMainInstance)
	{
		lua_close(this->_luaState);
	}
}

int LuaInstance::ExecScript(const char* script)
{
	int res = luaL_dostring(this->_luaState, script);

	if (res != 0)
	{
		Log::Instance().writeWarning(lua_tostring(this->_luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::ExecScriptFromFile(const char* scriptFileName)
{
	int res = luaL_dofile(this->_luaState, scriptFileName);
	
	if (res != 0)
	{
		Log::Instance().writeWarning(lua_tostring(this->_luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::GetArgumentsCount()
{
	return lua_gettop(this->_luaState);
}

template<>
int LuaInstance::GetFromLua<int>(int index)
{
	return lua_tointeger(this->_luaState, index);
}

template<>
double LuaInstance::GetFromLua<double>(int index)
{
	return lua_tonumber(this->_luaState, index);
}

template<>
char* LuaInstance::GetFromLua<char*>(int index)
{
	return (char*)lua_tostring(this->_luaState, index);
}

template<>
const char* LuaInstance::GetFromLua<const char*>(int index)
{
	return (const char*)lua_tostring(this->_luaState, index);
}

template<>
bool LuaInstance::GetFromLua<bool>(int index)
{
	return lua_toboolean(this->_luaState, index) != 0;
}

template<>
void* LuaInstance::GetFromLua<void*>(int index)
{
	return lua_touserdata(this->_luaState, index);
}


template<>
void LuaInstance::SendToLua<int>(int value)
{
	lua_pushinteger(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<double>(double value)
{
	lua_pushnumber(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<float>(float value)
{
	lua_pushnumber(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<const char*>(const char* value)
{
	lua_pushstring(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<bool>(bool value)
{
	lua_pushboolean(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<lua_CFunction>(lua_CFunction value)
{
	lua_pushcfunction(this->_luaState, value);
}

template<>
void LuaInstance::SendToLua<void*>(void* value)
{
	lua_pushlightuserdata(this->_luaState, value);
}
