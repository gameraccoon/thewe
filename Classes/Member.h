#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <memory>
#include <vector>

class Member
{
public:
	typedef std::shared_ptr<Member> Ptr;
	typedef std::weak_ptr<Member> WeakPtr;
	typedef std::vector<Member::Ptr> Vector;

	enum class State
	{
		NORMAL,
		MISSION
	};

public:
	Member(const std::string& specialization, int exp);

	static Ptr create(const std::string& specialization, int exp);

	int getExperience() const;
	void setExperience(int exp);
	int addExperience(int exp);
	void SetState(Member::State state);
	std::string getSpecialization() const;

	bool IsState(Member::State state) const;
	bool IsSpecial(const std::string special) const;

private:
	const std::string specialization;
	int experience;
	State _state;
};

#endif // MEMBER_H
