#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#include "LuaInstance.h"

namespace lua
{
	void BindGameClasses(LuaInstance* luaInstance);
	void BindFunctions(LuaInstance* luaInstance);
	void BindGlobalData(LuaInstance* luaInstance);
}

#endif // LUA_BINDINGS_H
