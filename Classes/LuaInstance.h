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

	int GetArgumentsCount();

	lua_State* GetLuaState();

	template<typename T>
	T GetFromLua(int index);
	template<typename T>
	void SendToLua(T value);

	template<typename T>
	void BindClass();

	template<typename T>
	void RegisterVariable(const char* name, T* value);
	void UnregisterVariable(const char* name);
private:
	lua_State *_luaState;
	bool _isMainInstance;

	LuaInstance(lua_State *_luaState);
};

#endif
