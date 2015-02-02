#ifndef LUA_INSTANCE_H
#define LUA_INSTANCE_H

typedef struct lua_State lua_State;

#include <memory>

class LuaInstance
{
public:
	LuaInstance();
	~LuaInstance();

	int ExecScript(const char* script);
	int ExecScriptFromFile(const char* scriptFileName);

	lua_State* GetLuaState();

	template<typename T>
	void BindClass();

	template<typename T>
	void RegisterVariable(const char* name, T* value);


private:
	lua_State *_luaState;
	bool _isMainInstance;

	LuaInstance(lua_State *_luaState);
};

#endif
