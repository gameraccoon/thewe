#include "LuaInstance.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <luabind/luabind.hpp>

#include <sstream>

#include "Log.h"

int luabindErrorHandler(lua_State* L)
{
	// log the error message
	luabind::object luaMsg(luabind::from_stack(L, -1));
	std::ostringstream message;
	message << "lua run-time error: " << luaMsg;

	// log the callstack
	std::string traceback = luabind::call_function<std::string>(luabind::globals(L)["debug"]["traceback"]);

	Log::Instance().writeError(message.str() + "<br/>" + traceback);

	// return unmodified error object
	return 1;
}

LuaInstance::LuaInstance()
{
	_luaState = luaL_newstate();
	luaL_openlibs(_luaState);
	luabind::open(_luaState);
	luabind::set_pcall_callback(luabindErrorHandler);
	_isMainInstance = true;
}

LuaInstance::LuaInstance(lua_State *luaState)
{
	_luaState = luaState;
	_isMainInstance = false;
}

LuaInstance::~LuaInstance()
{
	if (_isMainInstance)
	{
		lua_close(_luaState);
	}
}

lua_State* LuaInstance::GetLuaState()
{
	return _luaState;
}

int LuaInstance::ExecScript(const char* script)
{
	int res = luaL_dostring(_luaState, script);

	if (res != 0)
	{
		Log::Instance().writeWarning(lua_tostring(_luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::ExecScriptFromFile(const char* scriptFileName)
{
	int res = luaL_dofile(_luaState, scriptFileName);
	
	if (res != 0)
	{
		Log::Instance().writeWarning(lua_tostring(_luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::GetArgumentsCount()
{
	return lua_gettop(_luaState);
}

template<>
int LuaInstance::GetFromLua<int>(int index)
{
	return lua_tointeger(_luaState, index);
}

template<>
double LuaInstance::GetFromLua<double>(int index)
{
	return lua_tonumber(_luaState, index);
}

template<>
char* LuaInstance::GetFromLua<char*>(int index)
{
	return (char*)lua_tostring(_luaState, index);
}

template<>
const char* LuaInstance::GetFromLua<const char*>(int index)
{
	return (const char*)lua_tostring(_luaState, index);
}

template<>
bool LuaInstance::GetFromLua<bool>(int index)
{
	return lua_toboolean(_luaState, index) != 0;
}

template<>
void* LuaInstance::GetFromLua<void*>(int index)
{
	return lua_touserdata(_luaState, index);
}


template<>
void LuaInstance::SendToLua<int>(int value)
{
	lua_pushinteger(_luaState, value);
}

template<>
void LuaInstance::SendToLua<double>(double value)
{
	lua_pushnumber(_luaState, value);
}

template<>
void LuaInstance::SendToLua<float>(float value)
{
	lua_pushnumber(_luaState, value);
}

template<>
void LuaInstance::SendToLua<const char*>(const char* value)
{
	lua_pushstring(_luaState, value);
}

template<>
void LuaInstance::SendToLua<bool>(bool value)
{
	lua_pushboolean(_luaState, value);
}

template<>
void LuaInstance::SendToLua<lua_CFunction>(lua_CFunction value)
{
	lua_pushcfunction(_luaState, value);
}

template<>
void LuaInstance::SendToLua<void*>(void* value)
{
	lua_pushlightuserdata(_luaState, value);
}
