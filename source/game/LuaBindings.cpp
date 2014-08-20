#include "LuaType.h"

#include "Log.h"
#include "Cell.h"
#include "Town.h"

namespace LuaType
{
	int logLog(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::Instance().writeLog(luaInstance.getFromLua<char*>(i));
			
		return 0;
	}

	int logWarning(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::Instance().writeWarning(luaInstance.getFromLua<char*>(i));

		return 0;
	}
	
	int logError(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::Instance().writeError(luaInstance.getFromLua<char*>(i));

		return 0;
	}
	
	template<>
	void registerValue<Log>(LuaInstance *instance, Log* value)
	{
		instance->registerTableFunction("Log", LuaType::logLog);
		instance->registerTableFunction("Warn", LuaType::logWarning);
		instance->registerTableFunction("Error", LuaType::logError);
	}

	template<>
	void registerValue<Cell>(LuaInstance *instance, Cell* value)
	{
		const Cell::Info& info = value->GetInfo();

		instance->registerTableConstant<const char*, void*>("ptr", value);
		instance->registerTableConstant<const char*, int>("membersCount", info.membersCount);
		instance->registerTableConstant<const char*, float>("cash", info.cash);
		instance->registerTableConstant<const char*, float>("morale", info.morale);
		registerField<Town>(instance, "town", info.town._Get());
		//instance->registerTableFunction("getRotation", LuaType::cmdGetRotation);
	}

	template<>
	void registerValue<Town>(LuaInstance *instance, Town* value)
	{
		const Town::Info& info = value->GetInfo();

		instance->registerTableConstant<const char*, void*>("ptr", value);
		instance->registerTableConstant<const char*, const char*>("name", info.name.c_str());
		instance->registerTableConstant<const char*, float>("population", info.population);
		instance->registerTableConstant<const char*, float>("rank", info.rank);
	}
}