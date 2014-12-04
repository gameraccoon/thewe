#include "TutorialManager.h"

#include <luabind/luabind.hpp>
#include "World.h"

void TutorialManager::AddTutorial(Tutorial tutrorial)
{
	_tutorials.push(std::make_shared<Tutorial>(tutrorial));
}

bool TutorialManager::IsHaveTutorial()
{
	return _tutorials.size() > 0;
}

Tutorial::WeakPtr TutorialManager::GetCurrentTutorial()
{
	return _tutorials.front();
}

void TutorialManager::RemoveCurrentTutorial()
{
	if (_tutorials.size() > 0)
	{
		if (!_tutorials.front()->luaCallback.empty())
		{
			luabind::call_function<void>(World::Instance().GetLuaInst()->GetLuaState()
				, _tutorials.front()->luaCallback.c_str()
				, 0);
		}

		_tutorials.pop();
	}
}

bool TutorialManager::IsTutorialStateAvailable(const std::string& state) const
{
	return _availableTutorialStates.find(state) != _availableTutorialStates.end();
}

void TutorialManager::AddTutorialState(const std::string& state)
{
	_availableTutorialStates.insert(state);
}

void TutorialManager::RemoveTutorialState(const std::string& state)
{
	_availableTutorialStates.erase(_availableTutorialStates.find(state));
}

void TutorialManager::RunTutorialFunction(const std::string& function)
{
	luabind::call_function<void>(World::Instance().GetLuaInst()->GetLuaState()
		, std::string("RunTutorial_" + function).c_str()
		, 0);
}

const std::set<std::string>& TutorialManager::GetTutorialStatements() const
{
	return _availableTutorialStates;
}
