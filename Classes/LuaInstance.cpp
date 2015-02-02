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
