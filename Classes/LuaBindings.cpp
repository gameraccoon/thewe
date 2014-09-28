#include "LuaInstance.h"

#include <luabind/luabind.hpp>

#include "Log.h"
#include "Cell.h"
#include "Task.h"
#include "MessageManager.h"

template<>
void LuaInstance::BindClass<Log>()
{
	luabind::module(_luaState) [
	luabind::class_<Log>("LogClass")
			.def("log", &Log::writeLog)
			.def("warning", &Log::writeWarning)
			.def("error", &Log::writeError)
	];
}

template<>
void LuaInstance::BindClass<MessageManager>()
{
	luabind::module(_luaState) [
	luabind::class_<MessageManager>("MessageManagerClass")
			.def("sendMessage", &MessageManager::SendGameMessage)
	];
}

template<>
void LuaInstance::BindClass<const Cell::Info>()
{
	luabind::module(_luaState) [
	luabind::class_<Cell::Info>("CellInfoClass")
		.def_readonly("cash", &Cell::Info::cash)
		.def_readonly("contantement", &Cell::Info::contentment)
		.def_readonly("membersCount", &Cell::Info::membersCount)
		.def_readonly("morale", &Cell::Info::morale)
	];
}

template<>
void LuaInstance::BindClass<Task>()
{
	luabind::module(_luaState) [
	luabind::class_<Task>("TaskClass")
			.def("abort", &Task::Abort)
	];
}

template<>
void LuaInstance::RegisterVariable<Log>(const char* name, Log* value)
{
	luabind::globals(_luaState)[name] = value;
}

template<>
void LuaInstance::RegisterVariable<MessageManager>(const char* name, MessageManager* value)
{
	luabind::globals(_luaState)[name] = value;
}

template<>
void LuaInstance::RegisterVariable<const Cell::Info>(const char* name, const Cell::Info* value)
{
	luabind::globals(_luaState)[name] = value;
}
