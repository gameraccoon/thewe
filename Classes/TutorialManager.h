#ifndef TUTORIAL_MANAGER_H
#define TUTORIAL_MANAGER_H

#include "Tutorial.h"
#include <set>
#include <queue>

struct TutorialManager
{
public:
	void AddTutorial(Tutorial tutrorial);
	bool IsHaveTutorial();
	Tutorial::WeakPtr GetCurrentTutorial();
	void RemoveCurrentTutorial();
	bool IsTutorialStateAvailable(const std::string& state) const;
	void AddTutorialState(const std::string& state);
	void RemoveTutorialState(const std::string& state);
	void RunTutorialFunction(const std::string& function);
	const std::set<std::string>& GetTutorialStatements() const;

private:
	std::queue<Tutorial::Ptr> _tutorials;
	std::set<std::string> _availableTutorialStates;
};

#endif // TUTORIAL_MANAGER_H
