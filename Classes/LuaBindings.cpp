#include "LuaInstance.h"

#include <luabind/luabind.hpp>

#include "Log.h"
#include "MessageManager.h"
#include "Vector2.h"
#include "Cell.h"
#include "Task.h"

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
void LuaInstance::BindClass<Vector2>()
{
	luabind::module(_luaState) [
	luabind::class_<Vector2>("Vector2Class")
		.def(luabind::constructor<float, float>())
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)
	];
}

template<>
void LuaInstance::BindClass<Cell::Info>()
{
	luabind::module(_luaState) [
	luabind::class_<Cell::Info>("CellInfoClass")
		.def_readwrite("cash", &Cell::Info::cash)
		.def_readwrite("contantement", &Cell::Info::contentment)
		.def_readwrite("membersCount", &Cell::Info::membersCount)
		.def_readwrite("morale", &Cell::Info::morale)
	];
}

template<>
void LuaInstance::BindClass<const Task::Info>()
{
	luabind::module(_luaState) [
	luabind::class_<const Task::Info>("TaskInfoClass")
		.def_readonly("id", &Task::Info::id)
		.def_readonly("moraleLevel", &Task::Info::moralLevel)
		.def_readonly("severity", &Task::Info::severity)
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
