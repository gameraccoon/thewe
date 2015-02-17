#include "Member.h"

#include "Log.h"

Member::Member(const std::string& specialization, int exp)
	: specialization(specialization)
	, experience(exp)
	, _state(State::NORMAL)
{
}

Member::Ptr Member::create(const std::string& specialization, int exp)
{
	return std::make_shared<Member>(specialization, exp);
}

int Member::getExperience() const
{
	return experience;
}

void Member::setExperience(int exp)
{
	WARN_IF(exp < 0, "negative experience value");
	experience = exp;
}

int Member::addExperience(int exp)
{
	WARN_IF(exp < 0, "trying to add negative experience value");
	experience += exp;
	return experience;
}

std::string Member::getSpecialization() const
{
	return specialization;
}

void Member::SetState(Member::State state)
{
	_state = state;
}

bool Member::IsState(Member::State state) const
{
	return _state == state;
}

bool Member::IsSpecial(const std::string special) const
{
	return specialization == special;
}

