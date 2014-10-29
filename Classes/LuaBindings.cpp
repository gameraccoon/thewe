#include "LuaInstance.h"

#include <luabind/luabind.hpp>

#include "Log.h"
#include "NotificationMessageManager.h"
#include "Vector2.h"
#include "Cell.h"
#include "Task.h"
#include "GameInfo.h"
#include "World.h"

template<>
void LuaInstance::BindClass<Cell>()
{
	luabind::module(_luaState) [
	luabind::class_<Cell>("Cell")
		.def("getInfo", &Cell::GetInfo)
		.def("calcConnectivity", &Cell::CalcConnectivity)
		.def("calcDistanceToTheRootCell", &Cell::CalcDistanceToTheRootCell)
	];
}

template<>
void LuaInstance::BindClass<Cell::Info>()
{
	luabind::module(_luaState) [
	luabind::class_<Cell::Info>("CellInfo")
		.def_readwrite("cash", &Cell::Info::cash)
		.def_readwrite("membersCount", &Cell::Info::membersCount)
		.def_readwrite("ratsCount", &Cell::Info::ratsCount)
		.def_readwrite("techUnitsCount", &Cell::Info::techUnitsCount)
		.def_readwrite("devotion", &Cell::Info::devotion)
		.def_readwrite("morale", &Cell::Info::morale)
		.def_readwrite("experience", &Cell::Info::experience)
		.def_readwrite("fame", &Cell::Info::fame)
		.def_readwrite("townHeartPounding", &Cell::Info::townHeartPounding)
		.def_readwrite("townInfluence", &Cell::Info::townInfluence)
		.def_readwrite("townWelfare", &Cell::Info::townWelfare)
	];
}

template<>
void LuaInstance::BindClass<GameInfo>()
{
	luabind::module(_luaState) [
	luabind::class_<GameInfo>("GameInfoClass")
		.def("getString", &GameInfo::GetString)
		.def("getBool", &GameInfo::GetBool)
		.def("getFloat", &GameInfo::GetFloat)
		.def("getInt", &GameInfo::GetInt)
	];
}

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
void LuaInstance::BindClass<NotificationMessageManager>()
{
	luabind::module(_luaState) [
	luabind::class_<NotificationMessageManager>("MessageManagerClass")
		.def("sendMessage", &NotificationMessageManager::SendGameMessage)
	];
}

template<>
void LuaInstance::BindClass<const Task::Info>()
{
	luabind::module(_luaState) [
	luabind::class_<const Task::Info>("TaskInfo")
		.def_readonly("id", &Task::Info::id)
		.def_readonly("title", &Task::Info::title)
		.def_readonly("chanceToLooseMembers", &Task::Info::chanceToLooseMembers)
		.def_readonly("fameImpact", &Task::Info::fameImpact)
		.def_readonly("heartPoundingLevel", &Task::Info::heartPoundingLevel)
		.def_readonly("level", &Task::Info::level)
		.def_readonly("moraleLevel", &Task::Info::moralLevel)
		.def_readonly("needCash", &Task::Info::needCash)
		.def_readonly("needMembers", &Task::Info::needMembers)
		.def_readonly("needTech", &Task::Info::needTech)
		.def_readonly("politicalImpact", &Task::Info::politicalImpact)
	];
}

template<>
void LuaInstance::BindClass<Tutorial>()
{
	luabind::module(_luaState) [
	luabind::class_<Tutorial>("Tutorial")
		.def(luabind::constructor<std::string, std::string>())
		.def(luabind::constructor<std::string, std::string, std::string>())
	];
}

template<>
void LuaInstance::BindClass<Vector2>()
{
	luabind::module(_luaState) [
	luabind::class_<Vector2>("Vector2")
		.def(luabind::constructor<float, float>())
		.def_readwrite("x", &Vector2::x)
		.def_readwrite("y", &Vector2::y)
	];
}

template<>
void LuaInstance::BindClass<World>()
{
	luabind::module(_luaState) [
	luabind::class_<World>("WorldClass")
		.def("addInvestigatorByInfo", &World::AddInvestigatorByInfo)
		.def("addTutorial", &World::AddTutorial)
		.def("getCurrentTutorial", &World::GetCurrentTutorial)
		.def("removeCurrentTutorial", &World::RemoveCurrentTutorial)
		.def("isFirstLaunch", &World::IsFirstLaunch)
		.def("getTutorialState", &World::GetTutorialState)
		.def("setTutorialState", &World::SetTutorialState)
		.def("runTutorialFuncton", &World::RunTutorialFunction)
	];
}

template<>
void LuaInstance::RegisterVariable<Log>(const char* name, Log* value)
{
	luabind::globals(_luaState)[name] = value;
}

template<>
void LuaInstance::RegisterVariable<NotificationMessageManager>(const char* name, NotificationMessageManager* value)
{
	luabind::globals(_luaState)[name] = value;
}

template<>
void LuaInstance::RegisterVariable<GameInfo>(const char* name, GameInfo* value)
{
	luabind::globals(_luaState)[name] = value;
}

template<>
void LuaInstance::RegisterVariable<World>(const char* name, World* value)
{
	luabind::globals(_luaState)[name] = value;
}
