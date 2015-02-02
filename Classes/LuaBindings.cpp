#include "LuaBindings.h"

#include <luabind/luabind.hpp>

#include "LuaInstance.h"

#include "Log.h"
#include "NotificationMessageManager.h"
#include "Member.h"
#include "Vector2.h"
#include "Cell.h"
#include "Task.h"
#include "GameInfo.h"
#include "World.h"
#include "Localization.h"
#include "TutorialManager.h"

template<>
void LuaInstance::BindClass<Cell>()
{
	luabind::module(_luaState) [
	luabind::class_<Cell>("Cell")
		.property("getUid", &Cell::GetUid)
		.property("experience", &Cell::GetExperience, &Cell::SetExperience)
		.def("calcConnectivity", &Cell::CalcConnectivity)
		.def("calcDistanceToTheRootCell", &Cell::CalcDistanceToTheRootCell)
		.property("cash", &Cell::GetCash, &Cell::SetCash)
		.def("getMembersCount", &Cell::GetMembersCount)
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
void LuaInstance::BindClass<Member>()
{
	luabind::module(_luaState) [
	luabind::class_<Member>("Member")
		.def(luabind::constructor<const std::string&, int>())
		.def("getSpecialization", &Member::getSpecialization)
		.property("experience", &Member::setExperience, &Member::setExperience)
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
		.def_readonly("level", &Task::Info::level)
	];
}

template<>
void LuaInstance::BindClass<Tutorial>()
{
	luabind::module(_luaState) [
	luabind::class_<Tutorial>("Tutorial")
		.def(luabind::constructor<std::string, std::string, std::string, std::string>())
		.def(luabind::constructor<std::string, std::string, std::string>())
		.def(luabind::constructor<std::string, std::string>())
		.def(luabind::constructor<std::string>())
	];
}

template<>
void LuaInstance::BindClass<TutorialManager>()
{
	luabind::module(_luaState) [
	luabind::class_<TutorialManager>("TutorialManagerClass")
		.def("addTutorial", &TutorialManager::AddTutorial)
		.def("getCurrentTutorial", &TutorialManager::GetCurrentTutorial)
		.def("removeCurrentTutorial", &TutorialManager::RemoveCurrentTutorial)
		.def("addTutorialState", &TutorialManager::AddTutorialState)
		.def("removeTutorialState", &TutorialManager::RemoveTutorialState)
		.def("isTutorialStateAvailable", &TutorialManager::IsTutorialStateAvailable)
		.def("runTutorialFuncton", &TutorialManager::RunTutorialFunction)
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
		.def("addInvestigatorByCellUid", &World::AddInvestigatorByCellUid)
		.def("isFirstLaunch", &World::IsFirstLaunch)
		.def("getTutorialManager", &World::GetTutorialManager)
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

namespace lua
{
	std::string GetLocalizedString(std::string id)
	{
		return LocalizationManager::Instance().getText(id.c_str());
	}

	void BindGameClasses(LuaInstance *luaInstance)
	{
		luaInstance->BindClass<Log>();
		luaInstance->BindClass<NotificationMessageManager>();
		luaInstance->BindClass<Member>();
		luaInstance->BindClass<GameInfo>();
		luaInstance->BindClass<World>();
		luaInstance->BindClass<Cell>();
		luaInstance->BindClass<const Task::Info>();
		luaInstance->BindClass<Vector2>();
		luaInstance->BindClass<Tutorial>();
		luaInstance->BindClass<TutorialManager>();
	}

	void BindFunctions(LuaInstance* luaInstance)
	{
		luabind::module(luaInstance->GetLuaState())
		[
			luabind::def("GetLocalizedString", &GetLocalizedString)
		];
	}

	void BindGlobalData(LuaInstance* luaInstance)
	{
		luaInstance->RegisterVariable("Log", &(Log::Instance()));
		luaInstance->RegisterVariable("MessageManager", &(World::Instance().GetMessageManager()));
		luaInstance->RegisterVariable("GameInfo", &(GameInfo::Instance()));
		luaInstance->RegisterVariable("World", &(World::Instance()));
	}
}
